#pragma once

#include <functional>

namespace reactive
{
    namespace detail
    {
        template <typename Sender,
                  typename Transformation,
                  typename SourceMessageType = typename Sender::value_type,
                  typename MessageType = decltype(std::declval<Transformation>()
                                                 (std::declval<SourceMessageType>()))>
        class transform_impl
        {
        public:
            using value_type = MessageType;

            transform_impl(Sender && sender, Transformation transformation)
                : _sender{std::move(sender)}
                , _transformation{transformation}
            {}

            template <typename EmitFunction>
            void on_message(EmitFunction emit)
            {
                _emit = emit;
                _sender.on_message(
                    [this] (SourceMessageType && message) {
                            process_message(std::move(message));
                        });
            }

            void process_message(SourceMessageType && message) const
            {
                _emit(std::invoke(_transformation, std::move(message)));
            }

        private:
            Sender _sender;
            Transformation _transformation;
            std::function<void(MessageType&&)> _emit;
        };

        template <typename Transformation>
        struct transform_helper
        {
            Transformation function;
        };
    } // namespace detail

    template <typename Sender, typename Transformation>
    auto transform(Sender && sender, Transformation && transformation)
    {
        return detail::transform_impl<Sender, Transformation>(
                    std::forward<Sender>(sender),
                    std::forward<Transformation>(transformation));
    }

    namespace operators
    {
        template <typename Transformation>
        auto transform(Transformation && transformation)
        {
            return detail::transform_helper<Transformation>{
                        std::forward<Transformation>(transformation)};
        }

        template <typename Sender, typename Transformation>
        auto operator|(Sender && sender,
                    detail::transform_helper<Transformation> transformation)
        {
            return detail::transform_impl<Sender, Transformation>(
                        std::forward<Sender>(sender), transformation.function);
        }
    } // namespace operators
} // namespace reactive