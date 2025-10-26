#define BUILDING_GAME_ENGINE
#include"Globals.hpp"

sas::Globals& sas::Globals::instance() noexcept
{
    static Globals g;
    return g;
}