#ifndef LOGIC_H
#define LOGIC_H

#include <algorithm>
#include "Globals.h"
#include "SDLFuncs.h"

namespace gameoflife {

void setNextState (const Vec2& V) {
	unsigned char* sum = new unsigned char;
	*sum = 0;
	for (short x = std::max((short)(V.x-1), (short)0);
	           x < std::min((short)(V.x+2), MATRIXSIZE.x); ++x) {
		for (short y = std::max((short)(V.y-1), (short)0); 
	                   y < std::min((short)(V.y+2), MATRIXSIZE.y); ++y) {

			if (!(x == V.x && y == V.y)) {
				*sum += MATRIX[x][y];
			}
		}
	}
	if (MATRIX[V.x][V.y]) {
		if (*sum < 2 || *sum > 3) {
			MATRIXNEXT[V.x][V.y] = false;
		} else {
			MATRIXNEXT[V.x][V.y] = true;
		}
	} else {
		if (*sum == 3) {
			MATRIXNEXT[V.x][V.y] = true;
		} else {
			MATRIXNEXT[V.x][V.y] = false;
		}
	}
	delete sum;
}

void updateNextMatrix() {
	for (std::size_t x = 0; x < MATRIXSIZE.x; ++x) {
		for (std::size_t y = 0; y < MATRIXSIZE.y; ++y) {
			setNextState(Vec2(x,y));
		}
	}
}

void nextGeneration () {
	for (std::size_t x = 0; x < MATRIXSIZE.x; ++x) {
		for (std::size_t y = 0; y < MATRIXSIZE.y; ++y) {
			MATRIX[x][y] = MATRIXNEXT[x][y];
		}
	}
}

void invertCell (const Vec2& C) {
	MATRIX[C.x][C.y] = !MATRIX[C.x][C.y];
}

void renderMatrix (SDL_Renderer* r) {
	setColor(r, GREY1);
	renderFilledQuad(r, Vec2(0,0),Vec2(WIDTH,HEIGHT));
	for (std::size_t x = 0; x != MATRIXSIZE.x; ++x) {
		for (std::size_t y = 0; y != MATRIXSIZE.y; ++y) {
			if (MATRIX[x][y]) {
				setColor(r, YELLOW);
			} else {
				setColor(r,  GREY2);
			}
			renderFilledQuad(r, Vec2(x*10+1,y*10+1),Vec2(8,9));
		}
	}
}

Vec2 checkCellClicked (const Vec2INT& Vi) {
	if (Vi.y < MATRIXSIZE.y*10) {
		return Vec2(Vi.x/(short)10, Vi.y/(short)10);
	} else {
		return Vec2(-1, -1);
	}
}

bool getCell (const Vec2 V) {
	return MATRIX[V.x][V.y];
}

}

#endif // LOGIC_H
