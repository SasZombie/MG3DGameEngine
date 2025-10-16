#include "Transform.hpp"

#include <iostream>

std::ostream & sas::operator<<(std::ostream &os, const sas::Transform &t)
{   
    os << "Position " <<  t.position.x << ' ' << t.position.y << ' ' << t.position.z << '\n'
    << "Scale " <<  t.scale.x << ' ' << t.scale.y << ' ' << t.scale.z << '\n'
    << "Rotation " <<  t.rotation.x << ' ' << t.rotation.y << ' ' << t.rotation.z << '\n';

    return os;
}