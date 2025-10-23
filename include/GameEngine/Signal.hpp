#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace sas
{

    template <typename... Args>
    class Signal
    {
    public:
        //OnHitCallBack :)
        using collsionCallback = std::function<void(Args...)>;

        Signal() = default;
        Signal(const Signal &) = delete;
        Signal &operator=(const Signal &) = delete;

        void connect(collsionCallback cb) noexcept
        {
            callBacks.push_back(cb);
        }

        void clear() noexcept
        {
            callBacks.clear();
        }

        void emit(Args... args)
        {
            for (auto &s : callBacks)
            {
                s(args...);
            }
        }

    private:
        std::vector<collsionCallback> callBacks;
    };

} // namespace sas