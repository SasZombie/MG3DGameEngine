///@brief
///Read me [1]
#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma GCC diagnostic pop

#include <string>

#define MAX_KEYBOARD 512
#define MAX_MOUSE 8


class Window
{
	private:
		const char* name;
		int width, height;
		GLFWwindow* window;

		bool keys[MAX_KEYBOARD]{}; //Default false
		bool mouseButtons[MAX_MOUSE]{}; //Default fasle
		double xpos;
		double ypos;
	
	public:
		Window(const char* name, int width, int height);
		~Window() noexcept;
		GLFWwindow* getWindow() const noexcept;

		void init();
		void update() noexcept;
		void clear() noexcept;

		void setKey(int key, bool ok);
		void setMouseButton(int button, bool ok);
		void setMousePos(double xpos, double ypos);
		void getMousePos(double &xpos, double &ypos);
		bool isPressed(int key);
		bool isMousePressed(int button);

		int getWidth();
		int getHeight();
};