#include "GameEngine.hpp"
#include <utility>

sas::GameEngine::GameEngine() noexcept
    : window("Game Engine", winWidth, winHeight)
{
    camera = std::make_shared<Camera>(glm::vec3{0.f, 0.f, 0.f});
    // Preallocate some memory
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

void sas::GameEngine::addSceneNode(std::shared_ptr<Camera> ncamera, std::shared_ptr<Asset> asset) noexcept
{
    ncamera->addNode(asset);
    if (asset->hasCollisionObject())
    {
        collisionOctree.insert(asset.get());
    }
}

void sas::GameEngine::addSkybox(std::shared_ptr<Asset> asset) noexcept
{
    skybox = asset;
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, std::shared_ptr<Camera> ncamera) noexcept
{
    root->addNode(ncamera);
}

void sas::GameEngine::addSceneNode(SceneSharedNode root, SceneSharedNode node) noexcept
{
    root->addNode(node);
}

sas::SceneSharedNode sas::GameEngine::getRoot() const noexcept
{
    return sceneNodes;
}

std::shared_ptr<Camera> sas::GameEngine::getCamera() const noexcept
{
    return camera;
}

void sas::GameEngine::uppdate(const Camera *ncamera) noexcept
{
    // If no skybox, brench predictor will
    // Always evaluate this to false
    // So still fast
    if (skybox)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        skybox->uppdateWorldTransform({});
        skybox->uppdate(ncamera);
        // Skybox always visible
        skybox->draw(ncamera);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    collisionOctree.checkCollisions();

    sceneNodes->uppdateWorldTransform({});
    sceneNodes->uppdate(ncamera);

    cullingOctree.querryView(ncamera, drawingResults);

    for (const auto &elem : drawingResults)
    {
        elem->draw(ncamera);
    }

    drawingResults.clear();
}

void sas::GameEngine::saveScene(const std::filesystem::path &path) noexcept
{
    std::ofstream out(path);

    out << SerializeCodes::ROOT << '\n';
    sceneNodes->save(out);
    out << SerializeCodes::SKYBOX << '\n';
    skybox->save(out);
    out << SerializeCodes::ENDROOT;

    out.close();
}

void sas::GameEngine::loadScene(const std::filesystem::path &path) noexcept
{
    int cdepth = 0;
    std::ifstream in(path);
    SerializeCodes current;
    in >> current;

    if (current == SerializeCodes::ROOT)
    {
        in >> sceneNodes->localTransform;
        in >> sceneNodes->worldTransform;
    }
    SceneSharedNode currentTreeNode = sceneNodes;
    while (in >> current)
    {
        switch (current)
        {
        case SerializeCodes::SKYBOX:
        {

            int code;
            // Asset code skiped
            in >> code;

            std::string vert, frag, obj;
            std::optional<std::string> text = std::nullopt;

            in >> vert >> frag >> obj;

            int size;
            in >> size;

            if (size > 0)
            {
                text.emplace();
                in >> *text;
            }

            auto asset = addAsset(vert, frag, obj, text);
            in >> asset->localTransform;
            in >> asset->worldTransform;

            skybox = asset;

            currentTreeNode = asset;

            break;
        }

        case SerializeCodes::CAMERA:
            ++cdepth;
            in >> *camera;

            currentTreeNode->addNode(camera);

            currentTreeNode = camera;
            break;
        case SerializeCodes::ASSET:
        {
            std::string vert, frag, obj;
            std::optional<std::string> text = std::nullopt;

            in >> vert >> frag >> obj;

            int size;
            in >> size;

            if (size > 0)
            {
                text.emplace();
                in >> *text;
            }

            auto asset = addAsset(vert, frag, obj, text);
            in >> asset->localTransform;
            in >> asset->worldTransform;

            currentTreeNode->addNode(asset);

            ++cdepth;
            if (cdepth <= 1)
            {
                cullingOctree.insert(asset.get());
            }

            currentTreeNode = asset;

            break;
        }

        case SerializeCodes::END:
            if (auto parent = currentTreeNode->parent.lock())
            {
                --cdepth;
                currentTreeNode = parent;
            }
            break;
        case SerializeCodes::ROOT:
            std::cout << "ROOT\n";
            break;
        case SerializeCodes::ENDROOT:
            std::cout << "ENDROOT\n";
            break;
        default:
            assert(false && "Unreachable render Format case");
            std::unreachable();
            break;
        }
    }

    in.close();
}

[[nodiscard]] std::shared_ptr<sas::Asset> sas::GameEngine::addAsset(const std::string &vertex, const std::string &frag, const std::string &meshPath, const std::optional<std::string> &texturePath)
{
    auto shader = manager.loadShader(vertex, frag);
    std::shared_ptr<Mesh> mesh;

    if (texturePath)
    {
        auto tex = manager.loadTexture(*texturePath);
        mesh = manager.loadMesh(meshPath, tex);
    }
    else
    {
        mesh = manager.loadMesh(meshPath);
    }

    return manager.createAsset(shader, mesh, &window);
}

[[nodiscard]] std::shared_ptr<sas::Asset> sas::GameEngine::addAsset(const std::shared_ptr<Shader> &shader, const std::string &meshPath, const std::optional<std::string> &texturePath) noexcept
{
    std::shared_ptr<Mesh> mesh;

    if (texturePath)
    {
        auto tex = manager.loadTexture(*texturePath);
        mesh = manager.loadMesh(meshPath, tex);
    }
    else
    {
        mesh = manager.loadMesh(meshPath);
    }

    return manager.createAsset(shader, mesh, &window);
}
Window *sas::GameEngine::getWindow() noexcept
{
    return &window;
}

void sas::GameEngine::mainLoop(const Camera *namera) noexcept
{
    window.clear();

    uppdate(namera);

    window.update();
}
