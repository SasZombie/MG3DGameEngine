#pragma once

#include <vector>
#include <string>
#include <climits>
#include <iostream>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../iamgui/imgui.h"

#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"

#pragma GCC diagnostic pop

#include "States.hpp"


namespace sas
{
  class UI
  {
    private:
      
      mutable bool volume = true;
      int &health;
      int objectiveIndex = 0;
      GameState &currState;
      std::string objective[4] = {"Get the key", "Open the gate", "Take the gold", "Celebrate!:D"};
      ImGuiIO io;


      void renderPause() const noexcept;
      void renderOptions() const noexcept;
      void renderMain() const noexcept;
      void renderMenu() const noexcept;
      void renderEnding() const noexcept;

    public:
      UI(GLFWwindow *window, int &Nhealth, GameState &NcurrState)
        :  health(Nhealth), currState(NcurrState)
      {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        this->io = ImGui::GetIO();

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130"); 
      };

      bool getVolState() const noexcept;
      
      void nextObjective() noexcept;
      void prevObjective() noexcept; 
      void resetObjective() noexcept; 
      void renderUI() const noexcept;
      void uppdateUI(const GameState &nextState) noexcept;
  };
      
} 