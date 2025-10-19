#pragma once

namespace sas
{
    class Globals
    {
    public:
        Globals(const Globals &) = delete;
        Globals &operator=(const Globals &) = delete;
        Globals(Globals &&) = delete;
        Globals &operator=(Globals &&) = delete;

        static Globals &instance() noexcept
        {
            static Globals instance;
            return instance;
        }

        void setDeltaTime(float dt) noexcept
        {
            deltaTime = dt;
        }
        float getDeltaTime() const noexcept
        {
            return deltaTime;
        }

    private:
        Globals() noexcept = default;
        ~Globals() noexcept = default;

        float deltaTime = 0.f;
        bool someFlag = false;
    };

} // namespace sas