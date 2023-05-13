#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define START_ADDRESS 0x200
#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x50
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define VIDEO_SIZE 2048

#define GET_INSTRUCTION_TYPE(n) (((n) & 0xF000) >> 12)
#define GET_X(n) (((n) & 0x0F00) >> 8)
#define GET_Y(n) (((n) & 0x00F0) >> 4)
#define GET_N(n) (((n) & 0x000F))
#define GET_BYTE(n) ((n) & 0x00FF)
#define GET_ADDRESS(n) ((n) & 0x0FFF)

struct Chip8 {
	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t index;			// store memory addresses
	uint16_t PC;			// holds address of next instruction
	uint16_t stack[16];
	uint8_t SP;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t keypad[16];
	uint32_t video[VIDEO_SIZE];
	uint16_t opcode;
};

// Create instance of emulator
struct Chip8* createEmulator();

// Fetch, Decode, Execute Cycle
void Cycle(struct Chip8 *chip);

// Load ROM content into Chip8 memory
void loadRom(struct Chip8 *chip, char const *filename);

// Load fonts into memory
void loadFonts(struct Chip8 *chip);

// Generate random number between 0 and 255
uint8_t randByte();

// opcode 00E0: Clears the screen.
void OP_00E0(struct Chip8 *chip);

// opcode 00EE: Returns from a subroutine. 
void OP_00EE(struct Chip8 *chip);

// opcode 1NNN: Jumps to address NNN. 
void OP_1NNN(struct Chip8 *chip);

// opcode 2NNN: Calls subroutine at NNN. 
void OP_2NNN(struct Chip8 *chip);

// opcode 3XNN: Skips the next instruction if VX equals NN (usually the next instruction is a jump to skip a code block). 
void OP_3XNN(struct Chip8 *chip);

// opcode 4XNN: Skips the next instruction if VX does not equal NN (usually the next instruction is a jump to skip a code block).
void OP_4XNN(struct Chip8 *chip);

// opcode 5XY0: Skips the next instruction if VX equals VY (usually the next instruction is a jump to skip a code block)
void OP_5XY0(struct Chip8 *chip);

// opcode 6XNN: Sets VX to NN. 
void OP_6XNN(struct Chip8 *chip);

// opcode 7XNN: Adds NN to VX (carry flag is not changed). 
void OP_7XNN(struct Chip8 *chip);

// opcode 8XY0: Sets VX to the value of VY. 
void OP_8XY0(struct Chip8 *chip);

// opcode 8XY1: Sets VX to VX or VY. (bitwise OR operation)
void OP_8XY1(struct Chip8 *chip);

// opcode 8XY2: Sets VX to VX and VY. (bitwise AND operation) 
void OP_8XY2(struct Chip8 *chip);

// opcode 8XY3: Sets VX to VX xor VY
void OP_8XY3(struct Chip8 *chip);

// opcode 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not. 
void OP_8XY4(struct Chip8 *chip);

// opcode 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not. 
void OP_8XY5(struct Chip8 *chip);

// opcode 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
void OP_8XY6(struct Chip8 *chip);

// opcode 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not. 
void OP_8XY7(struct Chip8 *chip);

// opcode 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
void OP_8XYE(struct Chip8 *chip);

// opcode 9XY0: Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
void OP_9XY0(struct Chip8 *chip);

// opcode ANNN: Sets I to the address NNN. 
void OP_ANNN(struct Chip8 *chip);

// opcode BNNN: Jumps to the address NNN plus V0. 
void OP_BNNN(struct Chip8 *chip);

// opcode CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
void OP_CXNN(struct Chip8 *chip);

// opcode DXYN: draw(Vx, Vy, N)
void OP_DXYN(struct Chip8 *chip);

// opcode EX9E: Skips the next instruction if the key stored in VX is pressed (usually the next instruction is a jump to skip a code block).
void OP_EX9E(struct Chip8 *chip);

// opcode EXA1: Skips the next instruction if the key stored in VX is not pressed (usually the next instruction is a jump to skip a code block).
void OP_EXA1(struct Chip8 *chip);

// opcode FX07:
void OP_FX07(struct Chip8 *chip);

// opcode FX0A:
void OP_FX0A(struct Chip8 *chip);

// opcode FX15:
void OP_FX15(struct Chip8 *chip);

// opcode FX18:
void OP_FX18(struct Chip8* chip);

// opcode FX1E:
void OP_FX1E(struct Chip8* chip);

// opcode FX29:
void OP_FX29(struct Chip8 *chip);

// opcode FX33:
void OP_FX33(struct Chip8 *chip);

// opcode FX55:
void OP_FX55(struct Chip8 *chip);

// opcode FX65:
void OP_FX65(struct Chip8 *chip);