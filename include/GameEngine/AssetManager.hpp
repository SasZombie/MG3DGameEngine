#pragma once
#include <unordered_map>
#include <vector>

#include "Asset.hpp"

namespace sas
{

    class AssetManager
    {
    public:
        AssetManager() noexcept = default;
        
        std::shared_ptr<Mesh> loadMesh(const std::string &path) noexcept;
        std::shared_ptr<Mesh> loadMesh(const std::string &path, const std::shared_ptr<std::vector<Texture>>& tex) noexcept;
        std::shared_ptr<std::vector<Texture>> loadTexture(const std::string &path) noexcept;
        std::shared_ptr<Shader> loadShader(const std::string &vert, const std::string &frag);

        std::shared_ptr<Asset> createAsset(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Mesh> &mesh, Window *window) noexcept;
        
        ~AssetManager() noexcept = default;
    private:
        //cashé
        std::unordered_map<std::string, std::weak_ptr<Mesh>> meshCache;
        std::unordered_map<std::string, std::weak_ptr<std::vector<Texture>>> textureCache;
        std::unordered_map<std::string, std::weak_ptr<Shader>> shaderCache;
    };

} // namespace sas