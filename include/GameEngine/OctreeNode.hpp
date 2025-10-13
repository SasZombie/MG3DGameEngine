#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include <vector>
#include <memory>
#include "Asset.hpp"

namespace sas
{

    struct OctreeNode
    {
        glm::vec3 minBounds;
        glm::vec3 maxBounds;

        std::vector<Asset *> objects;
        std::unique_ptr<OctreeNode> children[8] = {nullptr};
        int depth;
        int maxObjects = 5;

        OctreeNode(const glm::vec3 &minB, const glm::vec3 &maxB, int d = 0) noexcept;

        bool isLeaf() const noexcept;

        ~OctreeNode() noexcept = default;
    };

    void subdivide(OctreeNode *node)
    {
        glm::vec3 center = (node->minBounds + node->maxBounds) * 0.5f;
        for (int i = 0; i < 8; ++i)
        {
            glm::vec3 newMin = node->minBounds;
            glm::vec3 newMax = center;

            if (i & 1)
                newMin.x = center.x, newMax.x = node->maxBounds.x;
            if (i & 2)
                newMin.y = center.y, newMax.y = node->maxBounds.y;
            if (i & 4)
                newMin.z = center.z, newMax.z = node->maxBounds.z;

            node->children[i] = std::make_unique<OctreeNode>(newMin, newMax, node->depth + 1);
        }
    }

    bool intersects(const glm::vec3 &minA, const glm::vec3 &maxA,
                    const glm::vec3 &minB, const glm::vec3 &maxB)
    {
        return (minA.x <= maxB.x && maxA.x >= minB.x) &&
               (minA.y <= maxB.y && maxA.y >= minB.y) &&
               (minA.z <= maxB.z && maxA.z >= minB.z);
    }

    void insert(OctreeNode *node, Asset *asset)
    {
        // Compute asset bounding box (use transform + mesh bounds)
        glm::vec3 assetMin = asset->transform.position - 100.f;
        glm::vec3 assetMax = asset->transform.position + 100.f;
                                                                // asset->mesh->boundingRadius

        // If asset doesn't intersect this node, skip
        if (!intersects(node->minBounds, node->maxBounds, assetMin, assetMax))
            return;

        if (node->isLeaf() && node->objects.size() < node->maxObjects)
        {
            node->objects.push_back(asset);
            return;
        }

        if (node->isLeaf())
        {
            subdivide(node); // create 8 children
            // redistribute existing objects
            for (Asset *a : node->objects)
            {
                for (int i = 0; i < 8; ++i)
                    insert(node->children[i].get(), a);
            }
            node->objects.clear();
        }

        // Insert the new asset into appropriate children
        for (int i = 0; i < 8; ++i)
            insert(node->children[i].get(), asset);
    }

    

} // namespace sas