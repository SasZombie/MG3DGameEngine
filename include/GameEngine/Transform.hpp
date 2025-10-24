#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include <iostream>

namespace sas
{

    struct Transform
    {
        glm::vec3 position{0.f};
        glm::vec3 rotation{0.f};
        glm::vec3 scale{1.f};

        glm::mat4 getModelMatrix() const noexcept
        {
            glm::mat4 M = glm::mat4(1.0f);

            M = glm::translate(M, position);

            M = glm::rotate(M, rotation.x, glm::vec3{1.f, 0.f, 0.f});
            M = glm::rotate(M, rotation.y, glm::vec3{0.f, 1.f, 0.f});
            M = glm::rotate(M, rotation.z, glm::vec3{0.f, 0.f, 1.f});

            M = glm::scale(M, scale);

            return M;
        }

        Transform combine(const Transform &parent) const noexcept
        {
            Transform result;

            result.scale = parent.scale * scale;
            result.rotation = parent.rotation + rotation;
            result.position = parent.position + position;

            return result;
        }

        bool operator==(const Transform &other) const
        {
            return position == other.position && rotation == other.rotation && scale == other.scale;
        }

        friend std::ostream &operator<<(std::ostream &os, const Transform &t);
    };
} // namespace sas
