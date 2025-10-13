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

#define works false

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
static void mouse_callback(GLFWwindow *window, int button, int action, int mods) noexcept;

#if works

static bool firstObjective(sas::UI &ui, int nrOfEnemies) noexcept;
static bool seccondObjective(sas::UI &ui, int nrOfEnemies) noexcept;
static bool thirdObjective(sas::UI &ui, int nrOfEnemies) noexcept;

static float sqaredDistance(float x1, float x2, float y1, float y2) noexcept;

static void init() noexcept;

static void processKeyboardInput(sas::UI &ui, sf::Music &music, const std::unordered_map<sas::GameState, sas::GameState> &states) noexcept;
#endif

bool goldAppear = true;
bool canPressKey = true;

bool jumping = false;
bool keyHand = false;
bool spawnKey = true;
bool openGate = false;

int currentObjective = 0;

sas::GameState currentStateOfGame = sas::GameState::MainGame;

float yOffset = 0.f;

float deltaTime = 0.f;
float yaw = -90.f, pitch = 0.f;

double lastX = 400, lastY = 300;

glm::vec3 keyPosition(322.f, -20.f, 404.f);

Window window("Game Engine", 1920, 1080);
std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{-47.f, -10, 490.f});

int main(int argc, char **argv)
{
	std::shared_ptr<sas::SceneNode> root = std::make_shared<sas::SceneNode>();

	root->addNode(camera);

	float lastFrame = 0.f;

	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

	glm::vec3 lightColor = glm::vec3(1.f);
	glm::vec3 lightPos = glm::vec3(0.f, 0.f, 0.f);

	const auto& textures1 = getDefaultTexture("Resources/Textures/City.bmp");
	const auto& textures2 = getDefaultTexture("Resources/Textures/test.bmp");
	// const auto& textures3 = getDefaultTexture("Resources/Textures/gate_diffuse.bmp");
	// const auto& textures4 = getDefaultTexture("Resources/Textures/brickwall_4.bmp");
	const auto& textures5 = getDefaultTexture("Resources/Textures/Skybox.bmp");
	// size_t tex = loadBMP("Resources/Textures/City.bmp");

		// std::vector<Texture> textures;
	// textures.push_back(Texture());
	// textures[0].id = tex;
	// textures[0].type = "texture_diffuse";

	Mesh sun = loadObj("Resources/Models/sphere.obj");
	Mesh caldare = loadObj("Resources/Models/Crate.obj", textures1);
	Mesh key = loadObj("Resources/Models/21929_Key_v1.obj", textures2);
	// Mesh goldBars = loadObj("Resources/Models/gold_bar.obj", textures2);
	// Mesh gate = loadObj("Resources/Models/gate_obj.obj", textures3);
	// Mesh wall = loadObj("Resources/Models/wall.obj", textures4);
	Mesh skyBox = loadObj("Resources/Models/caldare.obj", textures5);

	const auto sunAsset = std::make_shared<sas::Asset>(sunShader, sun, &window);
	sunAsset->basicPVM(camera.get());
	sunAsset->translate(lightPos);
	float scaleFactor = 50;
	sunAsset->scale({scaleFactor, scaleFactor, scaleFactor});	


	float boxRotation = 0.f;

	const auto skyBoxAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);

	skyBoxAsset->basicPVM(camera.get());
	skyBoxAsset->translate(glm::vec3(0.f, -50.f, 0.f));
	float scaleFactorBox = 1000.f;
	skyBoxAsset->scale(glm::vec3(scaleFactorBox, scaleFactorBox, scaleFactorBox));
	skyBoxAsset->rotate(boxRotation, glm::vec3{0.f, 1.f, 0.f});

	
	const auto keyAsset = std::make_shared<sas::Asset>(shader, key, &window);
	
	keyAsset->basicPVM(camera.get());
	keyAsset->translate(keyPosition);
	keyAsset->scale(glm::vec3(5.f, 5.f, 5.f));
	keyAsset->rotate(90.f, glm::vec3{0.f, 1.f, 0.f});
	keyAsset->rotate(300.f, glm::vec3{1.f, 0.f, 0.f});
	

	root->addNode(sunAsset);

	root->addNode(keyAsset);



	// root->addNode(skyBoxAsset);


	// const auto caldareAsset = std::make_shared<sas::Asset>(shader, caldare, &window);

	// caldareAsset->basicPVM(camera.get());
	// caldareAsset->translate(glm::vec3(0.f, -10.f, 0.f));
	// caldareAsset->scale(glm::vec3(500.f, 200.f, 500.f));

	// root->addNode(caldareAsset);


	// const auto goldBarsAsset = std::make_shared<sas::Asset>(shader, goldBars, &window);

	// goldBarsAsset->basicPVM(camera.get());
	// goldBarsAsset->translate(glm::vec3(-96.f, 50.f, -478.f));
	// goldBarsAsset->scale(glm::vec3(5.f, 5.f, 5.f));

	// root->addNode(goldBarsAsset);

	// goldBars.draw(shader);

	// const auto gateAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);

	// gateAsset->basicPVM(camera.get());
	// gateAsset->translate(glm::vec3(0.23f, -20.f, -338.f));
	// gateAsset->scale(glm::vec3(0.5f, 0.5f, 0.5f));

	// root->addNode(gateAsset);

	// const auto wallAsset = std::make_shared<sas::Asset>(shader, skyBox, &window);

	// wallAsset->basicPVM(camera.get());

	// root->addNode(wallAsset);

	// wallAsset.draw(shader);
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	glfwSetMouseButtonCallback(window.getWindow(), mouse_callback);
	int health = 100;
#if works
	float boxRotation = 0.f;
	float keyRotation = 0.f;
	float lastPressTime = 0.f;
	constexpr float minPressCooldown = 1.f;

	// State machine for Esc button
	const std::unordered_map<sas::GameState, sas::GameState> states{
		{sas::GameState::MainMenu, sas::GameState::Exit},
		{sas::GameState::Options, sas::GameState::MainMenu},
		{sas::GameState::MainGame, sas::GameState::PauseMenu},
		{sas::GameState::PauseMenu, sas::GameState::MainGame},
		{sas::GameState::PauseOptions, sas::GameState::PauseMenu}};

	// I want constexpr vector :c
	const std::vector<std::function<bool(sas::UI & ui, int nrOfEnemies)>> objectives = {
		firstObjective, seccondObjective, thirdObjective};

	glm::mat4 MVP = glm::mat4(1.0);
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 ProjectionMatrix = glm::perspective(90.f, window.getWidth() * 1.f / window.getHeight(), 0.1f, 10000.f);
	glm::mat4 ViewMatrix = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraViewDirection(), camera->getCameraUp());

	size_t MatrixID2 = glGetUniformLocation(sunShader.getId(), "MVP");
	size_t ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

	// size_t tex = loadBMP("Resources/Textures/City.bmp");
	// size_t tex3 = loadBMP("Resources/Textures/test.bmp");
	// size_t tex6 = loadBMP("Resources/Textures/gate_diffuse.bmp");
	// size_t tex7 = loadBMP("Resources/Textures/brickwall_4.bmp");
	// size_t tex9 = loadBMP("Resources/Textures/Skybox.bmp");

	// glClearColor(0.2f, 0.8f, 1.f, 1.f);

	// Has 1 single purpose
	// glutInit(&argc, argv);
	// init();

	// glEnable(GL_DEPTH_TEST);

	// std::vector<Texture> textures;
	// textures.push_back(Texture());
	// textures[0].id = tex;
	// textures[0].type = "texture_diffuse";

	// std::vector<Texture> textures3;
	// textures3.push_back(Texture());
	// textures3[0].id = tex3;
	// textures3[0].type = "texture_diffuse";

	// std::vector<Texture> textures6;
	// textures6.push_back(Texture());
	// textures6[0].id = tex6;
	// textures6[0].type = "texture_diffuse";

	// std::vector<Texture> textures7;
	// textures7.push_back(Texture());
	// textures7[0].id = tex7;
	// textures7[0].type = "texture_diffuse";

	// std::vector<Texture> textures9;
	// textures9.push_back(Texture());
	// textures9[0].id = tex9;
	// textures9[0].type = "texture_diffuse";

	Mesh sun = loadObj("Resources/Models/sphere.obj");
	// Mesh wall = loadObj("Resources/Models/wall.obj", textures7);
	// Mesh gate = loadObj("Resources/Models/gate_obj.obj", textures6);
	// Mesh caldare = loadObj("Resources/Models/Crate.obj", textures);
	// Mesh key = loadObj("Resources/Models/21929_Key_v1.obj", textures3);
	// Mesh goldBars = loadObj("Resources/Models/gold_bar.obj", textures3);
	// Mesh skyBox = loadObj("Resources/Models/caldare.obj", textures9);

	

	sf::Music backgroundMusic;

	bool opened = backgroundMusic.openFromFile("Music/Johnny Silverhand Theme(CelloViolin version) Cyberpunk 2077.mp3");
	if (!opened)
	{
		std::cerr << "Warning! Background music cannot be opened\n";
	}
	backgroundMusic.setLooping(true);
	backgroundMusic.setVolume(5);
	backgroundMusic.play();

	sas::UI ui(window.getWindow(), health, currentStateOfGame);
#endif
	// currentStateOfGame != sas::GameState::Exit &&
	while (!glfwWindowShouldClose(window.getWindow()))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();

		// sunAsset->basicPVM(camera.get());

		// sunAsset->draw();
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		// sunAsset->draw();
		camera->uppdate(camera.get());
		root->uppdate(camera.get());

		// sun.draw(sunShader);
#if works
		// ui.renderUI();
		// processKeyboardInput(ui, backgroundMusic, states);

		if (currentFrame - lastPressTime >= minPressCooldown)
		{
			canPressKey = true;
			lastPressTime = currentFrame;
		}

		ProjectionMatrix = glm::perspective(90.f, window.getWidth() * 1.f / window.getHeight(), 0.1f, 10000.f);
		ViewMatrix = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraViewDirection(), camera->getCameraUp());
		ModelMatrix = glm::mat4(1.0);
		// MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		// ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

		if (currentStateOfGame == sas::GameState::MainGame)
		{

			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			backgroundMusic.setVolume(40);

			sunShader.use();

			size_t MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");
			// std::cout << MatrixID << ' ' << sunShader.getId() << '\n';
			ModelMatrix = glm::translate(ModelMatrix, lightPos);
			float scaleFactor = 100;
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			sun.draw(sunShader);

			// shader.use();

			// ModelMatrix = glm::mat4(1.0);
			// ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, -50.f, 0.f));
			// ModelMatrix = glm::scale(ModelMatrix, glm::vec3(600.f, 600.f, 600.f));
			// ModelMatrix = glm::rotate(ModelMatrix, boxRotation, glm::vec3{0.f, 1.f, 0.f});

			// MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			// glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// skyBox.draw(shader);

			// Chestia cu care iluminez, dar tot vad soarele
			// glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			// glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			// glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

			// if (spawnKey)
			// {
			// 	ModelMatrix = glm::mat4(1.0);
			// 	ModelMatrix = glm::translate(ModelMatrix, keyPosition);
			// 	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f, 5.f, 5.f));
			// 	ModelMatrix = glm::rotate(ModelMatrix, keyRotation, glm::vec3{0.f, 1.f, 0.f});
			// 	ModelMatrix = glm::rotate(ModelMatrix, 300.f, glm::vec3{1.f, 0.f, 0.f});
			// 	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// 	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

			// 	key.draw(shader);
			// 	keyRotation = keyRotation + 5 * deltaTime;

			// 	if (keyRotation > 360)
			// 		keyRotation = 0;
			// }

			// ModelMatrix = glm::mat4(1.0);
			// ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, -10.f, 0.f));
			// ModelMatrix = glm::scale(ModelMatrix, glm::vec3(500.f, 200.f, 500.f));
			// MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

			// caldare.draw(shader);

			// if (goldAppear)
			// {
			// 	ModelMatrix = glm::mat4(1.0);
			// 	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-96.f, 50.f, -478.f));
			// 	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.f, 5.f, 5.f));
			// 	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// 	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			// 	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// 	goldBars.draw(shader);
			// }

			// if (!openGate)
			// {
			// 	ModelMatrix = glm::mat4(1.0);
			// 	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.23f, -20.f, -338.f));
			// 	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
			// 	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// 	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			// 	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// 	gate.draw(shader);
			// }

			// for (int i = 0; i < 10; ++i)
			// {
			// 	ModelMatrix = glm::mat4(1.0);
			// 	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(47.f + (47.f * i), -20.f, -337.f));
			// 	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(4.f, 4.f, 4.f));
			// 	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// 	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			// 	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// 	wall.draw(shader);
			// }

			// for (int i = 0; i < 10; ++i)
			// {
			// 	ModelMatrix = glm::mat4(1.0);
			// 	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-49.f - (47.f * i), -20.f, -337.f));
			// 	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(4.f, 4.f, 4.f));
			// 	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			// 	glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			// 	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// 	wall.draw(shader);
			// }

			// currentObjective = currentObjective + objectives[currentObjective](ui, 5);
		}
		else
		{
			backgroundMusic.setVolume(5);
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
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

#if works


void processKeyboardInput(sas::UI &ui, sf::Music &music, const std::unordered_map<sas::GameState, sas::GameState> &states) noexcept
{
	float cameraSpeed = 60 * deltaTime;

	if (window.isPressed(GLFW_KEY_ESCAPE) && canPressKey)
	{
		canPressKey = false;
		glfwGetCursorPos(window.getWindow(), &lastX, &lastY);
		currentStateOfGame = states.at(currentStateOfGame);
		if (ui.getVolState())
			music.play();
		else
			music.stop();
	}

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

	if (!openGate)
	{
		if (camera->getCameraPosition().z < szMin)
			camera->setCameraZ(szMin);
	}
	else
	{
		if (camera->getCameraPosition().z < zMin)
			camera->setCameraZ(zMin);
	}

	if (camera->getCameraPosition().z > zMax)
	{
		camera->setCameraZ(zMax);
	}
	if (camera->getCameraPosition().x < xMin)
	{
		camera->setCameraX(xMin);
	}
	if (camera->getCameraPosition().x > xMax)
	{
		camera->setCameraX(xMax);
	}
}

void init() noexcept
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

bool firstObjective(sas::UI &ui, int nrOfEnemies) noexcept
{
	float distance = sqaredDistance(camera->getCameraPosition().x, keyPosition.x, camera->getCameraPosition().z, keyPosition.z);
	if (distance < 250 && glfwGetKey(window.getWindow(), GLFW_KEY_E))
	{
		spawnKey = false;
		keyHand = true;
		ui.nextObjective();
		return true;
	}

	return false;
}

bool seccondObjective(sas::UI &ui, int nrOfEnemies) noexcept
{
	float distance = sqaredDistance(camera->getCameraPosition().x, -0.9f, camera->getCameraPosition().z, -333.f);

	if (distance < 100)
	{
		ui.nextObjective();
		keyHand = false;
		openGate = true;

		return true;
	}

	if (glfwGetKey(window.getWindow(), GLFW_KEY_G))
	{
		keyHand = false;
		spawnKey = true;
		keyPosition = camera->getCameraPosition();
		keyPosition.y = keyPosition.y - 10.f;
		ui.prevObjective();
		currentObjective = currentObjective - 1;
	}

	return false;
}

bool thirdObjective(sas::UI &ui, int nrOfEnemies) noexcept
{

	float distance = sqaredDistance(camera->getCameraPosition().x, -96.f, camera->getCameraPosition().z, -478.f);

	if (distance < 50 * 50 && glfwGetKey(window.getWindow(), GLFW_KEY_E))
	{
		goldAppear = false;
		// Need system such that I dont overflow
		// On finishing last objective
		// Easiest hack -> add an empty one that returns false
		return false;
	}
	return false;
}

float sqaredDistance(float x1, float x2, float y1, float y2) noexcept
{
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
#endif