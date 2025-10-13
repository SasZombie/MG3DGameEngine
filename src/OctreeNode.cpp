#include "OctreeNode.hpp"

sas::OctreeNode::OctreeNode(const glm::vec3 &minB, const glm::vec3 &maxB, int d) noexcept
    : minBounds(minB), maxBounds(maxB), depth(d)
{
    
}

bool sas::OctreeNode::isLeaf() const noexcept
{
    return children[0] == nullptr;
}
