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
#include <glm/gtx/norm.hpp>

#pragma GCC diagnostic pop

#include "UI.hpp"
#include "SceneNode.hpp"
#include "Asset.hpp"
#include "OctreeNode.hpp"

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
    Shader transparentShader("Shaders/transparent_vertex_shader.glsl", "Shaders/transparent_fragment_shader.glsl");

    // const auto &textures1 = getDefaultTexture("Resources/Textures/City.bmp");
    const auto &textures2 = getDefaultTexture("Resources/Textures/test.bmp");
    // const auto &textures3 = getDefaultTexture("Resources/Textures/gate_diffuse.bmp");
    const auto &textures4 = getDefaultTexture("Resources/Textures/brickwall_4.bmp");
    const auto &textures5 = getDefaultTexture("Resources/Textures/Skybox.bmp");

    Mesh sun = loadObj("Resources/Models/sphere.obj");
    Mesh cube = loadObj("Resources/Models/CubFata.obj", textures2);
    Mesh key = loadObj("Resources/Models/21929_Key_v1.obj", textures2);
    Mesh skyBox = loadObj("Resources/Models/caldare.obj", textures5);
    Mesh HitBox = loadObj("Resources/Models/HitBox.obj", textures4);

    std::shared_ptr<sas::Asset> SunAsset = std::make_shared<sas::Asset>(sunShader, sun, &window);
    std::shared_ptr<sas::Asset> SphereAsset = std::make_shared<sas::Asset>(shader, HitBox, &window);
    std::shared_ptr<sas::Asset> SkyBoxAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);
    std::shared_ptr<sas::Asset> CubeAsset = std::make_shared<sas::Asset>(shader, cube, &window);
    std::shared_ptr<sas::Asset> KeyAsset = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset1 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset2 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset3 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset4 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset5 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset6 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset11 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset22 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset33 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset44 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset55 = std::make_shared<sas::Asset>(shader, key, &window);
    std::shared_ptr<sas::Asset> KeyAsset66 = std::make_shared<sas::Asset>(shader, key, &window);

    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window.getWindow(), mouse_callback);

    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

    KeyAsset->scale({5.f, 5.f, 5.f});
    KeyAsset->translate({40.f, 0.f, 0.f});
    KeyAsset->rotate({0.f, 0.f, 90.f});

    KeyAsset1->scale({5.f, 5.f, 5.f});
    KeyAsset1->translate({60.f, 0.f, 0.f});
    KeyAsset1->rotate({0.f, 0.f, 90.f});

    KeyAsset2->scale({5.f, 5.f, 5.f});
    KeyAsset2->translate({80.f, 0.f, 0.f});
    KeyAsset2->rotate({0.f, 0.f, 90.f});

    KeyAsset3->scale({5.f, 5.f, 5.f});
    KeyAsset3->translate({20.f, 0.f, 0.f});
    KeyAsset3->rotate({0.f, 0.f, 90.f});

    KeyAsset4->scale({5.f, 5.f, 5.f});
    KeyAsset4->translate({0.f, 0.f, 0.f});
    KeyAsset4->rotate({0.f, 0.f, 90.f});

    KeyAsset5->scale({5.f, 5.f, 5.f});
    KeyAsset5->translate({-20.f, 0.f, 0.f});
    KeyAsset5->rotate({0.f, 0.f, 90.f});

    KeyAsset6->scale({5.f, 5.f, 5.f});
    KeyAsset6->translate({-40.f, 0.f, 0.f});
    KeyAsset6->rotate({0.f, 0.f, 90.f});

    KeyAsset11->scale({5.f, 5.f, 5.f});
    KeyAsset11->translate({-60.f, 0.f, 0.f});
    KeyAsset11->rotate({0.f, 0.f, 90.f});

    KeyAsset22->scale({5.f, 5.f, 5.f});
    KeyAsset22->translate({-80.f, 0.f, 0.f});
    KeyAsset22->rotate({0.f, 0.f, 90.f});

    KeyAsset33->scale({5.f, 5.f, 5.f});
    KeyAsset33->translate({-100.f, 0.f, 0.f});
    KeyAsset33->rotate({0.f, 0.f, 90.f});

    KeyAsset44->scale({5.f, 5.f, 5.f});
    KeyAsset44->translate({100.f, 0.f, 0.f});
    KeyAsset44->rotate({0.f, 0.f, 90.f});

    KeyAsset55->scale({5.f, 5.f, 5.f});
    KeyAsset55->translate({40.f, 0.f, 30.f});
    KeyAsset55->rotate({0.f, 0.f, 90.f});

    KeyAsset66->scale({5.f, 5.f, 5.f});
    KeyAsset66->translate({40.f, 0.f, -30.f});
    KeyAsset66->rotate({0.f, 0.f, 90.f});

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

    float minMax = 100.f;

    CubeAsset->translate({0, 0, 0});
    CubeAsset->scale({100.f, 100.f, 100.f});

    // sas::OctreeNode rootOctree({minMax, minMax, minMax}, {minMax, minMax, minMax}, *CubeAsset.get());

    // rootOctree.insert(SunAsset.get(), SunAsset.get()->transform.position);
    // rootOctree.insert(KeyAsset1.get(), KeyAsset1.get()->transform.position);
    // rootOctree.insert(KeyAsset2.get(), KeyAsset2.get()->transform.position);
    // rootOctree.insert(KeyAsset3.get(), KeyAsset3.get()->transform.position);
    // rootOctree.insert(KeyAsset4.get(), KeyAsset4.get()->transform.position);
    // rootOctree.insert(KeyAsset5.get(), KeyAsset5.get()->transform.position);
    // rootOctree.insert(KeyAsset6.get(), KeyAsset6.get()->transform.position);
    // rootOctree.insert(KeyAsset1.get(), KeyAsset11.get()->transform.position);
    // rootOctree.insert(KeyAsset2.get(), KeyAsset22.get()->transform.position);
    // rootOctree.insert(KeyAsset3.get(), KeyAsset33.get()->transform.position);
    // rootOctree.insert(KeyAsset4.get(), KeyAsset44.get()->transform.position);
    // rootOctree.insert(KeyAsset5.get(), KeyAsset55.get()->transform.position);
    // rootOctree.insert(KeyAsset6.get(), KeyAsset66.get()->transform.position);
    // rootOctree.insert(KeyAsset.get(), KeyAsset.get()->transform.position);

    glEnable(GL_DEPTH_TEST);

    // SphereAsset->scale({camera->rad, camera->rad, camera->rad});
    SphereAsset->translate({0.f, 0.f, 0.f});

    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processKeyboardInput();

        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        window.clear();

        glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        SkyBoxAsset->draw(camera.get());

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        SunAsset->draw(camera.get());
        KeyAsset->draw(camera.get());
        KeyAsset1->draw(camera.get());
        KeyAsset2->draw(camera.get());
        KeyAsset3->draw(camera.get());
        KeyAsset4->draw(camera.get());
        KeyAsset5->draw(camera.get());
        KeyAsset6->draw(camera.get());
        KeyAsset11->draw(camera.get());
        KeyAsset22->draw(camera.get());
        KeyAsset33->draw(camera.get());
        KeyAsset44->draw(camera.get());
        KeyAsset55->draw(camera.get());
        KeyAsset66->draw(camera.get());
        
        // SphereAsset->translate(camera->getCameraPosition());
        
        SphereAsset->draw(camera.get());
        
        std::vector<sas::Asset *> collidingObjects;
        // sas::queryCollisions(rootOctree, Sphere{camera->getCameraPosition(), camera->rad}, collidingObjects);


        if (!collidingObjects.empty())
        {
            std::cout << "Camera is colliding with " << collidingObjects.size() << " objects!\n";
        }
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