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
#include "SceneNode.hpp"

namespace sas
{
    class OctreeNode
    {

    public:
        OctreeNode() = default;
        OctreeNode(const glm::vec3 &pos, const glm::vec3 &size)
            : position(pos), sizexyz(size)
        {
        }
        glm::vec3 position;
        glm::vec3 sizexyz;

        std::array<SceneNode, 5> elements;
        std::vector<OctreeNode> children;

        int maxObjects = 5;
        int count = 0;

        bool isLeaf()
        {
            return children.empty();
        }
        void subdivide(const SceneNode &node)
        {
            count = 0;
            float hw = sizexyz.x / 2;
            float hh = sizexyz.y / 2;
            float hd = sizexyz.z / 2;

            children.emplace_back(glm::vec3{-hw / 2, -hh / 2, -hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{hw / 2, -hh / 2, hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{-hw / 2, hh / 2, -hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{hw / 2, hh / 2, -hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{-hw / 2, -hh / 2, hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{hw / 2, -hh / 2, hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{-hw / 2, hh / 2, hd / 2}, glm::vec3{hw, hh, hd});
            children.emplace_back(glm::vec3{hw / 2, hh / 2, hd / 2}, glm::vec3{hw, hh, hd});

            for(size_t i = 0; i < maxObjects; ++i)
            {
                size_t octant = getOctan(elements[i]);
                children[octant].insert(elements[i]);
            }
        }

        size_t getOctan(const SceneNode &node)
        {
            int octan = 0;

            if(position.x >= )
        }

        void insert(const SceneNode &node)
        {
            if (isLeaf())
            {
                if (count > maxObjects)
                {
                    subdivide(node);
                }
                else
                {
                    elements[count] = node;
                    ++count;
                }
            }
            else
            {
                size_t minDist = getOctan(node);
                children[minDist].insert(node);
            }
        }
    };

} // namespace sas