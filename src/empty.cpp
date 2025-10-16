
void Camera::uppdate(const Camera *camera) noexcept
{   
    //Dont need the pointer to self
    (void)camera;
    //Camera does NOT use the transform. It is way more complex here
    uppdateWorldTransform(worldTransform);

    SceneNode::uppdateAttachedToCamera(camera);
}

void sas::SceneNode::uppdate(const Camera *camera) noexcept
{
    for (auto &component : components)
    {
        component->uppdate(camera);
    }
}

void sas::SceneNode::uppdateAttachedToCamera(const Camera *camera) noexcept
{
    for (auto &component : components)
    {
        component->uppdateAttachedToCamera(camera);
    }
}

void sas::SceneNode::uppdateWorldTransform(const Transform &parentWorldTransform) noexcept
{
    worldTransform = parentWorldTransform.combine(localTransform);
    for (auto &child : components)
        child->uppdateWorldTransform(worldTransform);
}
