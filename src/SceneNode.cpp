#include "SceneNode.hpp"

sas::SceneNode::SceneNode(SceneSharedNode nparent) noexcept
    : parent(std::move(nparent))
{   
}

void sas::SceneNode::addNode(SceneSharedNode child) noexcept
{
    child->parent = shared_from_this();
    components.push_back(std::move(child));
}

void sas::SceneNode::uppdate(const Camera* camera) noexcept
{
    for(auto& component : components)
    {
        component->uppdate(camera);
    }
}

