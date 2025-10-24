#include "AssetManager.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "meshLoaderObj.hpp"

std::shared_ptr<Mesh> sas::AssetManager::loadMesh(const std::string &path) noexcept
{
    if (auto it = meshCache.find(path); it != meshCache.end())
    {
        if (auto existing = it->second.lock())
            return existing;
    }

    auto mesh = std::make_shared<Mesh>(loadObj(path));
    meshCache[path] = mesh;
    return mesh;
}

std::shared_ptr<Mesh> sas::AssetManager::loadMesh(const std::string &path, const std::shared_ptr<std::vector<Texture>> &tex) noexcept
{
    if (auto it = meshCache.find(path); it != meshCache.end())
    {
        if (auto existing = it->second.lock())
            return existing;
    }

    auto mesh = std::make_shared<Mesh>(loadObj(path, *tex.get()));
    meshCache[path] = mesh;
    return mesh;
}

std::shared_ptr<std::vector<Texture>> sas::AssetManager::loadTexture(const std::string &path) noexcept
{
    if (auto it = textureCache.find(path); it != textureCache.end())
    {
        if (auto existing = it->second.lock())
            return existing;
    }

    auto tex = std::make_shared<std::vector<Texture>>(getDefaultTexture(path.c_str()));
    textureCache[path] = tex;
    return tex;
}

std::shared_ptr<Shader> sas::AssetManager::loadShader(const std::string &vert, const std::string &frag)
{
    // Idealy we would separate vertex shaders and fragment shaders
    // But this will suffice for a demo
    const std::string key = vert + "|" + frag;

    if (auto it = shaderCache.find(key); it != shaderCache.end())
    {
        if (auto existing = it->second.lock())
            return existing;
    }

    auto shader = std::make_shared<Shader>(vert.c_str(), frag.c_str());
    shaderCache[key] = shader;
    return shader;
}

std::shared_ptr<sas::Asset> sas::AssetManager::createAsset(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Mesh> &mesh, Window *window) noexcept
{
    return std::make_shared<Asset>(*shader, *mesh, window);
}
