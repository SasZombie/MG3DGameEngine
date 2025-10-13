#pragma once

#include <memory>
#include <vector>

struct Camera;

namespace sas
{
    struct SceneNode : public std::enable_shared_from_this<SceneNode>
    {

        using SceneSharedNode = std::shared_ptr<SceneNode>;        
        std::weak_ptr<SceneNode> parent;
        std::vector<SceneSharedNode> components;
        
        //Used only by a root node
        SceneNode() noexcept = default;
        SceneNode(SceneSharedNode nparent) noexcept;
        
        void addNode(SceneSharedNode child) noexcept;

        void load()
        {
            
        }
        
        int run()
        {
            return 1;
        };

        virtual void uppdate(const Camera* camera) noexcept;

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