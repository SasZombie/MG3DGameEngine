 #pragma once

#include <optional>
#include "camera.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "mesh.hpp"


//Something that you can draw on the screen
namespace sas
{

    class Asset : public SceneNode
    {
    public:
        float boundingRadius = 0.1f; //TODO: This is extremmmly temporary
        int uniformShaderID;
        
    private:
        glm::mat4 ModelMatrix = glm::mat4(1.f);;    
        glm::mat4 ViewMatrix = glm::mat4(1.f);;    
        glm::mat4 ProjectionMatrix = glm::mat4(1.f);;    
        glm::mat4 MVP = glm::mat4(1.f);;

        Shader shader;
        std::optional<Mesh> mesh;
        Window* window;

    public:
        //Not owining window
        Asset() = default;
        Asset(Window *window) noexcept;
        Asset(const Mesh& mesh, Window *window) noexcept;
        Asset(const Shader& shader, const Mesh& mesh, Window *window) noexcept;
        void addMesh(const Mesh& mesh) noexcept;
        void basicPVM(const Camera* camera) noexcept;
        void draw(const Camera *camera) noexcept;
        void translate(const glm::vec3& newPosition) noexcept;
        void scale(const glm::vec3& newScaleVector) noexcept;
        void rotate(const glm::vec3& axisVector) noexcept;
        void setShader(const Shader& shader) noexcept;
        void setShaderUniformID(int id) noexcept;

        
        void uppdate(const Camera* camera) noexcept override;

        ~Asset() noexcept = default;
    };
} // namespace sas
