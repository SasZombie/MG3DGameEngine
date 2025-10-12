#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop


#include <tuple>

namespace sas
{
    class Object
    {
    protected:
        float x = 0.f, y = 0.f, z = 0.f;
    public:
        
        Object() = default;
        
        constexpr Object(float n_x, const float n_y, const float n_z)
            : x(n_x), y(n_y), z(n_z)
        {

        }

        constexpr Object(const glm::vec3& vect )
            : x(vect.x), y(vect.y), z(vect.z)
        {
        }

        constexpr std::tuple<float, float, float> getCoords() const noexcept
        {
            return std::make_tuple(x, y, z);
        }

        void setX(float nx) noexcept;
        void setY(float ny) noexcept;
        void setZ(float nz) noexcept;
        void setCoords(float nx, float ny, float nz) noexcept;

        ~Object() = default;
    };      
} 