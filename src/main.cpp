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

#include "Asset.hpp"
#include "OctreeNode.hpp"
#include "CollisionObjects.hpp"
#include "meshLoaderObj.hpp"
#include "texture.hpp"
// #include "shader.hpp"
// #include "window.hpp"

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
static void mouse_callback(GLFWwindow *window, int button, int action, int mods) noexcept;
static void processKeyboardInput(std::shared_ptr<sas::Asset> asset) noexcept;

float yOffset = 0.f;

float deltaTime = 0.f;
float yaw = -90.f, pitch = 0.f;

double lastX = 400, lastY = 300;

glm::vec3 keyPosition(322.f, -20.f, 404.f);

Window window("Game Engine", 1920, 1080);
std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{0.f, 0.f, 0.f});
sas::OctreeNode rootOctree({0, 0, 0}, {100, 100, 100});

bool showHitBoxes = false;

int main(int argc, char **argv)
{
    std::shared_ptr<sas::SceneNode> root = std::make_shared<sas::SceneNode>();
    float lastFrame = 0.f;

    Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
    Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

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

    Mesh fullCube = loadObj("Resources/Models/cube.obj");


    std::shared_ptr<sas::Asset> FullCubeAsset = std::make_shared<sas::Asset>(shader, fullCube, &window);

    std::shared_ptr<sas::Asset> SunAsset = std::make_shared<sas::Asset>(sunShader, sun, &window);
    std::shared_ptr<sas::Asset> SphereAsset = std::make_shared<sas::Asset>(shader, HitBox, &window);
    std::shared_ptr<sas::Asset> SkyBoxAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);
    std::shared_ptr<sas::Asset> CubeAsset = std::make_shared<sas::Asset>(shader, cube, &window);
    std::shared_ptr<sas::Asset> CubeAsset2 = std::make_shared<sas::Asset>(shader, cube, &window);
    std::shared_ptr<sas::Asset> CamerasKey = std::make_shared<sas::Asset>(shader, key, &window);
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
    std::shared_ptr<sas::Asset> KeyAsseyTEST = std::make_shared<sas::Asset>(shader, key, &window);

    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window.getWindow(), mouse_callback);

    glm::vec3 lightColor = glm::vec3(1.f);
    glm::vec3 lightPos = glm::vec3(10.f, 0.f, 0.f);

    KeyAsset1->scale({5.f, 5.f, 5.f});
    KeyAsset1->translate({60.f, 0.f, 0.f});
    KeyAsset1->rotate({0.f, 0.f, 90.f});

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

    KeyAsseyTEST->scale({25.f, 25.f, 5.f});
    KeyAsseyTEST->translate({40.f, 40.f, -30.f});
    KeyAsseyTEST->rotate({0.f, 0.f, 90.f});

    SunAsset->translate(lightPos);
    float scale = 1.f;
    SunAsset->scale({scale, scale, scale});

    
    float scaleSky = 100.f;
    SkyBoxAsset->translate({0.f, 0.f, 0.f});
    SkyBoxAsset->scale({scaleSky, scaleSky, scaleSky});
    SkyBoxAsset->rotate({0.f, 0.f, 0.f});

    root->addNode(camera);
    camera->addNode(CamerasKey);
    // CamerasKey->addNode(KeyAsset2);

    root->addNode(SkyBoxAsset);

    glEnable(GL_DEPTH_TEST);

    CubeAsset->translate({20, 0, 20});

    root->addNode(CubeAsset);
    CubeAsset2->translate({14, 0, 20});

    CubeAsset->addNode(CubeAsset2);

    root->addNode(KeyAsset1);

    // CamerasKey->scale({0.5f, 0.5f, 0.5f});
    CamerasKey->translate({0.5f, -0.4f, -2.5f});
    // CamerasKey->rotate({0.f, 0.f, 190.f});

    // KeyAsset2->scale({0.5f, 0.5f, 0.5f});
    KeyAsset2->translate({0.8f, -0.4f, -5.5f});
    // KeyAsset2->rotate({0.f, 0.f, 90.f});

    // CamerasKey->addNode();

    // CamerasKey->addNode();

    // rootOctree.insert(*KeyAsset.get());
    // rootOctree.insert(*CubeAsset.get());
    // rootOctree.insert(KeyAsset.get());
    // rootOctree.insert(CubeAsset.get());
    // rootOctree.insert(KeyAsset1.get());
    rootOctree.insert(CubeAsset2.get());
    rootOctree.insert(CamerasKey.get());
    // rootOctree.insert(KeyAsset2.get());

    rootOctree.addHitboxAsset(FullCubeAsset.get());

    float rotation = 0;

    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processKeyboardInput(CubeAsset);

        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        window.clear();

        glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        //TODO: Add support for these functions
        // When rendering SkyBox (Make a special field for it)
        // glDepthMask(GL_FALSE);
        // glDepthFunc(GL_LEQUAL);

        // SkyBoxAsset->draw(camera.get());

        // glDepthMask(GL_TRUE);
        // glDepthFunc(GL_LESS);

        rotation = rotation + 5 * deltaTime;

        if(rotation >= 360)
        {
            rotation = 0;
        }

        KeyAsset1->rotate({0.f, 0.f, rotation});

        // std::cout << "Hitbox camera = " << *camera << '\n'; 
        std::cout << "Hand Key world = " << CamerasKey->worldTransform; 
        std::cout << "Cube2 world = " << CubeAsset2->worldTransform ;
        // std::cout << "Cube1 world = " << CubeAsset->worldTransform ;

        // std::cout << "Hitbox \"Cube\" = " << CubeAsset->worldTransform << '\n'; 


        root->uppdate(camera.get());
        root->uppdateWorldTransform({});

        // FullCubeAsset->localTransform = CubeAsset->localTransform;
        // FullCubeAsset->worldTransform = CubeAsset->worldTransform;

        std::vector<sas::Asset* > collidingObjects;

        // rootOctree.queryIntersection(*CubeAsset2.get(), collidingObjects);
        rootOctree.queryIntersection(*CubeAsset2.get(), collidingObjects);

        if(showHitBoxes)
        {
            rootOctree.drawAsset(camera.get());
        }

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
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(glWindow, &xpos, &ypos);
    }
}

void processKeyboardInput(std::shared_ptr<sas::Asset> asset) noexcept
{
    float cameraSpeed = 30 * deltaTime;

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

    if(window.isPressed(GLFW_KEY_H))
    {
        showHitBoxes = !showHitBoxes;
    }
    if (window.isPressed(GLFW_KEY_SPACE))
    {
        asset->localTransform.position += glm::vec3{0.1f, 0, 0};
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