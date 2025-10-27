#pragma once

#include <vector>
#include <string>
#include <climits>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../iamgui/imgui.h"

#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"

#pragma GCC diagnostic pop

#include "window.hpp"
#include "SceneNode.hpp"

#include "States.hpp"


namespace sas
{
  class UI : public SceneNode
  {
    private:
      Window* window;

    public:
      UI(Window *window) noexcept;

      void save(std::ofstream &out) noexcept override;

      void uppdate(const Camera* camera) noexcept override;
  };
      
} 