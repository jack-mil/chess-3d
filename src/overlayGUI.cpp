// Adapted from the ImGUI demo ExampleAppConsole and a MPV UI built with ImGui
// https://github.com/tsl0922/mpv-debug-plugin
// https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L8304

#include "imgui.h"

#include "overlayGUI.hpp"

// Add portable string helpers to ImGui namespace
// from: https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L8346
namespace ImGui { // clang-format off
static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = ImGui::MemAlloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s)      { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
} // clang-format on

using namespace chess3d;

Overlay::Overlay() {
  m_console = new Console();
}

Overlay::~Overlay() {
  if (m_console) { delete m_console; }
}

void Overlay::toggleShow() { m_open = !m_open; }
void Overlay::toggleDebug() { m_demo = !m_demo; }

void Overlay::draw() {
  if (!m_open) { return; }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos, ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Chess 3D!", &m_open)) {
    drawHeader();
    drawConsole();
  }
  ImGui::End();
  if (m_demo) {
    ImGui::ShowDemoWindow(&m_demo);
    ImGui::ShowMetricsWindow(&m_demo);
  }
}

void Overlay::drawHeader() {
  ImGui::Text("Welcome to 3D chess!");
  ImGui::SameLine();
  ImGui::Text("Player makes first move.");
}

void Overlay::drawConsole() {
  ImGui::SetNextItemOpen(true, ImGuiCond_Once);
  if (!ImGui::CollapsingHeader("Moves", ImGuiTreeNodeFlags_DefaultOpen)) {
    return;
  }
  m_console->draw();
}

Overlay::Console::Console() {
  memset(InputBuf, 0, sizeof(InputBuf));
}

Overlay::Console::~Console() {
  for (int i = 0; i < Items.Size; i++) {
    ImGui::MemFree(Items[i]);
  }
  Items.clear();
}

void Overlay::Console::draw() {
  ImGui::Separator();

  // scrolling history feed
  const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None)) {

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    // print log of moves
    for (const char* move : Items) {
      // formatting (colors, etc) could be done here.
      ImGui::TextUnformatted(move);
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }
    ImGui::PopStyleVar();
  }
  ImGui::EndChild();
  ImGui::Separator();

  // Input Line
  bool reclaim_focus = false;
  ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue |
                                         ImGuiInputTextFlags_EscapeClearsAll;
  if (ImGui::InputTextWithHint("Move", "press ENTER to execute",
                               InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags)) {
    char* s = InputBuf;
    ImGui::Strtrim(s);
    if (s[0]) {
      AddLog(s);
    }
    strcpy(s, "");
    reclaim_focus = true;
  }

  // Auto-focus on window apparition
  ImGui::SetItemDefaultFocus();
  if (reclaim_focus) {
    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
  }
}

void Overlay::Console::AddLog(const char* move) {
  Items.push_back(ImGui::Strdup(move));
  // Delete older in-case size grows too long (unlikely)
  // if (Items.Size > LogLimit) {
  //   int offset = Items.Size - LogLimit;
  //   for (int i = 0; i < offset; i++) {
  //     ImGui::MemFree(Items[i]);
  //   }
  //   Items.erase(Items.begin(), Items.begin() + offset);
  // }
}