#include <iostream>
#include <chrono>
#include <thread>
#include "stdint.h"
#include <SDL2/SDL.h>

#include "chip8.h" //cpu core implementation

using namespace std;

//Keypad key map
uint8_t keymap[16] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
        SDLK_3,
	SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
	SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
};

int main(int argc, char **argv)
{
	//Command usage:
	if(argc != 2) {
		cout << "Usage: chip8 <ROM file>" << endl;
		return 1;
	}

	Chip8 chip8 = Chip8();	//Initialize Chip8

	int w = 1024;		//window width
	int h = 512;		//window height

	SDL_Window* window = NULL;

	//initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		exit(1);
	}

	//create window
	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		       								w, h, SDL_WINDOW_SHOWN);
	if(window == NULL){
		printf( "Window could not be created! SDL_Error: %s\n",
                SDL_GetError() );
		exit(2);
	}

	//create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, w, h);

	//Make texture storing frame buffer
	SDL_Texture*  sdlTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			64, 32);
	//temp pixel buffer
	uint32_t pixels[2048];

	load:
	//Attempt to load ROM
	if(!chip8.load(argv[1]))
		return 2;

	//Emulation loop
	while(true) {
		chip8.emulate_cycle();

		//process SDL events
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) exit(0);

			//process keydown events
			if(e.type == SDL_KEYDOWN) {
				if(e.key.keysym.sym == SDLK_ESCAPE)
					exit(0);

				if(e.key.keysym.sym == SDLK_F1)
					goto load;	//reload ROM

				for(int i = 0; i < 16; ++i) {
					if(e.key.keysym.sym == keymap[i]) {
						chip8.key[i] = 1;
					}
				}
			}

			//process keyup events
			if(e.type == SDL_KEYUP) {
				for(int i = 0; i < 16; ++i) {
					if(e.key.keysym.sym == keymap[i]) {
						chip8.key[i] = 0;
					}
				}
			}
		}

		//if draw occured we need to redraw the SDL screen
		if(chip8.drawFlag) {
			chip8.drawFlag = false;

			//store pixels in temp buffer
			for(int i = 0; i < 2048; ++i){
				uint8_t pixel = chip8.gfx[i];
				pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}
			SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}
}

