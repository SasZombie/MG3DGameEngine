#include "window.hpp"

#include <iostream>

//Handling keyboard actions
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* wind = (Window*) glfwGetWindowUserPointer(window);

	if (action != GLFW_RELEASE)
		wind->setKey(key, true);
	else
		wind->setKey(key, false);
}

//Handling mouse actions
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* wind = (Window*)glfwGetWindowUserPointer(window);

	if (action != GLFW_RELEASE)
		wind->setMouseButton(button, true);
	else
		wind->setMouseButton(button, false);
}

//Handling cursor position
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* wind = (Window*)glfwGetWindowUserPointer(window);
	wind->setMousePos(xpos, ypos);
}


static void error_callback( int error, const char *msg ) 
{
    //Endl for buffer flush on error
	std::cerr << "\033[31m" << " [" + std::to_string(error) + "] " + msg + '\n' << "\033[0m "<<  std::endl;
	//			  ^ red color                                                       ^ reset
}

Window::Window(const char* nName, int nWidth, int nHeight)
	: name(nName), width(nWidth), height(nHeight)
{
	init();
}

Window::~Window() noexcept
{
	glfwTerminate();
}

void Window::init()
{
	glfwSetErrorCallback( error_callback );

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}
	else
	{
		std::cout << "Successfully initializing glfw!\n";
	}

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == nullptr)
	{
		std::cerr << "Failed to create a GLFW window\n";
		glfwTerminate();

		throw std::runtime_error("Failed to open Window");
	}

	glfwMakeContextCurrent(window);

	//callbacks for user input
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialize GLEW");
	}
	else
	{
		std::cout << "Successfully initializing glew!\n";
	}

	std::cout << "Open GL " << glGetString(GL_VERSION) << '\n';
}

void Window::update() noexcept
{
	glfwPollEvents();
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSwapBuffers(window);
}

void Window::clear() noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::getWindow() const noexcept
{
	return window;
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

void Window::setKey(int key, bool ok)
{
	this -> keys[key] = ok;
}

void Window::setMouseButton(int button, bool ok)
{
	this->mouseButtons[button] = ok;
}

void Window::setMousePos(double newXpos, double newYpos)
{
	this->xpos = newXpos;
	this->ypos = newYpos;
}

void Window::getMousePos(double &nXpos, double &nYpos)
{
	nXpos = this->xpos;
	nYpos = this->ypos;
}

//Handling key pressed
bool Window::isPressed(int key)
{
	return keys[key];
}

//Handling mouse buttons pressed
bool Window::isMousePressed(int button)
{
	return mouseButtons[button];
}