#include <unordered_map>
#include <random>
#include <functional>
#include <vector>
#include <random>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <GLFW/glfw3.h>

#pragma GCC diagnostic pop

#include "AssetManager.hpp"
#include "OctreeNode.hpp"
#include "CollisionObjects.hpp"
#include "meshLoaderObj.hpp"
#include "Globals.hpp"
#include "texture.hpp"
#include "UI.hpp"
#include "Particle.hpp"
#include "GameEngine.hpp"

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
static void mouse_callback(GLFWwindow *window, int button, int action, int mods) noexcept;
static void processKeyboardInput() noexcept;

float yOffset = 0.f;

float deltaTime = 0.f;
float yaw = -90.f, pitch = 0.f;

double lastX = 400, lastY = 300;

constexpr size_t winWidth = 1920, winHeight = 1080;

std::shared_ptr<Camera> camera;
bool showHitBoxes = true;

Window *window;

int main(int argc, char **argv)
{
    sas::GameEngine ge;

    window = ge.getWindow();
    camera = ge.getCamera();
    float lastFrame = 0.f;

    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

    sas::AssetManager manager;

    auto shader = manager.loadShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    auto sunShader = manager.loadShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

    ge.loadScene("Scene.path");

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window->getWindow(), mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sas::Globals::instance().setDeltaTime(deltaTime);

        processKeyboardInput();
        window->clear();

        glUniform3f(glGetUniformLocation(shader.get()->getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.get()->getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.get()->getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        ge.uppdate(camera.get());

        window->update();
    }
}

void mouse_callback(GLFWwindow *glWindow, double xpos, double ypos) noexcept
{

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.15f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    if (yaw > 360)
        yaw = yaw - 360;
    if (yaw < 0)
        yaw = yaw + 360;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->setCameraViewDirection(glm::normalize(front));
}

void mouse_callback(GLFWwindow *glWindow, int button, int action, int mods) noexcept
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(glWindow, &xpos, &ypos);
    }
}

void processKeyboardInput() noexcept
{
    float cameraSpeed = 30 * sas::Globals::instance().getDeltaTime();

    if (window->isPressed(GLFW_KEY_LEFT_SHIFT))
        cameraSpeed = cameraSpeed * 2;

    glm::vec3 direction(0.f);
    if (window->isPressed(GLFW_KEY_W))
        direction += camera->getCameraViewDirection();
    if (window->isPressed(GLFW_KEY_S))
        direction -= camera->getCameraViewDirection();
    if (window->isPressed(GLFW_KEY_A))
        direction -= camera->getCameraRight();
    if (window->isPressed(GLFW_KEY_D))
        direction += camera->getCameraRight();

    direction.y = 0.f;

    if (glm::length(direction) > 0.f)
    {
        direction = glm::normalize(direction);
        camera->move(direction * cameraSpeed);
    }

    if (window->isPressed(GLFW_KEY_H))
    {
        showHitBoxes = !showHitBoxes;
    }
}