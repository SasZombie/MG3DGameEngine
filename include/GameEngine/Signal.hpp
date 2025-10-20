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
        using callBack = std::function<void(Args...)>;

        void connect(callBack cb) noexcept
        {
            callBacks.push_back(cb);
        }

        void clear() noexcept
        {
            callBacks.clear();
        }

        void emit(Args... args)
        {
            for(auto& s : callBacks)
            {
                s(args...);
            }
        }

    private:
        std::vector<callBack> callBacks;
    };

    
} // namespace sas