#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop

namespace sas
{
    struct CollisionObject
    {
        virtual void checkCollision() = 0;
    };

    // TOOD: Maybe implement Shhere as well
    struct Sphere : CollisionObject
    {
        glm::vec3 center;
        float radius;

        void checkCollision() override
        {
            assert(false && "TODO: Sphere Collision");
        }
    };

    struct AABB : CollisionObject
    {
        glm::vec3 min;
        glm::vec3 max;

        void checkCollision() override
        {
            assert(false && "TODO: Collision AABB");
        }
    };

} // namespace sas