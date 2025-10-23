#pragma once

#include <optional>
#include <functional>

#include "camera.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "mesh.hpp"
#include "Signal.hpp"
#include "CollisionObjects.hpp"

// Something that you can draw on the screen
namespace sas
{

    class Asset : public SceneNode
    {
        using Callback = std::function<void()>;
    public:
        int uniformShaderID;

    private:
        glm::mat4 ModelMatrix = glm::mat4(1.f);
        glm::mat4 ViewMatrix = glm::mat4(1.f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.f);
        glm::mat4 MVP = glm::mat4(1.f);

        Shader shader;
        std::optional<Mesh> mesh;
        CollisionObject* collisionObject = nullptr;
        Window *window;
        std::vector<Callback> callbacks;

        Signal<Asset&, Asset&> signals;
        
    public:
        Asset() = default;
        // Not owining window
        Asset(Window *window) noexcept;
        Asset(const Mesh &mesh, Window *window) noexcept;
        Asset(const Shader &shader, const Mesh &mesh, Window *window) noexcept;
        void addMesh(const Mesh &mesh) noexcept;
        void basicPVM(const Camera *camera) noexcept;
        void draw(const Camera *camera) noexcept;
        void drawAttachedToCamera(const Camera *camera) noexcept;

        void translate(const glm::vec3 &newPosition) noexcept;
        void scale(const glm::vec3 &newScaleVector) noexcept;
        void rotate(const glm::vec3 &axisVector) noexcept;
        void setShader(const Shader &shader) noexcept;
        void setShaderUniformID(int id) noexcept;

        void addCallback(Callback cb) noexcept;
        void onCollision(const Signal<Asset&, Asset&>::collsionCallback& cb) noexcept;
        void emit(Asset& other) noexcept;

        void uppdate(const Camera *camera) noexcept override;
        void uppdateAttachedToCamera(const Camera *camera) noexcept override;
        

        void addCollisionObject(CollisionObject *colObj) noexcept;
        bool hasCollisionObject() const noexcept;
        CollisionObject* getCollisionObject() const noexcept;       

        bool operator==(const Asset &other) const noexcept
        {
            return this == &other;
        } 

        ~Asset() noexcept = default;
    };
} // namespace sas
