#pragma once

#include <optional>

#include "mesh.hpp"
#include "window.hpp"
#include "camera.hpp"


//Something that you can draw on the screen
namespace sas
{

    class Asset : public SceneNode
    {
    public:
        glm::vec3 position;
        
    private:
        int uniformShaderID;
        glm::mat4 ModelMatrix = glm::mat4(1.f);;    
        glm::mat4 ViewMatrix = glm::mat4(1.f);;    
        glm::mat4 ProjectionMatrix = glm::mat4(1.f);;    
        glm::mat4 MVP = glm::mat4(1.f);;

        Shader shader;
        std::optional<Mesh> mesh;
        Window* window;

    public:
        //Not owining window
        Asset(Window *window) noexcept;
        Asset(const Mesh& mesh, Window *window) noexcept;
        Asset(const Shader& shader, const Mesh& mesh, Window *window) noexcept;
        void addMesh(const Mesh& mesh) noexcept;
        void basicPVM(const Camera* camera) noexcept;
        void draw() noexcept;
        void translate(const glm::vec3& newPosition) noexcept;
        void scale(const glm::vec3& newScaleVector) noexcept;
        void rotate(float degrees, const glm::vec3& axisVector) noexcept;
        void setShader(const Shader& shader) noexcept;
        void setShaderUniformID(int id) noexcept;
        
        void uppdate(const Camera* camera) noexcept override;

        ~Asset() noexcept = default;
    };
} // namespace sas
