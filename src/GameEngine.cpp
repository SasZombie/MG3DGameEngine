#include "GameEngine.hpp"

sas::GameEngine::GameEngine() noexcept
    : window("Game Engine", winWidth, winHeight)
{
    sceneNodes = std::make_shared<SceneNode>();
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, std::shared_ptr<Asset> asset) noexcept
{
    root->addNode(asset);
    cullingOctree.insert(asset.get());
}

void sas::GameEngine::addSkybox(std::shared_ptr<Asset> asset) noexcept
{
    skybox = asset.get();
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, std::shared_ptr<Camera> camera) noexcept
{
    root->addNode(camera);
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, SceneSharedNode node) noexcept
{
    root->addNode(node);
}

sas::SceneSharedNode sas::GameEngine::getRoot() const noexcept
{
    return sceneNodes;
}

void sas::GameEngine::checkCollision() noexcept
{
    // std::vector<sas::Asset *> collidingObjects;
    // collisionOctree.queryIntersection(*CubeAsset3.get(), collidingObjects);

    for(auto& node : sceneNodes->components)
    {

    }
}

void sas::GameEngine::uppdate(const Camera *camera) noexcept
{
    // If no skybox, brench predictor will
    // Always evaluate this to false
    // So still fast
    if (skybox)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        skybox->uppdateWorldTransform({});
        skybox->uppdate(camera);
        
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    sceneNodes->uppdateWorldTransform({});
    sceneNodes->uppdate(camera);
}

Window *sas::GameEngine::getWindow() noexcept
{
    return &window;
}

void sas::GameEngine::mainLoop(const Camera* camera) noexcept
{
    window.clear();

    uppdate(camera);

    window.update();    
}
