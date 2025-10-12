#pragma once

#include <memory>
#include <vector>

struct SceneNode : public std::enable_shared_from_this<SceneNode>
{
    using SceneSharedNode = std::shared_ptr<SceneNode>;

    std::weak_ptr<SceneNode> parent;
    std::vector<std::shared_ptr<SceneNode>> components;

    SceneNode(SceneSharedNode nparent) noexcept
        : parent(std::move(nparent))
    {

    }

    void load()
    {
        
    }
    
    int run()
    {

    };
};


/*
int main()
{
    Window window(plm);

    Scene s1();

    s1.addNode(Camera);



}

*/