#include "Transform.hpp"

#include <iostream>

namespace sas
{
    // std::ostream &operator<<(std::ostream &os, const sas::Transform &t)
    // {
    //     os << "Position " << t.position.x << ' ' << t.position.y << ' ' << t.position.z << '\n'
    //        << "Scale " << t.scale.x << ' ' << t.scale.y << ' ' << t.scale.z << '\n'
    //        << "Rotation " << t.rotation.x << ' ' << t.rotation.y << ' ' << t.rotation.z << '\n';

    //     return os;
    // }

    std::ostream &operator<<(std::ostream &os, const Transform &t)
    {
        os << t.position.x << ' ' << t.position.y << ' ' << t.position.z << '\n'
           << t.scale.x << ' ' << t.scale.y << ' ' << t.scale.z << '\n'
           << t.rotation.x << ' ' << t.rotation.y << ' ' << t.rotation.z << '\n';
        return os;
    }

    std::istream &operator>>(std::istream &is, Transform &t)
    {
        is >> t.position.x >> t.position.y >> t.position.z 
           >> t.scale.x >> t.scale.y >> t.scale.z
           >> t.rotation.x >> t.rotation.y >> t.rotation.z;
        return is;
    }

} // namespace sas