#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdint.h>

class Chip8 {
private:
	uint16_t stack[16];	//16 short int stack
	uint16_t sp;		//Stack Pointer

	uint8_t memory[4096];	//4k memory
	uint8_t V[16];		//V registers (v0 - VF)

	uint16_t pc;		//program counter
	uint16_t opcode;	//Current op code
	uint16_t I;		//Index register

	uint8_t delay_timer;	//Delay Timer
	uint8_t sound_timer;	//Sound Timer

	void init();
	
public:
		
	uint8_t gfx[2048];	//graphics buffer
	uint8_t key[16];	//keypad
	bool drawFlag;		//indicates draw has occurred

	Chip8();
	~Chip8();

	void emulate_cycle();	//emulate one cycle
	bool load(const char *file_path);	//load applicaiton;
};

#endif //CHIP_8_H
