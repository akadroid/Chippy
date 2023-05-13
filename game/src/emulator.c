#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "emulator.h"

// Chip-8 Emulator created by Danny Huynh

// Resources
// https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#execute
// https://austinmorlan.com/posts/chip8_emulator/
// https://www.raylib.com/
// https://en.wikipedia.org/wiki/CHIP-8

struct Chip8* createEmulator()
{
	struct Chip8 *emulator = (struct Chip8*)malloc(sizeof(struct Chip8));

	memset(emulator->memory, 0, 4096 * sizeof(uint8_t));
	memset(emulator->registers, 0, 16 * sizeof(uint8_t));
	emulator->index = 0;
	emulator->PC = START_ADDRESS;
	memset(emulator->stack, 0, 16 * sizeof(uint16_t));
	emulator->SP = 0;
	emulator->delayTimer = 0;
	emulator->soundTimer = 0;
	memset(emulator->keypad, 0, 16 * sizeof(uint8_t));
	memset(emulator->video, 0, VIDEO_SIZE * sizeof(uint32_t));
	emulator->opcode = 0;

	return emulator;
}

// Fetch, Decode, Execute Cycle
void Cycle(struct Chip8 *chip)
{
	// Fetch
	uint16_t first_byte = chip->memory[chip->PC] << 8;
	uint16_t second_byte = chip->memory[chip->PC + 1];
	chip->opcode = first_byte | second_byte;

	// Prepare to fetch next instruction
	chip->PC += 2;

	// Get values from opcode
	uint16_t INSTRUCTION_TYPE = GET_INSTRUCTION_TYPE(chip->opcode);
	uint16_t X = GET_X(chip->opcode);
	uint16_t Y = GET_Y(chip->opcode);
	uint16_t N = GET_N(chip->opcode); // right most nibble
	uint16_t NN = GET_BYTE(chip->opcode);
	uint16_t NNN = GET_ADDRESS(chip->opcode);

	// Decode
	switch (INSTRUCTION_TYPE) {
	case 0x0:
		switch (N) {
		case 0x0:
			OP_00E0(chip);
			break;
		case 0xE:
			OP_00EE(chip);
			break;
		}
		break;

	case 0x1:
		printf("1\n");
		OP_1NNN(chip);
		break;

	case 0x3:
		printf("2\n");
		OP_3XNN(chip);
		break;

	case 0x4:
		printf("4\n");
		OP_4XNN(chip);
		break;

	case 0x5:
		printf("5\n");
		OP_5XY0(chip);
		break;

	case 0x6:
		printf("6\n");
		OP_6XNN(chip);
		break;

	case 0x7:
		printf("7\n");
		OP_7XNN(chip);
		break;

	case 0x8:
		printf("8");
		switch (N) {
		case 0x0:
			printf("0\n");
			OP_8XY0(chip);
			break;
		case 0x1:
			printf("1\n");
			OP_8XY1(chip);
			break;
		case 0x2:
			printf("2\n");
			OP_8XY2(chip);
			break;
		case 0x3:
			printf("3\n");
			OP_8XY3(chip);
			break;
		case 0x4:
			printf("4\n");
			OP_8XY4(chip);
			break;
		case 0x5:
			printf("5\n");
			OP_8XY5(chip);
			break;
		case 0x6:
			printf("6\n");
			OP_8XY6(chip);
			break;
		case 0x7:
			printf("7\n");
			OP_8XY7(chip);
			break;
		case 0xE:
			printf("E\n");
			OP_8XYE(chip);
			break;
		}
		break;

	case 0x9:
		printf("8\n");
		OP_9XY0(chip);
		break;

	case 0xA:
		printf("A\n");
		OP_ANNN(chip);
		break;

	case 0xB:
		printf("B\n");
		OP_BNNN(chip);
		break;

	case 0xC:
		printf("C\n");
		OP_CXNN(chip);
		break;

	case 0xD:
		printf("D\n");
		OP_DXYN(chip);
		break;

	case 0xE:
		printf("E");
		switch (N) {
		case 0xE:
			printf("E\n");
			OP_EX9E(chip);
			break;
		case 0x1:
			printf("1\n");
			OP_EXA1(chip);
			break;
		}
		break;

	case 0xF:
		printf("F");
		switch (NN) {
		case 0x07:
			printf("07\n");
			OP_FX07(chip);
			break;
		case 0x0A:
			printf("0A\n");
			OP_FX0A(chip);
			break;
		case 0x15:
			printf("15\n");
			OP_FX15(chip);
			break;
		case 0x18:
			printf("18\n");
			OP_FX18(chip);
			break;
		case 0x1E:
			printf("1E\n");
			OP_FX1E(chip);
			break;
		case 0x29:
			printf("29\n");
			OP_FX29(chip);
			break;
		case 0x33:
			printf("33\n");
			OP_FX33(chip);
			break;
		case 0x55:
			printf("55\n");
			OP_FX55(chip);
			break;
		case 0x65:
			printf("65\n");
			OP_FX65(chip);
			break;
		}
		break;

	default:
		printf("opcode error\n");
	}

	// timers
	if (chip->delayTimer > 0)
		(chip->delayTimer)--;
	if (chip->soundTimer > 0)
		(chip->delayTimer)--;
}

// Load ROM content into Chip8 memory
void loadRom(struct Chip8 *chip, const char *filename) 
{
	FILE *file;
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Error while opening file\n");
		printf("%s\n", filename);
		exit(1);
	}

	// Get size of file and create buffer
	fseek(file, 0, SEEK_END);
	long bufsize = ftell(file);
	char *buffer = (char *) calloc(bufsize + 1, sizeof(char));
	rewind(file);

	size_t len = fread(buffer, sizeof(char), bufsize, file);
	if (ferror(file) != 0) {
		fputs("Error reading file", stderr);
		exit(2);
	} else {
		buffer[len++] = '\0';
	}
	fclose(file);

	// Load the ROM contents into Chip8's memory, starting at 0x200
	for (long i = 0; i < bufsize; i++) {
		chip->memory[START_ADDRESS + i] = buffer[i];
	}
	free(buffer);
}

// Load fonts into memory
void loadFonts(struct Chip8 *chip) 
{
	for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
		chip->memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
}

// Random number between 0 and 255
uint8_t randByte()
{
	return (uint8_t) (rand() % 256);
}

// opcode 00E0: CLS
void OP_00E0(struct Chip8 *chip)
{
	memset(chip->video, 0, VIDEO_SIZE*sizeof(uint32_t));
}

// opcode 00EE: RET
void OP_00EE(struct Chip8 *chip)
{
	--(chip->SP);
	chip->PC = chip->stack[chip->SP];
}

void OP_1NNN(struct Chip8 *chip)
{
	uint16_t address = GET_ADDRESS(chip->opcode);
	chip->PC = address;
}

void OP_2NNN(struct Chip8 *chip)
{
	uint16_t address = GET_ADDRESS(chip->opcode);
	chip->stack[chip->SP] = chip->PC;
	(chip->SP)++;
	chip->PC = address;
}

void OP_3XNN(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t byte = GET_BYTE(chip->opcode);

	// skip instruction
	if (chip->registers[x] == byte)
		chip->PC += 2;
}

void OP_4XNN(struct Chip8 *chip)
{
	uint8_t byte = GET_BYTE(chip->opcode);
	uint8_t x = GET_X(chip->opcode);

	// skip instruction
	if (chip->registers[x] != byte)
		chip->PC += 2;
}

void OP_5XY0(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	// skip instruction
	if (chip->registers[x] == chip->registers[y])
		chip->PC += 2;
}

void OP_6XNN(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t byte = chip->opcode & 0x00FF;
	chip->registers[x] = byte;
}

void OP_7XNN(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t byte = GET_BYTE(chip->opcode);

	chip->registers[x] += byte;
}

void OP_8XY0(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	chip->registers[x] = chip->registers[y];
}

void OP_8XY1(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	chip->registers[x] |= chip->registers[y];
}

void OP_8XY2(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	chip->registers[x] &= chip->registers[y];
}

void OP_8XY3(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	chip->registers[x] ^= chip->registers[y];
}

void OP_8XY4(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);
	uint16_t sum = chip->registers[x] + chip->registers[y];

	if (sum > 255)
		chip->registers[0xF] = 1;
	else
		chip->registers[0xF] = 0;

	// Keep only the right most 8 bits
	chip->registers[x] = sum & 0xFF; 
}

void OP_8XY5(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	if (chip->registers[x] > chip->registers[y])
		chip->registers[0xF] = 1;
	else
		chip->registers[0xF] = 0;

	chip->registers[x] -= chip->registers[y];	
}

void OP_8XY6(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);

	// Store least signicant bit of VX in VF
	chip->registers[0xF] = chip->registers[x] & 1;

	chip->registers[x] >>= 1;
}

void OP_8XY7(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	if (chip->registers[y] > chip->registers[x])
		chip->registers[0xF] = 1;
	else
		chip->registers[0xF] = 0;
	
	chip->registers[x] = chip->registers[y] - chip->registers[x];
}

void OP_8XYE(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);

	// Store most significant bit of VX in VF
	chip->registers[0xF] = (chip->registers[x] & (1 << 7)) >> 7;

	chip->registers[x] <<= 1;
}

void OP_9XY0(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);

	if (chip->registers[x] != chip->registers[y])
		chip->PC += 2;
}

void OP_ANNN(struct Chip8 *chip)
{
	uint16_t address = GET_ADDRESS(chip->opcode);
	chip->index = address;
}

void OP_BNNN(struct Chip8 *chip)
{
	uint16_t address = GET_ADDRESS(chip->opcode);
	chip->PC = address + chip->registers[0];
}

void OP_CXNN(struct Chip8 *chip)
{
	uint8_t ran_num = randByte();
	uint8_t x = GET_X(chip->opcode);
	uint8_t byte = GET_BYTE(chip->opcode);
	chip->registers[x] =  byte & ran_num;
}

void OP_DXYN(struct Chip8 *chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t y = GET_Y(chip->opcode);
	uint8_t height = GET_N(chip->opcode);

	uint8_t x_coord = chip->registers[x] % VIDEO_WIDTH;
	uint8_t y_coord = chip->registers[y] % VIDEO_HEIGHT;
	chip->registers[0xF] = 0;

	for (unsigned int row = 0; row < height; row++) {
		uint8_t sprite_data = chip->memory[chip->index + row];

		for (unsigned int col = 0; col < 8; col++) {
			uint8_t sprite_pixel = sprite_data & (0x80 >> col);
			uint32_t screen_pixel = chip->video[(x_coord + col) + (y_coord + row)*VIDEO_WIDTH];

			if (sprite_pixel) {
				if (screen_pixel == 0xFFFFFFFF)
					chip->registers[0xF] = 1;

				chip->video[(x_coord + col) + (y_coord + row)*VIDEO_WIDTH] ^= 0xFFFFFFFF;
			}
		}
	}
}

void OP_EX9E(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	if (chip->keypad[chip->registers[x]])
		chip->PC += 2;
}

void OP_EXA1(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	if (!chip->keypad[chip->registers[x]])
		chip->PC += 2;
}

void OP_FX07(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	chip->registers[x] = chip->delayTimer;
}

void OP_FX0A(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	
	if (chip->keypad[0x0])
		chip->registers[x] = 0x0;

	else if (chip->keypad[0x1])
		chip->registers[x] = 0x1;

	else if (chip->keypad[0x2])
		chip->registers[x] = 0x2;

	else if (chip->keypad[0x3])
		chip->registers[x] = 0x3;

	else if (chip->keypad[0x4])
		chip->registers[x] = 0x4;

	else if (chip->keypad[0x5])
		chip->registers[x] = 0x5;

	else if (chip->keypad[0x6])
		chip->registers[x] = 0x6;

	else if (chip->keypad[0x7])
		chip->registers[x] = 0x7;

	else if (chip->keypad[0x8])
		chip->registers[x] = 0x8;

	else if (chip->keypad[0x9])
		chip->registers[x] = 0x9;

	else if (chip->keypad[0xA])
		chip->registers[x] = 0xA;

	else if (chip->keypad[0xB])
		chip->registers[x] = 0xB;

	else if (chip->keypad[0xC])
		chip->registers[x] = 0xC;

	else if (chip->keypad[0xD])
		chip->registers[x] = 0xD;

	else if (chip->keypad[0xE])
		chip->registers[x] = 0xE;

	else if (chip->keypad[0xF])
		chip->registers[x] = 0xF;

	else
		chip->PC -= 2;
}

void OP_FX15(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	chip->delayTimer = chip->registers[x];
}

void OP_FX18(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	chip->soundTimer = chip->registers[x];
}

void OP_FX1E(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	chip->index += chip->registers[x];
}

void OP_FX29(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t character = chip->registers[x];

	chip->index = FONTSET_START_ADDRESS + (character * 5);
}

void OP_FX33(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	uint8_t num = chip->registers[x];
	
	uint8_t ones = num % 10;
	num /= 10;
	uint8_t tens = num % 10;
	num /= 10;
	uint8_t hundreds = num % 10;

	chip->memory[chip->index] = hundreds;
	chip->memory[chip->index+1] = tens;
	chip->memory[chip->index + 2] = ones;
}

void OP_FX55(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);
	
	for (int i = 0; i <= x; i++) {
		chip->memory[chip->index + i] = chip->registers[i];
	}
}

void OP_FX65(struct Chip8* chip)
{
	uint8_t x = GET_X(chip->opcode);

	for (int i = 0; i <= x; i++) {
		chip->registers[i] = chip->memory[chip->index + i];
	}
}

uint8_t fontset[FONTSET_SIZE] = {
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