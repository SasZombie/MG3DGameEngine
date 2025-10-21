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
    class OctreeNode
    {
        glm::vec3 position;
        glm::vec3 sizexyz;

        mutable Asset *hitbox;
        std::array<Asset *, 5> elements;
        std::vector<OctreeNode> children;

        size_t maxObjects = 5;
        size_t count = 0;

        bool intersectsView(const Camera *cam,
                            float fovY,
                            float aspect,
                            float viewRange,
                            const glm::vec3 &nodeMin,
                            const glm::vec3 &nodeMax) const noexcept;

    public:
        // Only the root may call this
        OctreeNode() noexcept = default;
        OctreeNode(const glm::vec3 &pos, const glm::vec3 &size) noexcept;

        bool isLeaf() const noexcept;
        size_t getOctan(const Asset &node) const noexcept;

        // TODO: This function potentially runs for ever
        //  This is because there is no limit on how small
        //  A cube can be, meaning that if I insert over 5
        //  Elements that occupy the same place in space
        //  The tree will try to subdivide infinitly
        void subdivide(Asset *node) noexcept;
        
        void insert(Asset *node) noexcept;
        void addHitboxAsset(Asset *asset) noexcept;
        void drawAsset(const Camera *camera) noexcept;

        bool intersectAABB(const glm::vec3 &pos1, const glm::vec3 &size1,
                           const glm::vec3 &pos2, const glm::vec3 &size2) const noexcept;

        // The container could be something other than a std::vector
        virtual void queryIntersection(const Asset &ast, std::vector<Asset *> &results) const noexcept;

        void querryView(const Camera *cam,
                        float fovY, float aspect, float viewRange,
                        std::vector<Asset *> &visible) const noexcept;

        // If any intersects
        virtual void queryIntersection(std::vector<Asset *> &results) const noexcept;
        virtual ~OctreeNode() = default;
    };

} // namespace sas