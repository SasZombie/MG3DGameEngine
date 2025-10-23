#include "OctreeNode.hpp"

sas::OctreeNode::OctreeNode(const glm::vec3 &pos, const glm::vec3 &size) noexcept
    : position(pos), sizexyz(size)
{
}

bool sas::OctreeNode::isLeaf() const noexcept
{
    return children.empty();
}

void sas::OctreeNode::subdivide(Asset *node) noexcept
{
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

    size_t octant = 0;
    // We know that we have maxObjects
    // Otherwise we wouldn't be here
    for (size_t i = 0; i < maxObjects; ++i)
    {
        octant = getOctan(*elements[i]);
        children[octant].insert(elements[i]);
    }

    octant = getOctan(*node);
    children[octant].insert(node);
    count = 0;
}

size_t sas::OctreeNode::getOctan(const Asset &node) const noexcept
{
    int octan = 0;
    const auto &nodePosition = node.worldTransform.position;
    // std::cout << "Node Position = " << nodePosition.x << ' ' << nodePosition.y << ' ' << nodePosition.z << '\n';
    if (position.x >= nodePosition.x)
        octan |= 1;
    if (position.y >= nodePosition.y)
        octan |= 2;
    if (position.z >= nodePosition.z)
        octan |= 4;

    return octan;
}

void sas::OctreeNode::insert(Asset *node) noexcept
{
    std::cout << "Insert\n";
#ifdef debugMode

    if (!node->getCollisionObject())
    {
        // std::cerr << "Warning! Object inserted without any Collision Object(s)  \nNote that"
        //              " only the object that has the collision object should/needs to be inserted\n"
        //              "If you plan on adding it later, you can safely ignore this warning";
    }
#endif

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
        size_t minDist = getOctan(*node);
        children[minDist].insert(node);
    }
}

void sas::OctreeNode::addHitboxAsset(Asset *asset) noexcept
{
    this->hitbox = asset;
}

void sas::OctreeNode::drawAsset(const Camera *camera) noexcept
{
    for (size_t i = 0; i < count; ++i)
    {
        const auto &asset = elements[i];

        hitbox->worldTransform = asset->getCollisionObject()->worldTransform;
        // std::cout << asset->getCollisionObject()->worldTransform << '\n';
        // hitbox->localTransform = asset->getCollisionObject()->worldTransform //asset->localTransform;

        if (auto p = asset->parent.lock())
        {
            // Dont like this hack
            // But drawing the hitboxes
            // Are for debuging only
            // So it does not matter
            if (std::dynamic_pointer_cast<Camera>(p))
            {
                hitbox->drawAttachedToCamera(camera);
                continue;
            }
        }
        hitbox->draw(camera);
    }
}

// bool sas::OctreeNode::intersectAABB(const glm::vec3 &pos1, const glm::vec3 &size1,
//                                     const glm::vec3 &pos2, const glm::vec3 &size2) const noexcept
// {
//     return !(pos1.x + size1.x < pos2.x || pos1.x > pos2.x + size2.x ||
//              pos1.y + size1.y < pos2.y || pos1.y > pos2.y + size2.y ||
//              pos1.z + size1.z < pos2.z || pos1.z > pos2.z + size2.z);
// }

bool sas::OctreeNode::intersectAABB(const glm::vec3 &center1, const glm::vec3 &size1,
                                    const glm::vec3 &center2, const glm::vec3 &size2) const noexcept
{
    glm::vec3 half1 = size1 * 0.5f;
    glm::vec3 half2 = size2 * 0.5f;

    glm::vec3 min1 = center1 - half1;
    glm::vec3 max1 = center1 + half1;
    glm::vec3 min2 = center2 - half2;
    glm::vec3 max2 = center2 + half2;

    return !(max1.x < min2.x || min1.x > max2.x ||
             max1.y < min2.y || min1.y > max2.y ||
             max1.z < min2.z || min1.z > max2.z);
}

// The container could be something other than a std::vector
void sas::OctreeNode::queryIntersection(sas::Asset &ast, std::vector<sas::Asset *> &results) const noexcept
{
    // General intersection for this. We don't care about accuracy here

    if (!intersectAABB(position, sizexyz, ast.worldTransform.position, ast.worldTransform.scale))
        return;

    if (isLeaf())
    {
        for (size_t i = 0; i < count; ++i)
        {
            const auto &asset = elements[i];

            // Skip Self
            if (*asset == ast)
                continue;

            if (ast.getCollisionObject()->checkCollision(*asset->getCollisionObject()))
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

void sas::OctreeNode::queryIntersection(std::vector<sas::Asset *> &results) const noexcept
{

    if (isLeaf())
    {
        for (size_t i = 0; i < count; ++i)
        {
            queryIntersection(*elements[i], results);
        }
    }
    else
    {
        for (const auto &child : children)
        {
            child.queryIntersection(results);
        }
    }
}

static bool pointInPOV(const glm::vec3 &camPos, const glm::vec3 &camForward, const glm::vec3 &camUp, const glm::vec3 &camRight,
                       float fovY, float aspect, float viewRange,
                       const glm::vec3 &point)
{
    glm::vec3 toPoint = point - camPos;
    float dist2 = glm::dot(toPoint, toPoint);
    if (dist2 > viewRange * viewRange)
        return false;

    glm::vec3 dir = glm::normalize(toPoint);

    // This projects into the camera
    float forwardDot = glm::dot(dir, camForward);
    float rightDot = glm::dot(dir, camRight);
    float upDot = glm::dot(dir, camUp);

    float halfVFOV = fovY * 0.5f;
    float halfHFOV = atan(tan(halfVFOV) * aspect);

    // Angular checks
    float horizAngle = atan2(rightDot, forwardDot);
    float vertAngle = atan2(upDot, forwardDot);

    return fabs(horizAngle) <= halfHFOV && fabs(vertAngle) <= halfVFOV;
}

bool sas::OctreeNode::intersectsView(const Camera *cam,
                                     float fovY,
                                     float aspect,
                                     float viewRange,
                                     const glm::vec3 &nodeMin,
                                     const glm::vec3 &nodeMax) const noexcept
{
    glm::vec3 camPos = cam->worldTransform.position;

    std::array<glm::vec3, 8> corners = {
        glm::vec3(nodeMin.x, nodeMin.y, nodeMin.z),
        glm::vec3(nodeMax.x, nodeMin.y, nodeMin.z),
        glm::vec3(nodeMin.x, nodeMax.y, nodeMin.z),
        glm::vec3(nodeMax.x, nodeMax.y, nodeMin.z),
        glm::vec3(nodeMin.x, nodeMin.y, nodeMax.z),
        glm::vec3(nodeMax.x, nodeMin.y, nodeMax.z),
        glm::vec3(nodeMin.x, nodeMax.y, nodeMax.z),
        glm::vec3(nodeMax.x, nodeMax.y, nodeMax.z)};

    for (const auto &c : corners)
        if (pointInPOV(camPos, cam->getCameraViewDirection(), cam->getCameraUp(), cam->getCameraRight(),
                       fovY, aspect, viewRange, c))
            return true;

    // If inside cube
    if (camPos.x >= nodeMin.x && camPos.x <= nodeMax.x &&
        camPos.y >= nodeMin.y && camPos.y <= nodeMax.y &&
        camPos.z >= nodeMin.z && camPos.z <= nodeMax.z)
        return true;

    return false;
}

void sas::OctreeNode::querryView(const Camera *cam, float fovY, float aspect, float viewRange,
                                 std::vector<Asset *> &visible) const noexcept
{
    if (!intersectsView(cam, fovY, aspect, viewRange, this->position, this->sizexyz))
        return;

    if (isLeaf())
    {
        for (size_t i = 0; i < count; ++i)
        {
            Asset *asst = elements[i];
            if (!asst)
                continue;

            glm::vec3 pos = asst->worldTransform.position;
            if (pointInPOV(cam->worldTransform.position,
                           cam->getCameraViewDirection(),
                           cam->getCameraUp(),
                           cam->getCameraRight(),
                           fovY, aspect, viewRange, pos))
            {
                visible.push_back(asst);
            }
        }
    }
    else
    {
        for (const auto &child : children)
            child.querryView(cam, fovY, aspect, viewRange, visible);
    }
}

void sas::OctreeNode::checkCollisions() const noexcept
{
    std::vector<Asset *> allAssets;
    getAllPoints(allAssets);

    for (auto &p : allAssets)
    {
        std::vector<Asset *> results;

        // std::cout << "{\n";
        // std::cout << p->getCollisionObject()->worldTransform << '\n';
        queryIntersection(*p, results);

        for (auto &other : results)
        {
            p->emit(*other);
        }
    }
}

void sas::OctreeNode::getAllPoints(std::vector<Asset *> &result) const noexcept
{
    if (isLeaf())
    {
        for (size_t i = 0; i < count; ++i)
        {
            result.push_back(elements[i]);
        }
    }
    else
    {
        for (auto &child : children)
        {
            child.getAllPoints(result);
        }
    }
}
