/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include "emulator.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
Image image;
Texture2D texture;
Vector2 position = { 0,0 };
struct Chip8* emulator = NULL;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    //Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    image.data = NULL;
    image.format = (int)PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    image.mipmaps = 1;
    image.height = VIDEO_HEIGHT;
    image.width = VIDEO_WIDTH;

    emulator = createEmulator();
    loadRom(emulator, file_name);
    loadFonts(emulator);

    texture = LoadTextureFromImage(image);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // Update GAMEPLAY screen variables here!
    
    // Update if Key pressed
    switch (GetKeyPressed()) {
    case KEY_X:
        emulator->keypad[0x0] = 1;
        break;
    case KEY_ONE:
        emulator->keypad[0x1] = 1;
        break;
    case KEY_TWO:
        emulator->keypad[0x2] = 1;
        break;
    case KEY_THREE:
        emulator->keypad[0x3] = 1;
        break;
    case KEY_Q:
        emulator->keypad[0x4] = 1;
        break;
    case KEY_W:
        emulator->keypad[0x5] = 1;
        break;
    case KEY_E:
        emulator->keypad[0x6] = 1;
        break;
    case KEY_A:
        emulator->keypad[0x7] = 1;
        break;
    case KEY_S:
        emulator->keypad[0x8] = 1;
        break;
    case KEY_D:
        emulator->keypad[0x9] = 1;
        break;
    case KEY_Z:
        emulator->keypad[0xA] = 1;
        break;
    case KEY_C:
        emulator->keypad[0xB] = 1;
        break;
    case KEY_FOUR:
        emulator->keypad[0xC] = 1;
        break;
    case KEY_R:
        emulator->keypad[0xD] = 1;
        break;
    case KEY_F:
        emulator->keypad[0xE] = 1;
        break;
    case KEY_V:
        emulator->keypad[0xF] = 1;
        break;
    default:
        break;
    }

    // Update if key released
    if (IsKeyReleased(KEY_X))
        emulator->keypad[0x0] = 0;
    else if (IsKeyReleased(KEY_ONE))
        emulator->keypad[0x1] = 0;
    else if (IsKeyReleased(KEY_TWO))
        emulator->keypad[0x2] = 0;
    else if (IsKeyReleased(KEY_THREE))
        emulator->keypad[0x3] = 0;
    else if (IsKeyReleased(KEY_Q))
        emulator->keypad[0x4] = 0;
    else if (IsKeyReleased(KEY_W))
        emulator->keypad[0x5] = 0;
    else if (IsKeyReleased(KEY_E))
        emulator->keypad[0x6] = 0;
    else if (IsKeyReleased(KEY_A))
        emulator->keypad[0x7] = 0;
    else if (IsKeyReleased(KEY_S))
        emulator->keypad[0x8] = 0;
    else if (IsKeyReleased(KEY_D))
        emulator->keypad[0x9] = 0;
    else if (IsKeyReleased(KEY_Z))
        emulator->keypad[0xA] = 0;
    else if (IsKeyReleased(KEY_C))
        emulator->keypad[0xB] = 0;
    else if (IsKeyReleased(KEY_FOUR))
        emulator->keypad[0xC] = 0;
    else if (IsKeyReleased(KEY_R))
        emulator->keypad[0xD] = 0;
    else if (IsKeyReleased(KEY_F))
        emulator->keypad[0xE] = 0;
    else if (IsKeyReleased(KEY_V))
        emulator->keypad[0xF] = 0;

    // Click to switch to title
    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySound(fxCoin);
        finishScreen = 1; // TITLE
    }

    // Sound
    if (emulator->soundTimer > 0)
        PlaySound(fxBeep);

    // Fetch-Decode-Execute
    Cycle(emulator);

    // Turns the video memory into a displayable texture
    UpdateTexture(texture, emulator->video);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // Draw GAMEPLAY screen here!
    ClearBackground(BLACK);
    DrawTextureEx(texture, position, 0, 10, SKYBLUE);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // Unload GAMEPLAY screen variables here!
    UnloadTexture(texture);
    free(emulator);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}