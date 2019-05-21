#include "imgui_impl_softraster.h"
#include "../misc/softraster/softraster.h"

texture_base_t *Screen = nullptr;

bool ImGui_ImplSoftraster_Init(texture_base_t *screen)
{
    ImGui::CreateContext();

    if (screen != nullptr)
    {
        Screen = screen;
        return true;
    }
    return false;
}

void ImGui_ImplSoftraster_Shutdown()
{
    Screen = nullptr;
}

void ImGui_ImplSoftraster_NewFrame()
{
    if (Screen == nullptr) return;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = Screen->w;
    io.DisplaySize.y = Screen->h;

    ImGui::NewFrame();
}

void ImGui_ImplSoftraster_RenderDrawData(ImDrawData* draw_data)
{
    if (Screen == nullptr) return;

    Screen->clear();

    switch (Screen->type)
    {
    case ALPHA8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_alpha8_t*>(Screen));
        break;

    case VALUE8:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_value8_t*>(Screen));
        break;

    case COLOR16:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color16_t*>(Screen));
        break;

    case COLOR24:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color24_t*>(Screen));
        break;

    case COLOR32:
        renderDrawLists<int32_t>(draw_data, *reinterpret_cast<texture_color32_t*>(Screen));
        break;

    default: return;
    }
}

