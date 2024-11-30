#include "UIOverlay.hpp"
#include "OgreImGuiOverlay.h"

#include <imgui.h>

bool ChessDialog::frameStarted(const Ogre::FrameEvent& /* evt */)
{
  Ogre::ImGuiOverlay::NewFrame();

  auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
  auto center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::Begin("Rendering Settings", NULL, flags);
  // Ogre::DrawRenderingSettings(nextRenderer);
  ImGui::Separator();
  if (ImGui::Button("Accept")) {
    // Ogre::Root::getSingleton().queueEndRendering();
    saveConfig = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    saveConfig = false;
    // Ogre::Root::getSingleton().queueEndRendering();
  }
  ImGui::End();
  return true;
}
