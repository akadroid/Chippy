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
#include <stdint.h>

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
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        PlaySound(fxCoin);
        finishScreen = 1; // TITLE
    }
    
    Cycle(emulator);
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