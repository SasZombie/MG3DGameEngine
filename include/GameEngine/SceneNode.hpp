#pragma once

#include <memory>
#include <vector>
#include "Transform.hpp"

struct Camera;

namespace sas
{
    struct SceneNode : public std::enable_shared_from_this<SceneNode>
    {

        using SceneSharedNode = std::shared_ptr<SceneNode>; 
        
        std::weak_ptr<SceneNode> parent;
        Transform localTransform;
        Transform worldTransform;
        std::vector<SceneSharedNode> components;
        
        //Used only by a root node
        SceneNode() noexcept = default;
        SceneNode(const glm::vec3& position) noexcept;

        //99.9 children use this
        SceneNode(SceneSharedNode nparent) noexcept;
        
        void addNode(SceneSharedNode child) noexcept;

        virtual void uppdate(const Camera* camera) noexcept;
        virtual void uppdateAttachedToCamera(const Camera* camera) noexcept;

        void uppdateWorldTransform(const Transform& parentWorldTransform) noexcept;

        virtual ~SceneNode() = default;
    };   
} // namespace sas
    
    /*
    int main()
    {
        Window window(plm);
        
        Scene s1();

    s1.addNode(Camera);



}

*/