#include <unordered_map>
#include <random>
#include <functional>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/freeglut.h>
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
#include "GameEngine.hpp"
// #include "shader.hpp"
// #include "window.hpp"

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
static void mouse_callback(GLFWwindow *window, int button, int action, int mods) noexcept;
static void processKeyboardInput() noexcept;

float yOffset = 0.f;

float deltaTime = 0.f;
float yaw = -90.f, pitch = 0.f;

double lastX = 400, lastY = 300;

glm::vec3 keyPosition(322.f, -20.f, 404.f);

constexpr size_t winWidth = 1920, winHeight = 1080;

std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{0.f, 0.f, 0.f});
bool showHitBoxes = true;

Window *window;
int main(int argc, char **argv)
{
    sas::GameEngine ge;
    window = ge.getWindow();

    std::shared_ptr<sas::SceneNode> root = ge.getRoot();

    float lastFrame = 0.f;

    // Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    // Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

    // // const auto &textures1 = getDefaultTexture("Resources/Textures/City.bmp");
    // const auto &textures2 = getDefaultTexture("Resources/Textures/test.bmp");
    // // const auto &textures3 = getDefaultTexture("Resources/Textures/gate_diffuse.bmp");
    // const auto &textures4 = getDefaultTexture("Resources/Textures/brickwall_4.bmp");
    // const auto &textures5 = getDefaultTexture("Resources/Textures/Skybox.bmp");

    

    // Mesh sun = loadObj("Resources/Models/sphere.obj");
    // Mesh cube = loadObj("Resources/Models/CubFata.obj", textures2);
    // Mesh key = loadObj("Resources/Models/21929_Key_v1.obj", textures2);
    // Mesh skyBox = loadObj("Resources/Models/caldare.obj", textures5);
    // Mesh HitBox = loadObj("Resources/Models/HitBox.obj", textures4);

    // Mesh fullCube = loadObj("Resources/Models/cube.obj");

    // std::shared_ptr<sas::Asset> FullCubeAsset = std::make_shared<sas::Asset>(shader, fullCube, window);

    // std::shared_ptr<sas::Asset> SunAsset = std::make_shared<sas::Asset>(sunShader, sun, window);
    // std::shared_ptr<sas::Asset> SphereAsset = std::make_shared<sas::Asset>(shader, HitBox, window);
    // std::shared_ptr<sas::Asset> SkyBoxAsset = std::make_shared<sas::Asset>(shader, skyBox, window);
    // std::shared_ptr<sas::Asset> CubeAsset = std::make_shared<sas::Asset>(shader, cube, window);
    // std::shared_ptr<sas::Asset> CubeAsset2 = std::make_shared<sas::Asset>(shader, cube, window);
    // std::shared_ptr<sas::Asset> CubeAsset3 = std::make_shared<sas::Asset>(shader, cube, window);
    // std::shared_ptr<sas::Asset> CamerasKey = std::make_shared<sas::Asset>(shader, key, window);
    // std::shared_ptr<sas::Asset> KeyAsset1 = std::make_shared<sas::Asset>(shader, key, window);

    sas::AssetManager manager;
    auto shader = manager.loadShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    auto sunShader = manager.loadShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

    auto goldenTexture = manager.loadTexture("Resources/Textures/test.bmp");
    auto skyBoxTexture = manager.loadTexture("Resources/Textures/Skybox.bmp");

    auto cubeMesh = manager.loadMesh("Resources/Models/CubFata.obj", goldenTexture);
    auto keyMesh = manager.loadMesh("Resources/Models/21929_Key_v1.obj", goldenTexture);
    auto skyBoxMesh = manager.loadMesh("Resources/Models/caldare.obj", skyBoxTexture);

    auto CubeAsset = manager.createAsset(shader, cubeMesh, window);
    auto CubeAsset2 = manager.createAsset(shader, cubeMesh, window);
    auto CubeAsset3 = manager.createAsset(shader, cubeMesh, window);


    auto KeyAsset = manager.createAsset(shader, keyMesh, window);
    auto KeyAsset1 = manager.createAsset(shader, keyMesh, window);
    auto CamerasKey = manager.createAsset(shader, keyMesh, window);
    
    auto SkyBoxAsset = manager.createAsset(shader, skyBoxMesh, window);


    float scaleMax = 1000.f;
    sas::CollisionObject *CubeCollisionObject = new sas::AABB;
    sas::CollisionObject *Cube2CollisonObject = new sas::AABB;
    sas::CollisionObject *Cube3CollisonObject = new sas::AABB;

    CubeAsset->addCollisionObject(CubeCollisionObject);
    CubeAsset2->addCollisionObject(Cube2CollisonObject);
    CubeAsset3->addCollisionObject(Cube3CollisonObject);


    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

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

    glEnable(GL_DEPTH_TEST);

    CubeAsset->translate({-5, 0, -5});
    ge.addSceneNode(root, CubeAsset);

    CubeAsset2->translate({20, 0, -5});
    ge.addSceneNode(root, CubeAsset2);

    CubeAsset3->translate({0, 0, -5});
    ge.addSceneNode(root, CubeAsset3);

    CamerasKey->translate({0.5f, -0.4f, -2.5f});
    ge.addSceneNode(root, KeyAsset1);

    float rotation = 0;
    int negative = 1;

    float viewRange = 1000.f;
    float fovRadians = glm::radians(360.f);

    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);


    KeyAsset1->addCallback([&window = KeyAsset1]()
                           {
                               window->localTransform.rotation.z += 5.f * sas::Globals::instance().getDeltaTime();

                               if (window->localTransform.rotation.z >= M_PI * 2.f)
                                   window->localTransform.rotation.z = 0.f; });


    CubeAsset3->addCallback([&window = CubeAsset3, &negative]()
                            {
                                float deltaX = negative * 2.f * sas::Globals::instance().getDeltaTime();

                                window->localTransform.position += glm::vec3{deltaX, 0.f, 0.f};
                            });

    CubeAsset3->onCollision([&negative](sas::Asset &self, sas::Asset &other)
                            {
                                negative = negative * -1;

                                float deltaX = negative * 50.f * deltaTime;

                                self.localTransform.position += glm::vec3{deltaX, 0.f, 0.f};
                            });


    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window->getWindow(), mouse_callback);

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sas::Globals::instance().setDeltaTime(deltaTime);

        processKeyboardInput();
        window->clear();

        glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glUniform3f(glGetUniformLocation(shader.get()->getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.get()->getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.get()->getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        ge.uppdate(camera.get());
        // std::vector<sas::Asset*> results;
        // cullingOctree.querryView(camera.get(), results);

        // std::cout << results.size() << '\n';


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