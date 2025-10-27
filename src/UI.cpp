#include "UI.hpp"

#include "camera.hpp"
#include <fstream>

sas::UI::UI(Window *nwindow) noexcept
    : window(nwindow)
{
}

void sas::UI::save(std::ofstream& out) noexcept
{
    out << sas::SerializeCodes::UI << '\n';
    SceneNode::save(out);
}

//This is not how it is supposed to be drawn
//Instead it should be used in the draw function
//Again this is a short-coming due to the lack of
//Specializations of Asset, explained in the 
//Read me
void sas::UI::uppdate(const Camera *camera) noexcept
{
    const auto &pos = parent.lock();

    if (!pos)
    {
        return;
    }

    glm::vec3 cameraPos = camera->getCameraPosition();
    glm::vec3 objectPos = pos->worldTransform.position;
    float distance = glm::length(cameraPos - objectPos);

    if (distance > 10.f)
        return;
    const auto &clipSpace = pos->getClipSpacePos();
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
    float screenX = (ndc.x * 0.5f + 0.5f) * 1920;
    float screenY = (1.0f - (ndc.y * 0.5f + 0.5f)) * 1080; // must invert Y

    ImDrawList *drawList = ImGui::GetForegroundDrawList();
    drawList->AddText(ImVec2(screenX, screenY), IM_COL32(255, 255, 255, 255), pos->name.c_str());
};