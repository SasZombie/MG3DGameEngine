#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include <vector>
#include <memory>
#include <functional>
#include "Asset.hpp"

namespace sas
{

    // Just so I can draw it for testing
    struct OctreeNode : public Asset
    {
        glm::vec3 minBounds;
        glm::vec3 maxBounds;

        std::vector<Asset *> objects;
        std::unique_ptr<OctreeNode> children[8] = {nullptr};
        int depth;
        int maxObjects = 5;

        OctreeNode(const glm::vec3 &minB, const glm::vec3 &maxB, const Asset &ass, int d = 0) noexcept;

        bool isLeaf() const noexcept;

        void subdivide()
        {
            glm::vec3 center = (minBounds + maxBounds) * 0.5f;

            for (int i = 0; i < 8; ++i)
            {
                glm::vec3 childMin = minBounds;
                glm::vec3 childMax = center;

                // Set x, y, z based on i
                if (i & 1)
                    childMin.x = center.x;
                else
                    childMax.x = center.x;
                if (i & 2)
                    childMin.y = center.y;
                else
                    childMax.y = center.y;
                if (i & 4)
                    childMin.z = center.z;
                else
                    childMax.z = center.z;

                Asset subDiv = static_cast<Asset&>(*this);
                subDiv.transform.scale = subDiv.transform.scale * glm::vec3{0.5, 0.5, 0.5};
                children[i] = std::make_unique<OctreeNode>(childMin, childMax, subDiv, depth + 1);
            }
        }

        void insert(Asset *obj, const glm::vec3 &objPos)
        {
            if (!isLeaf())
            {
                // Push into appropriate child
                for (auto &child : children)
                {
                    if (child->contains(objPos))
                    {
                        child->insert(obj, objPos);
                        return;
                    }
                }
            }

            objects.push_back(obj);

            if (objects.size() > maxObjects && depth < 8)
            {
                if (isLeaf())
                    subdivide();

                for (auto it = objects.begin(); it != objects.end();)
                {
                    bool moved = false;
                    for (auto &child : children)
                    {
                        if (child->contains((*it)->transform.position))
                        {
                            child->insert(*it, (*it)->transform.position);
                            it = objects.erase(it);
                            moved = true;
                            break;
                        }
                    }
                    if (!moved)
                        ++it;
                }
            }
        }

        bool contains(const glm::vec3 &point) const
        {
            return (point.x >= minBounds.x && point.x <= maxBounds.x) &&
                   (point.y >= minBounds.y && point.y <= maxBounds.y) &&
                   (point.z >= minBounds.z && point.z <= maxBounds.z);
        }

        void traverse(std::function<void(OctreeNode &)> func)
        {
            func(*this);
            if (!isLeaf())
            {
                for (auto &child : children)
                {
                    if (child)
                    {
                        child->traverse(func);
                    }
                }
            }
        }

        ~OctreeNode() noexcept = default;
    };

} // namespace sas