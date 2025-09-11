#include "chip8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "raylib.h"
unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initialize(Chip8 *chip8)
{
    chip8->pc = 0x200; // Program counter starts at 0x200 (Start adress program)
    chip8->opcode = 0; // Reset current opcode
    chip8->I = 0;      // Reset index register
    chip8->sp = 0;     // Reset stack pointer

    // Clear display
    for (int i = 0; i < 2048; ++i)
        chip8->gfx[i] = 0;

    // Clear stack
    for (int i = 0; i < 16; ++i)
        chip8->stack[i] = 0;

    for (int i = 0; i < 16; ++i)
        chip8->key[i] = chip8->V[i] = 0;

    // Clear memory
    for (int i = 0; i < 4096; ++i)
        chip8->memory[i] = 0;

    // Load fontset
    for (int i = 0; i < 80; ++i)
        chip8->memory[i] = chip8_fontset[i];

    // Reset timers
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    // Clear screen once
    chip8->drawFlag = true;

    srand(time(NULL));
}
void loadGame(char title[], Chip8 *chip8)
{
    printf("\"Loaded ROM:\" %s\n", title);
    FILE *gamebin;
    gamebin = fopen(title, "rb");

    if (gamebin == NULL)
    {
        printf("file.txt file failed to open.");
        return;
    }
    fseek(gamebin, 0, SEEK_END);
    unsigned long lSize = ftell(gamebin);
    rewind(gamebin);
    printf("Filesize: %d\n", (int)lSize);

    // Allocate memory to contain the whole file
    char *buffer = (char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        return;
    }

    // Copy the file into the buffer
    size_t result = fread(buffer, 1, lSize, gamebin);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        return;
    }

    // Copy buffer to Chip8 memory
    if ((4096 - 512) > lSize)
    {
        for (int i = 0; i < (int)lSize; ++i)
            chip8->memory[i + 512] = buffer[i];
    }
    else
    {
        printf("Error: ROM too big for memory");
    }

    fclose(gamebin);
    free(buffer);
}

void emulateCycle(Chip8 *chip8)
{

    // Fetch opcode
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
        // Address
    unsigned int nnn = chip8->opcode & 0x0FFF;

    // 8 Bit constant
    unsigned int nn = chip8->opcode & 0x00FF;

    // 4 Bit constant
    unsigned int n = chip8->opcode & 0x000F;

    // 4 bit register identifier
    unsigned int X = (chip8->opcode >> 8) & 0x000F;
    unsigned int Y = (chip8->opcode >> 4) & 0x000F;

    // printf("Program Counter: %d \n", chip8->pc);
    chip8->pc += 2;

    // printf("opcode:0x%X\n", chip8->opcode);
    // printf("nnn: 0x%X\n", nnn);
    // printf("nn: 0x%X\n", nn);
    // printf("n: 0x%X\n", n);
    switch (chip8->opcode & 0xF000)
    {
    case 0x0:
        switch (chip8->opcode & 0x00FF)
        {
        // Clears the screen
        case 0x00E0:
            memset(chip8->gfx, 0, sizeof(chip8->gfx));
            break;

        // Returns from subroutine
        case 0x00EE:
            chip8->sp--;
            chip8->pc = chip8->stack[chip8->sp];

            break;
        }
        break;

    // Pula pro endereço NNN
    case 0x1000:
        chip8->pc = nnn;
        break;

    // Execute subroutine starting at address NNN
    case 0x2000:
        chip8->stack[chip8->sp] = chip8->pc;
        chip8->sp++;
        chip8->pc = nnn;
        break;

    case 0x3000:
        if (chip8->V[X] == nn)
            chip8->pc += 2;
        break;

    case 0x4000:
        if (chip8->V[X] != nn)
            chip8->pc += 2;
        break;

    case 0x5000:
        if (chip8->V[X] == chip8->V[Y])
            chip8->pc += 2;
        break;

    case 0x6000:
        chip8->V[X] = nn;
        break;
    // 7
    case 0x7000:
        chip8->V[X] += nn;
        break;
    // 8XYN
    case 0x8000:
        switch (n)
        {
        case 0x0000:
            chip8->V[X] = chip8->V[Y];
            break;
        // 8xy1
        case 0x0001:
            chip8->V[X] |= chip8->V[Y];
            break;
        // 8xy2
        case 0x0002:
            chip8->V[X] &= chip8->V[Y];
            break;
        // 8xy3
        case 0x0003:
            chip8->V[X] ^= chip8->V[Y];
            break;
        // 8xy4
        case 0x0004:

            int i;
            i = (int)(chip8->V[X]) + (int)(chip8->V[Y]);
            if (i > 255)
                chip8->V[0xF] = 1;
            else
                chip8->V[0xF] = 0;
            chip8->V[X] = i & 0xFF;
            break;
        // 8xy5
        case 0x0005:
            if (chip8->V[X] > chip8->V[Y])
                chip8->V[0xF] = 1;
            else
                chip8->V[0xF] = 0;
            chip8->V[X] -= chip8->V[Y];
            break;
        // 8xy6
        case 0x0006:
            chip8->V[0xF] = chip8->V[X] & 1;
            chip8->V[X] >>= 1;
            break;
        // 8xy7
        case 0x0007:
            if (chip8->V[Y] > chip8->V[X])
                chip8->V[0xF] = 1;
            else
                chip8->V[0xF] = 0;
            chip8->V[X] = chip8->V[Y] - chip8->V[X];
            break;
        // 8xyE
        case 0x000E:
            chip8->V[0xF] = chip8->V[X] >> 7;
            chip8->V[X] <<= 1;
            break;

        default:
            break;
        }
        break;
    case 0x9000:
    	if(chip8->V[X] != chip8->V[Y]) chip8->pc += 2;
        break;
    // ANNN: Sets I to the address NNN
    case 0xA000:
        chip8->I = nnn;
        break;
    case 0xB000:
        chip8->pc = nnn + chip8->V[0x0];
        break;

    case 0xC000:
        chip8->V[X] = (rand() % 0x100) & (nn);
        break;

    case 0xD000:
        unsigned int xPos = chip8->V[X] % 64;
        unsigned int yPos = chip8->V[Y] % 32;

        chip8->V[0xF] = 0;

        for (int row = 0; row < n; row++)
        {
            unsigned char spriteByte = chip8->memory[chip8->I + row];
            unsigned int y = (yPos + row) % 32;

            for (int col = 0; col < 8; col++)
            {
                unsigned char spritePixel = (spriteByte >> (7 - col)) & 1;
                unsigned int x = (xPos + col) % 64;

                unsigned int index = y * 64 + x;
printf("DRAW x:%u y:%u gfx[%u]:%d -> sprite:%d\n", x, y, index, chip8->gfx[index], spritePixel);
                if (spritePixel)
                {
                    if (chip8->gfx[index] == 1)
                    {
                        chip8->V[0xF] = 1;
                    }
                    chip8->gfx[index] ^= 1;
                }
            }
        }

        chip8->drawFlag = true;
        break;
    case 0xF000:
        break;
    default:
        printf("Unknown opcode: 0x%X\n", chip8->opcode);
        break;
    }
    if (chip8->delay_timer > 0)
        chip8->delay_timer--;
    if (chip8->sound_timer > 0)
    {
        if (chip8->sound_timer == 1)
        {
            // aqui dá pra tocar beep com Raylib ou só logar
        }
        chip8->sound_timer--;
    }
}