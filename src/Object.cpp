#include "Object.hpp"

void sas::Object::setCoords(float nx, float ny, float nz) noexcept
{
    this->x = nx;
    this->y = ny;
    this->z = nz;
}

void sas::Object::setX(float nx) noexcept
{
    this->x = nx;
}


void sas::Object::setY(float ny) noexcept
{
    this->y = ny;
}

void sas::Object::setZ(float nz) noexcept
{
    this->z = nz;
}