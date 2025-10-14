#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"


#include <glm/glm.hpp>

#pragma GCC diagnostic pop

//Sphere : public Octree
struct Sphere
{
    glm::vec3 center;
    float radius;
};

//TODO: Do support for this
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};
