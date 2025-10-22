#pragma once

#include <vector>

#include "SceneNode.hpp"
#include "OctreeNode.hpp"
#include "window.hpp"

namespace sas
{
    static constexpr size_t winWidth = 1920, winHeight = 1080;

    class GameEngine
    {
    // private:
    public:
        Window window;
        
        //Culling trees always present
        OctreeNode cullingOctree;
        //Collision Trees not always present
        //But very lightweight if empty
        OctreeNode collisionOctree;
        SceneSharedNode sceneNodes;

        Asset* skybox;

    public:
        GameEngine() noexcept;

        void addSceneNode(SceneSharedNode root, std::shared_ptr<Asset> asset) noexcept;

        void addSkybox(std::shared_ptr<Asset> asset) noexcept;

        void addSceneNode(SceneSharedNode root, std::shared_ptr<Camera> camera) noexcept;

        void addSceneNode(SceneSharedNode root, SceneSharedNode node) noexcept;

        SceneSharedNode getRoot() const noexcept;

        void checkCollision() noexcept;

        void uppdate(const Camera* camera) noexcept;

        Window* getWindow() noexcept;

        void mainLoop(const Camera* camera) noexcept;

        ~GameEngine() noexcept = default;
    };
    
} // namespace sas
