#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop

#include "Transform.hpp"

namespace sas
{

    struct AABB;
    struct Sphere;

///@brief
/*
    The collision when something is attached to a camerais not yet
    Implemented
    This is because unlike an average asset
    Camera rotates and objects attached to it
    Rottate as well. This makes it annoying.
    Collision between 2 objects work always even when rotated

    This does not support mixing types
    AABB with Sphere
    If we want that we should employ some kind
    Of visitor pattern
*/
    struct CollisionObject
    {
        bool isColliding = false;
        Transform worldTransform;

        virtual bool checkCollision(const CollisionObject &other) = 0;
        virtual void uppdateBounds(const Transform &newWorldTransform) = 0;

        virtual ~CollisionObject() = default;
    };

    // TOOD: Maybe implement Shhere as well
    struct Sphere : public CollisionObject
    {
        glm::vec3 center;
        float radius;

        bool checkCollision(const CollisionObject &other) override
        {
            assert(false && "TODO: Sphere Collision");
            return true;
        }

        void uppdateBounds(const Transform &newWorldTransform) override
        {
            assert(false && "TODO: Sphere uppdate Bounds");
        }
    };

    struct AABB : public CollisionObject
    {
        glm::vec3 min;
        glm::vec3 max;

        AABB() = default;

        AABB(const glm::vec3 &nMin, const glm::vec3 &nMax) noexcept
            : min(nMin), max(nMax)
        {
        }

        bool checkCollision(const CollisionObject &otherAABB) override
        {
            const AABB &other = static_cast<const AABB &>(otherAABB);
            // std::cout << "Dau check la min = " << min.x << ' ' << min.y << ' ' << min.z << '\n'
            //           << "Dau check la min = " << max.x << ' ' << max.y << ' ' << max.z << '\n'
            //           << "Dau check la min = " << other.min.x << ' ' << other.min.y << ' ' << other.min.z << '\n'
            //           << "Dau check la min = " << other.max.x << ' ' << other.max.y << ' ' << other.max.z << '\n';

            return (other.min.x <= this->max.x && other.max.x >= this->min.x) &&
                   (other.min.y <= this->max.y && other.max.y >= this->min.y) &&
                   (other.min.z <= this->max.z && other.max.z >= this->min.z);
        }

        void uppdateBounds(const Transform &newWorldTransform) override
        {
            worldTransform = newWorldTransform;
            min = worldTransform.position - worldTransform.scale * 0.5f;
            max = worldTransform.position + worldTransform.scale * 0.5f;
        }

        ~AABB() = default;
    };

} // namespace sas
