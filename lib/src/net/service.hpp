#pragma once

#include <iostream>
#include <functional>
#include <regex>

#include <boost/asio.hpp>

namespace detail
{
    template <typename MessageType>
    struct with_client
    {
        MessageType value;
        boost::asio::ip::tcp::socket * socket;

        void reply(std::string const & message) const
        {
            boost::asio::async_write(
                    *socket,
                    boost::asio::buffer(message, message.length()),
                                    [] (auto, auto) {});
        }
    };
} // namespace detail

template <typename MessageType>
auto make_with_client(MessageType && value,
                      boost::asio::ip::tcp::socket * socket)
{
    return ::detail::with_client<MessageType>{
        std::forward<MessageType>(value), socket};
}

template <typename F>
auto lift_with_client(F && function)
{
    return [function = std::forward<F>(function)] (auto && ws) {
        return make_with_client(std::invoke(function, ws.value), ws.socket);
    };
}

template <typename F>
auto apply_with_client(F && function)
{
    return [function = std::forward<F>(function)] (auto &&ws) {
        return std::invoke(function, std::forward<decltype(ws)>(ws).value);
    };
}

template <typename EmitFunction>
class session
    : public std::enable_shared_from_this<session<EmitFunction>>
{
public:
    session(boost::asio::ip::tcp::socket&& socket, EmitFunction emit)
        : _socket{std::move(socket)}
        , _emit{emit}
    {}

    void start()
    {
        do_read();
    }

private:
    using shared_session = std::enable_shared_from_this<session<EmitFunction>>;

    void do_read()
    {
        auto self = shared_session::shared_from_this();
        boost::asio::async_read_until(
            _socket, _data, "\r\n\r\n",
            [this, self](boost::system::error_code const & error,
                         std::size_t size) {
                if (!error)
                {
                    std::istream is(&_data);
                    std::string line;
                    std::string res;

                    do
                    {
                        std::getline(is, line);
                        res.append(line).push_back('\n');
                    } while (line != "\r");

                    std::smatch match;
                    std::regex reg("content-length: *(([[:digit:]])+)",
                        std::regex_constants::ECMAScript | std::regex_constants::icase);

                    if (std::regex_search(res, match, reg))
                    {
                        do_read_body();

                        size_t body_length = std::stoull(match[1]);

                        char * body = new char[body_length];
                        for (size_t i = 0; i < body_length; ++i)
                            body[i] = 0;

                        is.read(body, body_length);

                        res.append(body);
                        delete[] body;
                    }
                    _emit(make_with_client(std::move(res), &_socket));
                    do_read();
                }
            });
    }

    void do_read_body()
    {
        auto self = shared_session::shared_from_this();
        boost::asio::read(
            _socket, _data,
            [this, self](boost::system::error_code const & error,
                         std::size_t size) {
                return size;
            });
    }

    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _data;
    EmitFunction _emit;
};

template <typename Socket, typename EmitFunction>
auto make_shared_session(Socket && socket, EmitFunction && emit)
{
    return std::make_shared<session<EmitFunction>>(
                std::forward<Socket>(socket),
                std::forward<EmitFunction>(emit));
}

class service
{
public:
    using value_type = ::detail::with_client<std::string>;

    explicit service(boost::asio::io_service& service,
                     unsigned short port = 42042)
        : _acceptor{service, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)}
        , _socket{service}
    {}

    service(service const & other) = delete;
    service(service && other) = default;

    template <typename EmitFunction>
    void on_message(EmitFunction emit)
    {
        _emit = emit;
        do_accept();
    }

private:
    void do_accept()
    {
        _acceptor.async_accept(
            _socket, [this] (boost::system::error_code const & error) {
                if (!error)
                {
                    make_shared_session(
                            std::move(_socket),
                            _emit
                        )->start();
                }
                else
                {
                    std::cerr << error.message() << std::endl;
                }

                do_accept();
            });
    }

    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;
    std::function<void(::detail::with_client<std::string>&&)> _emit;
};
