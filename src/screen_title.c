/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

static int framesCounter = 0;
static int finishScreen = 0;

// Buttons
bool showButton;
Texture2D button;
float frameHeight;
Rectangle sourceRec;
Rectangle btnBounds;
int btnState;
bool btnAction;
Vector2 mousePoint;

// File loading
char file_name[250];

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    //Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    file_name[0] = '\0';

    // Button variables
    showButton = false;
    button = LoadTexture("../resources/button.png");
    frameHeight = (float)button.height / 3;
    sourceRec = (Rectangle) { 0, 0, (float)button.width, frameHeight };
    btnBounds = (Rectangle){ GetScreenWidth() / 2.0f - button.width / 2.0f, GetScreenHeight() / 2.0f - button.height / 3 / 2.0f + 150, (float)button.width, frameHeight };
    btnState = 0;
    btnAction = false;
    mousePoint = (Vector2){ 0.0f, 0.0f };
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    /* Getting ROM PATH when dragged onto window */
    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        showButton = true;

        char tmp[200] = { 0 };
        TextCopy(file_name, droppedFiles.paths[0]);
        printf("%s\n", file_name);
        UnloadDroppedFiles(droppedFiles);
    }

    /* Button Press */
    if (showButton) {
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds)) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState = 0;

        // Press button to change to GAMEPLAY screen
        if (btnAction) {
            PlaySound(fxCoin);
            finishScreen = 1;   // GAMEPLAY
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState * frameHeight;
    }

    /* Load Default ROM*/
    if (IsKeyPressed(KEY_ENTER)) {
        PlaySound(fxCoin);
        TextCopy(file_name, "../resources/roms/pong.c8");
        finishScreen = 1;
    }
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
    Vector2 pos = MeasureTextEx(font, "CHIP-8 EMULATOR", font.baseSize * 4.0f, 4);
    pos.x = (float)GetScreenWidth() / 2 - pos.x / 2;
    pos.y = 10;
    float dropRomWidth = (float)GetScreenWidth()/2 - (float)MeasureText("DROP A .ch8 ROM INTO THIS WINDOW!", 30)/2;
    
    // Draw title screen
    ClearBackground(BLACK);
    DrawTextEx(font, "CHIP-8 EMULATOR", pos, font.baseSize*4.0f, 4, WHITE);
    DrawText("DROP A .ch8 ROM INTO THIS WINDOW!", dropRomWidth, GetScreenHeight()/2 - 30, 30, SKYBLUE);

    if (showButton)
        DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    //Unload TITLE screen variables here!
    UnloadTexture(button); // weird line that appears when I unload, not sure how to fix. has something to do with texture
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}