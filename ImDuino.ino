#define SCREEN_MODE_8
//#define CLIP_SCREEN
#include "softraster.h"
#include "imgui.h"

#ifndef ESP32
#define ESP32
#endif // ESP32

#include "SPI.h"
#include <TFT_22_ILI9225.h>

//#define PRINT_ATLAS
#define SMALL_ATLAS
#include "fontAtlas.h"

#include "ESP32pinout.h"

const uint8_t TFTLED = PIN32; //PIN25;
const uint8_t TFTRST = PIN33; //PIN26;
const uint8_t TFTRS  = PIN27;
const uint8_t TFTCS  = HSPICS0;
const uint8_t TFTCLK = HSPICLK;
const uint8_t TFTSDI = HSPIMOSI;

#define TFTX 220
#define TFTY 176

texture_t<color16_t> screen;
texture_t<alpha8_t> fontAtlas;

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFTRST, TFTRS, TFTCS, TFTLED, 128);
SPIClass tftspi(HSPI);

void updateScreen()
{
    tft.drawBitmap(0, 0, (uint16_t*)screen.pixels, screen.w, screen.h);
}

unsigned long drawTime;
unsigned long rasterTime;

void renderFunc(ImDrawData* drawData)
{
    rasterTime = millis();
    renderDrawLists(drawData, screen);
    rasterTime = millis() - rasterTime;

    drawTime = millis();
    updateScreen();
    drawTime = millis() - drawTime;
}

void setup()
{
    Serial.begin(115200);

    tft.begin(tftspi);
    tft.setFont(Terminal6x8);
    tft.setOrientation(3);
    digitalWrite(TFTLED, HIGH);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = TFTX;
    io.DisplaySize.y = TFTY;

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = false;
    style.AntiAliasedFill = false;
    style.WindowRounding = 0.0f;

    #ifdef SMALL_ATLAS
    io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight | ImFontAtlasFlags_NoMouseCursors;
    #endif

    uint8_t* pixels;
    int width, height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    fontAtlas.w = width;
    fontAtlas.h = height;

    #ifdef PRINT_ATLAS
    Serial.println("uint8_t const fontAtlasPixels[] = {");
    int depth = 0;
    for(int y = 0; y < fontAtlas.h; y++)
    {
        for(int x = 0; x < fontAtlas.w; x++)
        {
            Serial.print("0x");
            char c = pixels[x + y*fontAtlas.w] >> 4;
            if (c < 0xA)
            {
                c += '0';
            }
            else
            {
                c -= 0xA;
                c += 'A';
            }
            Serial.print(c);
            c = pixels[x + y*fontAtlas.w] & 0xF;
            if (c < 0xA)
            {
                c += '0';
            }
            else
            {
                c -= 0xA;
                c += 'A';
            }
            Serial.print(c);
            Serial.print(", ");
            if (depth++ > 16)
            {
                depth = 0;
                Serial.println("");
            }
        }
    }
    Serial.print("};");
    #endif

    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();    //ImGui::MemFree(pixels);

    fontAtlas.init(width, height, (alpha8_t*)fontAtlasPixels);

    io.Fonts->TexID = &fontAtlas;

    screen.init(TFTX, TFTY);
}

float f = 0.0f;
unsigned long time = 0;

// try while 1 in loop instead of going off the end of the loop (add delay(0))
void loop()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = 1.0f/60.0f;
    //io.MousePos = mouse_pos;
    //io.MouseDown[0] = mouse_button_0;
    //io.MouseDown[1] = mouse_button_1;
    //[0.0f - 1.0f]
    // io.NavFlags |= ImGuiNavFlags_EnableGamepad;
    io.NavInputs[ImGuiNavInput_Activate] = 0.0f;        // activate / open / toggle / tweak value       // e.g. Circle (PS4), A (Xbox), B (Switch), Space (Keyboard)
    io.NavInputs[ImGuiNavInput_Cancel] = 0.0f;          // cancel / close / exit                        // e.g. Cross    (PS4), B (Xbox), A (Switch), Escape (Keyboard)
    io.NavInputs[ImGuiNavInput_Input] = 0.0f;           // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
    io.NavInputs[ImGuiNavInput_Menu] = 0.0f;            // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
    io.NavInputs[ImGuiNavInput_DpadLeft] = 0.0f;        // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
    io.NavInputs[ImGuiNavInput_DpadRight] = 0.0f;
    io.NavInputs[ImGuiNavInput_DpadUp] = 0.0f;
    io.NavInputs[ImGuiNavInput_DpadDown] = 0.0f;
    io.NavInputs[ImGuiNavInput_TweakSlow] = 0.0f;       // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    io.NavInputs[ImGuiNavInput_TweakFast] = 0.0f;       // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

    ImGui::NewFrame();
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetWindowSize(ImVec2(TFTX, TFTY));

    f += 0.05;
    if(f > 1.0f) f = 0.0f;

    unsigned int deltaTime = millis() - time;
    time += deltaTime;

    deltaTime -= (drawTime + rasterTime);

    ImGui::Text("SPI screen draw time %d ms", drawTime);

    ImGui::Text("Raster time %d ms", rasterTime);
    Serial.println(rasterTime);

    ImGui::Text("Remaining time %d ms", deltaTime);

    ImGui::SliderFloat("SliderFloat", &f, 0.0f, 1.0f);

    screen.clear();

    ImGui::Render();
    renderFunc(ImGui::GetDrawData());
}

