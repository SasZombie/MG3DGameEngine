#include "OctreeNode.hpp"


sas::OctreeNode::OctreeNode(const glm::vec3 &pos, const glm::vec3 &size) noexcept
    : position(pos), sizexyz(size)
{
}

bool sas::OctreeNode::isLeaf() const noexcept
{
    return children.empty();
}
void sas::OctreeNode::subdivide(const Asset &node) noexcept
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

    size_t octant;
    // We know that we have maxObjects
    // Otherwise we wouldn't be here
    for (size_t i = 0; i < maxObjects; ++i)
    {
        octant = getOctan(elements[i]);
        children[octant].insert(elements[i]);
    }

    octant = getOctan(node);
    children[octant].insert(node);
}

size_t sas::OctreeNode::getOctan(const Asset &node) const noexcept
{
    int octan = 0;
    const auto &nodePosition = node.worldTransform.position;
    if (position.x >= nodePosition.x)
        octan |= 1;
    if (position.y >= nodePosition.y)
        octan |= 2;
    if (position.z >= nodePosition.z)
        octan |= 4;

    return octan;
}

void sas::OctreeNode::insert(const Asset &node) noexcept
{
    if (isLeaf())
    {
        if (count >= maxObjects)
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

bool sas::OctreeNode::intersectAABB(const glm::vec3 &pos1, const glm::vec3 &size1,
                                    const glm::vec3 &pos2, const glm::vec3 &size2) const noexcept
{
    return !(pos1.x + size1.x < pos2.x || pos1.x > pos2.x + size2.x ||
             pos1.y + size1.y < pos2.y || pos1.y > pos2.y + size2.y ||
             pos1.z + size1.z < pos2.z || pos1.z > pos2.z + size2.z);
}

// The container could be something other than a std::vector
void sas::OctreeNode::queryIntersection(const sas::Asset &ast, std::vector<sas::Asset> &results) const noexcept
{

    if (!intersectAABB(position, sizexyz, ast.worldTransform.position, ast.worldTransform.scale))
        return;

    if (isLeaf())
    {
        for (size_t i = 0; i < count; ++i)
        {
            const auto &asset = elements[i];
            if (intersectAABB(asset.worldTransform.position, asset.worldTransform.scale, ast.worldTransform.position, ast.worldTransform.scale))
            {
                results.push_back(asset);
            }
        }
    }
    else
    {
        for (const auto &child : children)
        {
            child.queryIntersection(ast, results);
        }
    }
}