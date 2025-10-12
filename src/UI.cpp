#include "UI.hpp"
#include <utility>

void sas::UI::renderPause() const noexcept
{
    constexpr float buttonSizeL = 150, buttonSizeR = 50;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(1920/2 - 100 , 1080/2 - 150 ), 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(200, 200));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Render Options" , nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );   
    
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.00f, 0.0f, 1);    
    
    if (ImGui::Button("Resume", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::MainGame;        
    }    

    if (ImGui::Button("Options", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::PauseOptions;
    }

    if (ImGui::Button("Main Menu", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::MainMenu;
    }            

    if (ImGui::IsItemHovered())
    {
        style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);
    } 
    else
    {   
        style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.056f, 0.19f, 1.0f);
    }

    ImGui::End();
    ImGui::PopStyleColor();  
    ImGui::Render();
    
}

void sas::UI::renderOptions() const noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(1920/2 - 100 , 1080/2 - 150 ), 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(200, 200));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Render Menu" , nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );   
    
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.00f, 0.0f, 1);    

    style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.056f, 0.19f, 1.0f);
    
    ImGui::Checkbox("Volume", &this->volume);

    ImGui::End();
    ImGui::PopStyleColor();  
    ImGui::Render();


}

void sas::UI::renderMain() const noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 100), ImVec2(250, 200));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Render Menu" , nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );   

    ImGui::Text("Health");
    ImGui::SameLine();
    ImGui::ProgressBar(health * 0.01, ImVec2(0.f, 15.0f), " ");
    
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 0.f);    
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);   
    ImGui::Text("%s", objective[objectiveIndex].c_str());

    ImGui::End();
    ImGui::PopStyleColor();  
    ImGui::Render();
}

void sas::UI::renderMenu() const noexcept
{

    static constexpr float buttonSizeL = 150, buttonSizeR = 50;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(1920/2 - 100 , 1080/2 - 150 ), 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(200, 200));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Render Menu" , nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );   
    
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.00f, 0.0f, 1);    
 
    if (ImGui::Button("Start", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::MainGame;        
    }    

    if (ImGui::Button("Options", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::Options;
    }   

    if (ImGui::Button("Exit", {buttonSizeL, buttonSizeR}))
    {
        this->currState = GameState::Exit;
    }               

    if (ImGui::IsItemHovered())
    {
        style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);
    } 
    else
    {   
        style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.056f, 0.19f, 1.0f);
    }
    
    ImGui::End();
    ImGui::PopStyleColor();  
    ImGui::Render();

}

void sas::UI::renderEnding() const noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(1920/2 - 100 , 1080/2 - 150 ), 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(200, 200));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Render Menu" , nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );                   
    ImGui::End();

    ImGui::PopStyleColor();  
    ImGui::Render();;   
}

void sas::UI::renderUI() const noexcept
{
    switch (currState)
    {
        case GameState::MainMenu:
            renderMenu();
            break;
        case GameState::MainGame:
            renderMain();
            break;
        case GameState::Options:
        case GameState::PauseOptions:
            renderOptions();
            break;
        case GameState::PauseMenu:
            renderPause();
            break;
        default:
            assert(false && "Unreachable render UI case");
            std::unreachable();
            break;
    }
}

bool sas::UI::getVolState() const noexcept
{
    return this->volume;
}

void sas::UI::uppdateUI(const GameState &nextState) noexcept
{
    this->currState = nextState;
}

void sas::UI::nextObjective() noexcept
{
    ++this->objectiveIndex;
}

void sas::UI::prevObjective() noexcept
{
    --this->objectiveIndex;
}

void sas::UI::resetObjective() noexcept
{
    this->objectiveIndex = 0;
}
