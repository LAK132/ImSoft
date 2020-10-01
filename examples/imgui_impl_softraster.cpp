#include "imgui_impl_softraster.h"
#include "../misc/softraster/softraster.h"

texture_base_t *Screen = nullptr;

bool ImGui_ImplSoftraster_Init(texture_base_t *screen)
{
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

  ImGuiIO &io      = ImGui::GetIO();
  io.DisplaySize.x = Screen->w;
  io.DisplaySize.y = Screen->h;
}

void ImGui_ImplSoftraster_RenderDrawData(ImDrawData *draw_data)
{
  if (Screen == nullptr) return;

  Screen->clear();

  // using pos_t = uint32_t;
  using pos_t = float;

  switch (Screen->type)
  {
    case texture_type_t::ALPHA8:
      renderDrawLists<pos_t>(draw_data,
                             *reinterpret_cast<texture_alpha8_t *>(Screen));
      break;

    case texture_type_t::VALUE8:
      renderDrawLists<pos_t>(draw_data,
                             *reinterpret_cast<texture_value8_t *>(Screen));
      break;

    case texture_type_t::COLOR16:
      renderDrawLists<pos_t>(draw_data,
                             *reinterpret_cast<texture_color16_t *>(Screen));
      break;

    case texture_type_t::COLOR24:
      renderDrawLists<pos_t>(draw_data,
                             *reinterpret_cast<texture_color24_t *>(Screen));
      break;

    case texture_type_t::COLOR32:
      renderDrawLists<pos_t>(draw_data,
                             *reinterpret_cast<texture_color32_t *>(Screen));
      break;

    default: return;
  }
}
