#include "chip8.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MODIFIER 10
#define CYCLES_PER_FRAME 8 
int main(int argc, char **argv)
{

    Chip8 chip8;
    InitWindow(SCREEN_WIDTH * MODIFIER, SCREEN_HEIGHT * MODIFIER, "CHIP-8");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);

    initialize(&chip8);

    Rectangle rect = {0,
                      0,
                      MODIFIER,
                      MODIFIER};

    if (argc < 2) {
        printf("Using: %s <rom>\n", argv[0]);
        return 1;
    }
    loadGame(argv[1], &chip8);
    while (!WindowShouldClose())
    {
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            emulateCycle(&chip8);
        }

        if (chip8.drawFlag)
        {

            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
            {
                if (chip8.gfx[i])
                {
                    rect.x = (i % SCREEN_WIDTH) * MODIFIER;
                    rect.y = (i / SCREEN_WIDTH) * MODIFIER;;
                    DrawRectangleRec(rect, WHITE);
                }
            }
            EndDrawing();
            chip8.drawFlag = false;

        }
    }
    CloseWindow();
    return 0;
}
