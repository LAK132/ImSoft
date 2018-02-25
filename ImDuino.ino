#define SCREEN_MODE_8
//#define CLIP_SCREEN
#include "softraster.h"
#include "imgui.h"

#define ESP32

#include "SPI.h"
#include <TFT_22_ILI9225.h>

//#define PRINT_ATLAS
#define SMALL_ATLAS
#include "fontAtlas.h"

#include "ESP32pinout.h"

const uint8_t TFTLED =  PIN32; //PIN25;
const uint8_t TFTRST =  PIN33; //PIN26;
const uint8_t TFTRS =   PIN27;
const uint8_t TFTCS =   HSPICS0; 
const uint8_t TFTCLK =  HSPICLK; 
const uint8_t TFTSDI =  HSPIMOSI;

#define TFTX 220
#define TFTY 176

screen_t<color16_t> screen;
clip_t screenClip;
texture_t<color16_t> screenBuffer;
texture_t<color8_t> fontAtlas;
void_texture_t fontAtlasVdPtr;

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFTRST, TFTRS, TFTCS, TFTSDI, TFTCLK, TFTLED, 128);
//SPIClass tftspi(HSPI);

void updateScreen()
{
  int16_t x1 = screenClip.x1 > 0 ? screenClip.x1 : 0;
  int16_t y1 = screenClip.y1 > 0 ? screenClip.y1 : 0;
  int16_t x2 = screenClip.x2 < TFTX ? screenClip.x2 : TFTX-1;
  int16_t y2 = screenClip.y2 < TFTY ? screenClip.y2 : TFTY-1;

  if (x1 == -1 || y1 == -1 || x2 == -1 || y2 == -1) return;

  tft.startWrite(x1, y1, x2, y2);
  for (int16_t x = x2; x >= x1; x--) //uint16_t x causes undeflow
  {
    for (int16_t y = y1; y <= y2; y++) 
    {
      uint16_t& col = screenBuffer.col[x][y];
      tft._spiWrite(col>>8);
      tft._spiWrite(col);
    }
  }
  tft.endWrite();

  #ifdef CLIP_SCREEN
  screenClip.x1 = -1;
  screenClip.y1 = -1;
  screenClip.x2 = -1;
  screenClip.y2 = -1;
  #else
  screenClip.x1 = 0;
  screenClip.y1 = 0;
  screenClip.x2 = TFTX-1;
  screenClip.y2 = TFTY-1;
  #endif
}

unsigned long drawTime;
unsigned long rasterTime;

void renderFunc(ImDrawData* drawData)
{
  rasterTime = millis();
  Softraster::renderDrawLists(drawData, &screen);
  rasterTime = millis() - rasterTime;

  drawTime = millis();
  updateScreen();
  drawTime = millis() - drawTime;
}

void setup()
{
  Serial.begin(115200);

  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.begin();
  //tftspi.begin(TFTCLK, HSPIMISO, TFTSDI, TFTCS);
  tft.begin(/*tftspi*/);
  tft.setFont(Terminal6x8);
  tft.setOrientation(3);
  digitalWrite(TFTLED, HIGH);
    
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = TFTX;
  io.DisplaySize.y = TFTY;
  //io.RenderDrawListsFn = renderFunc; //obsolete

  ImGuiStyle& style = ImGui::GetStyle();
  style.AntiAliasedLines = false;
  style.AntiAliasedFill = false;
  style.WindowRounding = 0.0f;
  
  #ifdef SMALL_ATLAS
  io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight | ImFontAtlasFlags_NoMouseCursors;
  #endif

  uint8_t* pixels;
  int width, height;
  io.Fonts->GetTexDataAsAlpha8(&pixels, &(width), &(height));
  fontAtlas.w = width;
  fontAtlas.h = height;

  #ifdef PRINT_ATLAS
  Serial.print("{");
  for(int x = 0; x < fontAtlas.tex8.w; x++)
  {
    for(int y = 0; y < fontAtlas.tex8.h; y++)
    {
      Serial.print("0x");
      Serial.print(pixels[x + y*fontAtlas.tex8.w], HEX);
      Serial.print(", ");
    }
    Serial.println("");
  }
  Serial.print("}");
  #endif

  io.Fonts->ClearInputData();
  io.Fonts->ClearTexData();  //ImGui::MemFree(pixels);

  fontAtlas.pre_init();
  for(size_t i = 0; i < fontAtlas.w; i++)
  {
    fontAtlas.col[i] = (color8_t*)&(fontAtlasPixels[i * fontAtlas.h]);
  }
  
  fontAtlasVdPtr.texture = (void*)&fontAtlas;
  fontAtlasVdPtr.mode = fontAtlas.colorMode;

  io.Fonts->TexID = &fontAtlasVdPtr;

  screenBuffer.init(TFTX, TFTY);

  screen.w = TFTX;
  screen.h = TFTY;
  screen.buffer = &screenBuffer;
  screen.clip = &screenClip;
}

float f = 0.0f;
float time = 0.0f;

// try while 1 in loop instead of going off the end of the loop (add delay(0))
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
  ImGui::SetWindowPos(ImVec2(0.0, 0.0));
  ImGui::SetWindowSize(ImVec2(TFTX, TFTY));
  
  f += 0.05;
  if(f > 1.0f) f = 0.0f;
  
  float deltaTime = millis() - time;
  time += deltaTime;
  
  deltaTime -= (drawTime + rasterTime);
  
  //Serial.print("Draw time: "); Serial.println(drawTime);
  ImGui::Text("Draw time %f ms", drawTime / 1.0f);
  
  //Serial.print("Raster time: "); Serial.println(rasterTime);
  ImGui::Text("Raster time %f ms", rasterTime / 1.0f);

  //Serial.print("Remaining time: "); Serial.println(deltaTime);
  ImGui::Text("Remaining time %f ms", deltaTime);
  
  ImGui::SliderFloat("float3", &f, 0.0f, 1.0f);

  screenBuffer.clear();
  
  ImGui::Render();
  renderFunc(ImGui::GetDrawData());
  // Serial.println("fin bitconnnnnnnnnneeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeect");
  //delay(5000);
}

