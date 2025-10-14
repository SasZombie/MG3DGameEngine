#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include <vector>
#include <memory>
#include <iostream>
// Pyramid shaped
class Frustum
{
public:
    glm::vec4 planes[6];

    void update(const glm::mat4 &proj, const glm::mat4 &view)
    {
        glm::mat4 clip = proj * view;

        planes[0] = glm::vec4(
            clip[0][3] + clip[0][0],
            clip[1][3] + clip[1][0],
            clip[2][3] + clip[2][0],
            clip[3][3] + clip[3][0]);

        planes[1] = glm::vec4(
            clip[0][3] - clip[0][0],
            clip[1][3] - clip[1][0],
            clip[2][3] - clip[2][0],
            clip[3][3] - clip[3][0]);

        planes[2] = glm::vec4(
            clip[0][3] + clip[0][1],
            clip[1][3] + clip[1][1],
            clip[2][3] + clip[2][1],
            clip[3][3] + clip[3][1]);

        planes[3] = glm::vec4(
            clip[0][3] - clip[0][1],
            clip[1][3] - clip[1][1],
            clip[2][3] - clip[2][1],
            clip[3][3] - clip[3][1]);

        planes[4] = glm::vec4(
            clip[0][3] + clip[0][2],
            clip[1][3] + clip[1][2],
            clip[2][3] + clip[2][2],
            clip[3][3] + clip[3][2]);

        planes[5] = glm::vec4(
            clip[0][3] - clip[0][2],
            clip[1][3] - clip[1][2],
            clip[2][3] - clip[2][2],
            clip[3][3] - clip[3][2]);

        for (int i = 0; i < 6; ++i)
        {
            float length = glm::length(glm::vec3(planes[i]));
            planes[i] /= length;
        }
    }

    bool intersects(const glm::vec3 &minB, const glm::vec3 &maxB) const
    {
        for (int i = 0; i < 6; ++i)
        {
            std::cout << "i = " << i << '\n';
            std::cout << planes[i].x << ' ' << planes[i].y << ' ' << planes[i].z << '\n';
        
            glm::vec3 positive = minB;
            if (planes[i].x >= 0)
                positive.x = maxB.x;
            if (planes[i].y >= 0)
                positive.y = maxB.y;
            if (planes[i].z >= 0)
                positive.z = maxB.z;

            if (glm::dot(glm::vec3(planes[i]), positive) + planes[i].w < 0)
                return false;
        }
        return true;
    }
};
