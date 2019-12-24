#pragma once

#include <functional>

namespace reactive
{
    namespace detail
    {
        template <typename Sender,
                  typename Predicate,
                  typename MessageType = typename Sender::value_type>
        class filter_impl
        {
        public:
            using value_type = MessageType;

            filter_impl(Sender && sender, Predicate predicate)
                : _sender{std::move(sender)}
                , _predicate{predicate}
            {}

            template <typename EmitFunction>
            void on_message(EmitFunction emit)
            {
                _emit = emit;
                _sender.on_message(
                    [this] (MessageType &&message) {
                        process_message(std::move(message));
                    });
            }

            void process_message(MessageType && message) const
            {
                if (_emit && std::invoke(_predicate, message))
                    _emit(std::move(message));
            }

        private:
            Sender _sender;
            Predicate _predicate;
            std::function<void(MessageType&&)> _emit;
        };

        template <typename Predicate>
        struct filter_helper
        {
            Predicate predicate;
        };
    } // namespace detail

    template <typename Sender, typename Predicate>
    auto filter(Sender && sender, Predicate && predicate)
    {
        return detail::filter_impl<Sender, Predicate>(
            std::forward<Sender>(sender),
            std::forward<Predicate>(predicate));
    }

    namespace operators
    {
        template <typename Predicate>
        auto filter(Predicate && predicate)
        {
            return detail::filter_helper<Predicate>{
                std::forward<Predicate>(predicate)};
        }

        template <typename Sender, typename Predicate>
        auto operator|(Sender && sender, detail::filter_helper<Predicate> filter)
        {
            return detail::filter_impl<Sender, Predicate>(
                std::forward<Sender>(sender), filter.predicate);
        }
    } // namespace operators
} // namespace reactive
