#include "OctreeNode.hpp"

sas::OctreeNode::OctreeNode(const glm::vec3 &minB, const glm::vec3 &maxB, const Asset& ass, int d) noexcept
    : minBounds(minB), maxBounds(maxB), depth(d), Asset(ass)
{
    
}

bool sas::OctreeNode::isLeaf() const noexcept
{
    return children[0] == nullptr;
}
