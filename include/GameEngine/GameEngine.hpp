#pragma once

#include <vector>
#include <filesystem>
#include "SceneNode.hpp"
#include "OctreeNode.hpp"
#include "window.hpp"
#include "UI.hpp"
#include "AssetManager.hpp"

namespace sas
{
    static constexpr std::size_t winWidth = 1920, winHeight = 1080;

    class GameEngine
    {
    private:
        ImGuiIO io;

        Window window;
        std::shared_ptr<Camera> camera;
        // Culling trees always present
        OctreeNode cullingOctree;
        // Collision Trees not always present
        // But very lightweight if empty
        OctreeNode collisionOctree;
        SceneSharedNode sceneNodes;

        std::shared_ptr<Asset> skybox;
        std::vector<Asset *> drawingResults;


        sas::AssetManager manager;

    public:
        GameEngine() noexcept;

        // Skybox is special Asset, no root
        void addSkybox(std::shared_ptr<Asset> asset) noexcept;

        // Non assets go here(UI, timers(?), so on)
        void addSceneNode(SceneSharedNode root, SceneSharedNode node) noexcept;
        // Root to asset
        void addSceneNode(SceneSharedNode root, std::shared_ptr<Asset> asset) noexcept;
        // Attaching camera is special
        void addSceneNode(SceneSharedNode root, std::shared_ptr<Camera> camera) noexcept;
        // Things attached to cameras are not added in a culling Tree
        void addSceneNode(std::shared_ptr<Camera> camera, std::shared_ptr<Asset> asset) noexcept;

        [[nodiscard]] std::shared_ptr<Asset> addAsset(const std::string &vertex, const std::string &frag, const std::string &meshPath, const std::optional<std::string> &texturePath = std::nullopt);
        [[nodiscard]] std::shared_ptr<Asset> addAsset(const std::shared_ptr<Shader> &shader, const std::string &meshPath, const std::optional<std::string> &texturePath = std::nullopt) noexcept;

        void uppdate(const Camera *camera) noexcept;

        SceneSharedNode getRoot() const noexcept;
        Window *getWindow() noexcept;
        std::shared_ptr<Camera> getCamera() const noexcept;

        // TODO: this requires a lot more work
        // Not sure it will be done
        void mainLoop(const Camera *camera) noexcept;

        void saveScene(const std::filesystem::path& path) noexcept;

        void loadScene(const std::filesystem::path &path) noexcept;

        ~GameEngine() noexcept = default;
    };

} // namespace sas
