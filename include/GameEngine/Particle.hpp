#pragma once
#include <memory>

#include "Asset.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop

namespace sas
{

    struct Particle
    {
        glm::vec3 position;
        glm::vec3 velocity;

        //Maybe add some randomness to these 2
        glm::vec4 color;  
        glm::vec3 scale;
        float life;
    };

    class ParticleSystem : public SceneNode
    {
    private:
        std::shared_ptr<Asset> particleAsset;
        std::vector<Particle> particles;

        size_t maxParticles;

    public:
        ParticleSystem(std::shared_ptr<Asset> asset, size_t nMaxParticles) noexcept;

        void emit(const glm::vec3 &position, const glm::vec3 &velocity, float life) noexcept;
        void uppdate(const Camera* camera) noexcept override;
        void draw(const Camera *camera) noexcept;
    };

} // namespace sas