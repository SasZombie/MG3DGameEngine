#pragma once
#include <ostream>
#include <istream>

namespace sas
{
    enum struct SerializeCodes
    {
        ROOT,
        ASSET,
        CAMERA,
        SKYBOX,
        UI,
        END,
        ENDROOT
    };

    inline std::ostream &operator<<(std::ostream &os, SerializeCodes code)
    {
        return os << static_cast<int>(code);
    }

    inline std::istream &operator>>(std::istream &is, SerializeCodes &code)
    {
        int value;
        is >> value;
        code = static_cast<SerializeCodes>(value);
        return is;
    }
}