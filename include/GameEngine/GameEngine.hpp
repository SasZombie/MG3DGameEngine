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

        std::vector<Asset*> drawingResults;

    public:
        GameEngine() noexcept;

        //Skybox is special Asset, no root
        void addSkybox(std::shared_ptr<Asset> asset) noexcept;

        //Non assets go here(UI, timers(?), so on)
        void addSceneNode(SceneSharedNode root, SceneSharedNode node) noexcept;
        //Root to asset
        void addSceneNode(SceneSharedNode root, std::shared_ptr<Asset> asset) noexcept;
        //Attaching camera is special
        void addSceneNode(SceneSharedNode root, std::shared_ptr<Camera> camera) noexcept;
        //Things attached to cameras are not added in a culling Tree
        void addSceneNode(std::shared_ptr<Camera> camera, std::shared_ptr<Asset> asset) noexcept;


        
        void uppdate(const Camera* camera) noexcept;
        
        SceneSharedNode getRoot() const noexcept;
        Window* getWindow() noexcept;

        //TODO
        void mainLoop(const Camera* camera) noexcept;

        ~GameEngine() noexcept = default;
    };
    
} // namespace sas
