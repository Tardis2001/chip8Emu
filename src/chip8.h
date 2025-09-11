
#include <stdbool.h>
#include <time.h>
typedef struct
{
    unsigned short int opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char gfx [(64 * 32) * 10];
    unsigned char key[16];
    unsigned char  delay_timer;		
	unsigned char  sound_timer;		
    bool drawFlag;
} Chip8;

void initialize(Chip8 *chip8);
void loadGame(char title[],Chip8 *chip8);
void emulateCycle(Chip8 *chip8);
