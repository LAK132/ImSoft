#include "imgui.h"

#define TFTX 220
#define TFTY 176

void TFTRenderFunc(ImDrawData* draw_data)
{
  /*for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
    const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
    const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
    {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback)
      {
        pcmd->UserCallback(cmd_list, pcmd);
      }
      else
      {
        //MyEngineBindTexture(pcmd->TextureId);
        //MyEngineScissor((int)pcmd->ClipRect.x, (int)pcmd->ClipRect.y, (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        //MyEngineDrawIndexedTriangles(pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer, vtx_buffer);
      }
      idx_buffer += pcmd->ElemCount;
    }
  }*/
}

void setup()
{
  Serial.begin(115200);
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = TFTX;
  io.DisplaySize.y = TFTY;
  io.RenderDrawListsFn = TFTRenderFunc;
  unsigned char* pixels;
  int width, height;
  //io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
  io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
  //MyTexture* texture = MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA)
  //io.Fonts->TexID = (void*)texture;
}

void loop()
{
  ImGuiIO& io = ImGui::GetIO();
  io.DeltaTime = 1.0f/60.0f;
  //io.MousePos = mouse_pos;
  //io.MouseDown[0] = mouse_button_0;
  //io.MouseDown[1] = mouse_button_1;
  //[0.0f - 1.0f]
  io.NavFlags |= ImGuiNavFlags_EnableGamepad;
  io.NavInputs[ImGuiNavInput_Activate] = 0.0f;    // activate / open / toggle / tweak value       // e.g. Circle (PS4), A (Xbox), B (Switch), Space (Keyboard)
  io.NavInputs[ImGuiNavInput_Cancel] = 0.0f;      // cancel / close / exit                        // e.g. Cross  (PS4), B (Xbox), A (Switch), Escape (Keyboard)
  io.NavInputs[ImGuiNavInput_Input] = 0.0f;       // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
  io.NavInputs[ImGuiNavInput_Menu] = 0.0f;        // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
  io.NavInputs[ImGuiNavInput_DpadLeft] = 0.0f;    // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
  io.NavInputs[ImGuiNavInput_DpadRight] = 0.0f;
  io.NavInputs[ImGuiNavInput_DpadUp] = 0.0f;
  io.NavInputs[ImGuiNavInput_DpadDown] = 0.0f;
  io.NavInputs[ImGuiNavInput_TweakSlow] = 0.0f;   // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
  io.NavInputs[ImGuiNavInput_TweakFast] = 0.0f;   // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

  ImGui::NewFrame();
  
  static float f = 0.0f;
  ImGui::Text("Hello, world!");
  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

  ImGui::Render();
  //SwapBuffers();
  Serial.print("Cool ");
  Serial.println(f);
}

