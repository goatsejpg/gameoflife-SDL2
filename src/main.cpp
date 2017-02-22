#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <unistd.h>
#include <time.h>

#include "Structs.h"
#include "Globals.h"
#include "SDLFuncs.h"
#include "Logic.h"
#include "Buttons.h"

const int FPSlimit = 60;
const int FPSsleep = 1000000 / FPSlimit;

int main (int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL: Video couldn't be initialized." << std::endl;
	}
	SDL_Window*     window = SDL_CreateWindow("Game Of Life",
	                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                         WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,
	                         SDL_RENDERER_ACCELERATED);
	SDL_Event        event;

	if (TTF_Init() == -1) {
		std::cerr << "SDL: TTF couldn't be initalized." << std::endl;
	}

	TTF_Font* OpenSans = TTF_OpenFont("../resources/fonts/OpenSans-Regular.ttf", 10);
	TTF_Font* Clock    = TTF_OpenFont("../resources/fonts/clock.ttf", 64);

	std::cerr << "SDL: Setup finished" << std::endl; 

	bool playing = true;
	Vec2 lastCellDrawn = Vec2(0,0);
	Vec2INT mouseXY = Vec2INT(0,0);
	bool draw    =false;
	bool firstCellDrawn;
	bool PAUSED = true;
	unsigned char count;

	bool drawFrame = true;

	struct timespec start, end;
	unsigned int deltaTime; // micro seconds

	Display display(Vec2(105,510), renderer, &SPEED, "DELAY", OpenSans, Clock);

	Button* buttons[4];

	SpeedButton minusSpeed(Vec2INT(10,510), 
	loadTexture(renderer,"../resources/bmp/minusbuttonpressed.bmp"),
	loadTexture(renderer,"../resources/bmp/minusbutton.bmp"),
	-1, &SPEED, Vec2INT(80,80)
	);

	SpeedButton plusSpeed(Vec2INT(262,510), 
	loadTexture(renderer,"../resources/bmp/plusbuttonpressed.bmp"),
	loadTexture(renderer,"../resources/bmp/plusbutton.bmp"),
	1, &SPEED, Vec2INT(80,80)
	);

	Pause pauseButton(Vec2INT(705,510),
	loadTexture(renderer,"../resources/bmp/pausebuttonpressed.bmp"),
	loadTexture(renderer,"../resources/bmp/pausebutton.bmp"),
	&PAUSED, Vec2INT(80,80)
	);

	ResetButton reset(Vec2INT(380, 510),
	loadTexture(renderer,"../resources/bmp/resetbuttonpressed.bmp"),
	loadTexture(renderer,"../resources/bmp/resetbutton.bmp"),
	Vec2INT(120,80));

	buttons[0] = &minusSpeed; buttons[1] = &plusSpeed;
	buttons[2] = &pauseButton; buttons[3] = &reset;

	gameoflife::renderMatrix(renderer);

	display.render(renderer);
	for (auto B : buttons) {
		B->render(renderer);
	}	
	SDL_RenderPresent(renderer);
	SDL_RenderPresent(renderer);

	while (playing) {

		clock_gettime(CLOCK_MONOTONIC, &start);
		drawFrame = false;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				playing = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					SDL_GetMouseState(&mouseXY.x, &mouseXY.y);
			
					firstCellDrawn = gameoflife::getCell(
					gameoflife::checkCellClicked(mouseXY));

					for (auto B : buttons) {
						if (B->checkPress(mouseXY)) {
							B->action();
							drawFrame = true;
							break;
						}
					}

					gameoflife::invertCell(
					gameoflife::checkCellClicked(mouseXY));

					if (mouseXY != Vec2(-1, -1)) {
						draw = true;
						drawFrame = true;
					}
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					draw = false;
					
					for (auto B : buttons) {
						B->pressed(false);
					}
					drawFrame = true;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseXY.x, &mouseXY.y);
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					PAUSED = !PAUSED;
					break;
				}
				break;
			}
		}

		if (PAUSED == false && count >= SPEED) {
			gameoflife::updateNextMatrix();
			gameoflife::nextGeneration();
			count = 0;
			drawFrame = true;
		}

		if (draw) {
			Vec2 temp = gameoflife::checkCellClicked(mouseXY);
			if (temp != lastCellDrawn && firstCellDrawn == gameoflife::getCell(temp)) {
				gameoflife::invertCell(temp);
				MATRIXNEXT[temp.x][temp.y] = !firstCellDrawn;
				drawFrame = true;
			}
			lastCellDrawn = temp;
		}

		if (drawFrame) {
			gameoflife::renderMatrix(renderer);

			display.render(renderer);
			for (auto B : buttons) {
				B->render(renderer);
			}
			SDL_RenderPresent(renderer);
		}

		count++;

		clock_gettime(CLOCK_MONOTONIC, &end);
		deltaTime = (end.tv_nsec - start.tv_nsec)/1000;
		if (deltaTime < FPSsleep) {
			usleep (FPSsleep - deltaTime);
		}
	}

	return 0;
}
