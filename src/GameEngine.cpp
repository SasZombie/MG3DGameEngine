#include "GameEngine.hpp"

sas::GameEngine::GameEngine() noexcept
    : window("Game Engine", winWidth, winHeight)
{
    //Preallocate some memory 
    drawingResults.reserve(100);
    sceneNodes = std::make_shared<SceneNode>();

    collisionOctree = {{0.f, 0.f, 0.f}, {100.f, 100.f, 100.f}};
    cullingOctree = {{0.f, 0.f, 0.f}, {100.f, 100.f, 100.f}};
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, std::shared_ptr<Asset> asset) noexcept
{
    root->addNode(asset);
    cullingOctree.insert(asset.get());
    if (asset->hasCollisionObject())
    {
        collisionOctree.insert(asset.get());
    }
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
        //Skybox always visible
        skybox->draw(camera);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    collisionOctree.checkCollisions();

    sceneNodes->uppdateWorldTransform({});
    sceneNodes->uppdate(camera);

    cullingOctree.querryView(camera, drawingResults);

    for (const auto &elem : drawingResults)
    {
        elem->draw(camera);
    }

    drawingResults.clear();
}

Window *sas::GameEngine::getWindow() noexcept
{
    return &window;
}

void sas::GameEngine::mainLoop(const Camera *camera) noexcept
{
    window.clear();

    uppdate(camera);

    window.update();
}
