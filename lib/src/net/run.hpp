#pragma once

#include "mtry.hpp"
#include "filter.hpp"
#include "sink.hpp"
#include "transform.hpp"
#include "service.hpp"

#include "../http/request_parser.hpp"

void run()
{
    using namespace reactive::operators;

    auto transform = [](auto f) {
            return reactive::operators::transform(lift_with_client(f));
        };
    auto filter = [](auto f) {
            return reactive::operators::filter(apply_with_client(f));
        };

    boost::asio::io_service event_loop;

    auto pipeline =
        service(event_loop)
            | filter([] (std::string const & message) {
                return message.length() > 0;
            })

            | transform([] (std::string const & message) {
                return mtry([&] {
                    return http::parse_request(message);
                });
            })

            | sink([] (auto const & message) {
                auto const & request = message.value;

                if (!request)
                {
                    message.reply(handle_eptr(request.error()));
                    return;
                }
                else
                {
                    message.reply("200 OK\n");
                    return;
                }
            });

    std::cerr << "Service is running...\n";
    event_loop.run();
}
