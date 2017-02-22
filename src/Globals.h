#ifndef GLOBALS_H
#define GLOBALS_H

#include "Structs.h"
#include <SDL2/SDL_pixels.h>

const unsigned short WIDTH=800, HEIGHT=600;
signed char SPEED = 6;
const Vec2 MATRIXSIZE = {80,50};
bool MATRIX[WIDTH/10][(HEIGHT-100)/10];
bool MATRIXNEXT[WIDTH/10][(HEIGHT-100)/10];
const SDL_Color 
WHITE  = {255,255,255}, 
GREY1  = {180,180,180}, 
GREY2  = {90 , 90, 90}, 
BLACK  = {0x0,0x0,0x0}, 
RED    = {255,0x0,0x0}, 
GREEN  = {0x0,255,0x0}, 
BLUE   = {0x0,0x0,255}, 
YELLOW = {255,255,0x0}, 
MAGENTA= {255,0x0,255}, 
CYAN   = {0x0,255,255};


#endif // GLOBALS_H
