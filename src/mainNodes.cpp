#include <unordered_map>
#include <random>
#include <functional>
#include <vector>

#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "meshLoaderObj.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/freeglut.h>
#include <SFML/Audio.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include "UI.hpp"
#include "SceneNode.hpp"
#include "Asset.hpp"
#include "UseTexture.hpp"

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
static void mouse_callback(GLFWwindow *window, int button, int action, int mods) noexcept;
static void processKeyboardInput() noexcept;

float yOffset = 0.f;

float deltaTime = 0.f;
float yaw = -90.f, pitch = 0.f;

double lastX = 400, lastY = 300;

glm::vec3 keyPosition(322.f, -20.f, 404.f);

Window window("Game Engine", 1920, 1080);
std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{0.f, 0.f, 0.f});

int main(int argc, char **argv)
{
    std::shared_ptr<sas::SceneNode> root = std::make_shared<sas::SceneNode>();
    float lastFrame = 0.f;

    Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

    // const auto &textures1 = getDefaultTexture("Resources/Textures/City.bmp");
    const auto &textures2 = getDefaultTexture("Resources/Textures/test.bmp");
    // const auto &textures3 = getDefaultTexture("Resources/Textures/gate_diffuse.bmp");
    // const auto &textures4 = getDefaultTexture("Resources/Textures/brickwall_4.bmp");
    const auto &textures5 = getDefaultTexture("Resources/Textures/Skybox.bmp");

    Mesh sun = loadObj("Resources/Models/sphere.obj");
    Mesh key = loadObj("Resources/Models/21929_Key_v1.obj", textures2);
    Mesh skyBox = loadObj("Resources/Models/caldare.obj", textures5);

    std::shared_ptr<sas::Asset> SunAsset = std::make_shared<sas::Asset>(sunShader, sun, &window);
    std::shared_ptr<sas::Asset> SkyBoxAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);
    std::shared_ptr<sas::Asset> KeyAsset = std::make_shared<sas::Asset>(shader, key, &window);

    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window.getWindow(), mouse_callback);

    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

    KeyAsset->rotate({0.f, 0.f, 90.f});

    SunAsset->translate(lightPos);
    float scale = 1.f;
    SunAsset->scale({scale, scale, scale});

    SkyBoxAsset->translate({0.f, 0.f, 0.f});

    float scaleSky = 100.f;
    SkyBoxAsset->scale({scaleSky, scaleSky, scaleSky});
    SkyBoxAsset->rotate({0.f, 0.f, 0.f});

    root->addNode(SunAsset);
    root->addNode(KeyAsset);
    root->addNode(SkyBoxAsset);

   	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processKeyboardInput();

        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        window.clear();

        root->uppdate(camera.get());    

        glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        window.update();
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
    (void)glWindow;
    (void)action;
    (void)mods;
}

void processKeyboardInput() noexcept
{
    float cameraSpeed = 60 * deltaTime;

    // if (window.isPressed(GLFW_KEY_ESCAPE) && canPressKey)
    // {
    // 	canPressKey = false;
    // 	glfwGetCursorPos(window.getWindow(), &lastX, &lastY);
    // 	currentStateOfGame = states.at(currentStateOfGame);
    // 	if (ui.getVolState())
    // 		music.play();
    // 	else
    // 		music.stop();
    // }

    if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
        cameraSpeed = cameraSpeed * 2;

    glm::vec3 direction(0.f);
    if (window.isPressed(GLFW_KEY_W))
        direction += camera->getCameraViewDirection();
    if (window.isPressed(GLFW_KEY_S))
        direction -= camera->getCameraViewDirection();
    if (window.isPressed(GLFW_KEY_A))
        direction -= camera->getCameraRight();
    if (window.isPressed(GLFW_KEY_D))
        direction += camera->getCameraRight();

    direction.y = 0.f;

    if (glm::length(direction) > 0.f)
    {
        direction = glm::normalize(direction);
        camera->move(direction * cameraSpeed);
    }

    if (window.isPressed(GLFW_KEY_SPACE))
    {
        // jumping = true;
        // camera->setJumping(true);
    }

    static constexpr float zMin = -490.f, zMax = 490.f, xMin = -490.f, xMax = 490.f;
    static constexpr float szMin = -334.f;

    // if (!openGate)
    // {
    // 	if (camera->getCameraPosition().z < szMin)
    // 		camera->setCameraZ(szMin);
    // }
    // else
    // {
    // 	if (camera->getCameraPosition().z < zMin)
    // 		camera->setCameraZ(zMin);
    // }

    // if (camera->getCameraPosition().z > zMax)
    // {
    // 	camera->setCameraZ(zMax);
    // }
    // if (camera->getCameraPosition().x < xMin)
    // {
    // 	camera->setCameraX(xMin);
    // }
    // if (camera->getCameraPosition().x > xMax)
    // {
    // 	camera->setCameraX(xMax);
    // }
}