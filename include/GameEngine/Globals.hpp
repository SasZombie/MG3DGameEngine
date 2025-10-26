#pragma once

#ifdef _WIN32
    #ifdef BUILDING_GAME_ENGINE
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #define ENGINE_API
#endif


namespace sas
{
    class ENGINE_API Globals
    {
    public:
        Globals(const Globals &) = delete;
        Globals &operator=(const Globals &) = delete;
        Globals(Globals &&) = delete;
        Globals &operator=(Globals &&) = delete;

        static Globals &instance() noexcept;

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