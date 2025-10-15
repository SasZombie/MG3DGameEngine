#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop

#include "OctreeNode.hpp"

namespace sas
{

    // Here we are supposed to ovverwrite the queryIntersection function
    // That is marked virtual, but I think it is enought with just AABB
    // This is why I am not overrwriting anything here
    // The OctreeNode should be an interface but due to above, I am not
    // Marking the queryIntersection function as pure virtual

    struct Sphere : public OctreeNode
    {
        glm::vec3 center;
        float radius;
    };

    struct AABB : public OctreeNode
    {
        glm::vec3 min;
        glm::vec3 max;
    };

} // namespace sas