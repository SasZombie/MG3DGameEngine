#include "Particle.hpp"
#include "Globals.hpp"

sas::ParticleSystem::ParticleSystem(std::shared_ptr<Asset> asset, size_t nMaxParticles) noexcept
    : particleAsset(asset), maxParticles(nMaxParticles)
{
    particles.reserve(nMaxParticles);
}

void sas::ParticleSystem::emit(const glm::vec3 &nposition, const glm::vec3 &nvelocity, float life) noexcept
{
    if (particles.size() >= maxParticles)
        return;

    Particle p;
    p.position = nposition;
    p.velocity = nvelocity;
    p.life = life;
    p.color = glm::vec4(1.f);  
    p.scale = glm::vec3(0.1f); 
    particles.push_back(p);
}

void sas::ParticleSystem::uppdate(const Camera *camera) noexcept
{
    float dt = sas::Globals::instance().getDeltaTime();
    for (auto &p : particles)
    {
        p.life -= dt;
        if (p.life > 0.f)
        {
            p.position += p.velocity * dt;
        }
    }
    std::cout << "Appelat\n";

    draw(camera);

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle &p)
                       { return p.life <= 0.f; }),
        particles.end());
}

void sas::ParticleSystem::draw(const Camera *camera) noexcept
{
    for (auto &p : particles)
    {
        particleAsset->translate(p.position);
        particleAsset->scale(p.scale);
        particleAsset->uppdateWorldTransform({});
        particleAsset->draw(camera);

        // std::cout << particleAsset->worldTransform;
    }
}
