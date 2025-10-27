#include <unordered_map>
#include <random>
#include <functional>
#include <vector>

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

    ge.loadScene("Aici.path");

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window->getWindow(), mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ge.saveScene("Aici.path");

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

int main2(int argc, char **argv)
{
    sas::GameEngine ge;

    window = ge.getWindow();
    camera = ge.getCamera();

    std::shared_ptr<sas::SceneNode> root = ge.getRoot();

    float lastFrame = 0.f;

    sas::AssetManager manager;
    auto shader = manager.loadShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    auto sunShader = manager.loadShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

    auto CubeAsset = ge.addAsset(shader, "Resources/Models/CubFata.obj", "Resources/Textures/test.bmp");
    auto CubeAsset2 = ge.addAsset(shader, "Resources/Models/CubFata.obj", "Resources/Textures/test.bmp");
    auto CubeAsset3 = ge.addAsset(shader, "Resources/Models/CubFata.obj", "Resources/Textures/test.bmp");

    auto KeyAsset = ge.addAsset(shader, "Resources/Models/21929_Key_v1.obj", "Resources/Textures/test.bmp");
    auto KeyAsset1 = ge.addAsset(shader, "Resources/Models/21929_Key_v1.obj", "Resources/Textures/test.bmp");
    auto CamerasKey = ge.addAsset(shader, "Resources/Models/21929_Key_v1.obj", "Resources/Textures/test.bmp");

    auto SkyBoxAsset = ge.addAsset(shader, "Resources/Models/caldare.obj", "Resources/Textures/Skybox.bmp");

    float scaleMax = 1000.f;
    sas::CollisionObject *CubeCollisionObject = new sas::AABB;
    sas::CollisionObject *Cube2CollisonObject = new sas::AABB;
    sas::CollisionObject *Cube3CollisonObject = new sas::AABB;

    CubeAsset->addCollisionObject(CubeCollisionObject);
    CubeAsset2->addCollisionObject(Cube2CollisonObject);
    CubeAsset3->addCollisionObject(Cube3CollisonObject);

    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

    KeyAsset1->name = "Key";
    KeyAsset1->scale({5.f, 5.f, 5.f});
    KeyAsset1->translate({60.f, 0.f, 0.f});
    KeyAsset1->rotate({0.f, 0.f, 90.f});

    float scaleSky = 100.f;
    SkyBoxAsset->translate({0.f, 0.f, 0.f});
    SkyBoxAsset->scale({scaleSky, scaleSky, scaleSky});
    SkyBoxAsset->rotate({0.f, 0.f, 0.f});

    ge.addSceneNode(root, camera);

    ge.addSceneNode(camera, CamerasKey);

    ge.addSkybox(SkyBoxAsset);

    CubeAsset->translate({-5, 0, -5});
    ge.addSceneNode(root, CubeAsset);

    CubeAsset2->translate({20, 0, -5});
    ge.addSceneNode(root, CubeAsset2);

    CubeAsset3->translate({0, 0, -5});
    ge.addSceneNode(root, CubeAsset3);

    CamerasKey->translate({0.5f, -0.4f, -2.5f});
    ge.addSceneNode(root, KeyAsset1);

    CubeAsset3->addNode(std::make_shared<sas::UI>(window));

    int negative = 1;

    float rotation = 0;

    KeyAsset1->addCallback("Scripts/KeyAssetScript.src");

    CubeAsset3->addCallback([&negative](sas::Asset &self)
                            {
                                float deltaX = negative * 2.f * sas::Globals::instance().getDeltaTime();

                                self.localTransform.position += glm::vec3{deltaX, 0.f, 0.f}; });

    CubeAsset3->onCollision([&negative](sas::Asset &self, sas::Asset &other)
                            {
                                negative = negative * -1;

                                float deltaX = negative * 50.f * deltaTime;

                                self.localTransform.position += glm::vec3{deltaX, 0.f, 0.f}; });

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window->getWindow(), mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ge.saveScene("Aici.path");

    return 1;

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sas::Globals::instance().setDeltaTime(deltaTime);

        processKeyboardInput();
        window->clear();

        // Why are these here:
        // I realized later in the development
        // That certain assets are drawn differently,
        // That a skybox should enable some gl stuff
        // The light sources should enable these 3
        // And so on. This implies the usage of another
        // Specialization of Asset such as Iluminating Asset
        // SkyBox assets and so on. I could encapsulate
        // These as well, but I am not sure if it would bring
        // Anything of value to the scope of this project
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