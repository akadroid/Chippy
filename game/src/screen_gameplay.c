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
static Image* image = NULL;
Texture2D texture;
static uint32_t BRUH[] = { 0xFFFFFFFF, 0xFFFFFFFF };
Vector2 position = { 100,100 };
struct Chip8* emulator = NULL;

const char *filename = "IB.ch8";

int x = -5;
int y = -5;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    image = (Image*)malloc(sizeof(Image));
    image->data = NULL;
    image->format = (int)PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    image->mipmaps = 1;
    image->height = VIDEO_HEIGHT;
    image->width = VIDEO_WIDTH;

    emulator = createEmulator();
    loadRom(emulator, filename);
    loadFonts(emulator);

    texture = LoadTextureFromImage(*image);
    //UpdateTexture(texture, emulator->video);
    //UnloadImage(*image);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        //finishScreen = 1;
        PlaySound(fxCoin);
    }
    
    Cycle(emulator);
    //texture = LoadTextureFromImage(*image);
    UpdateTexture(texture, emulator->video);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    ClearBackground(BLACK);
    DrawTextureEx(texture, position, 0, 10, RED);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadTexture(texture);
    free(image);
    free(emulator);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}