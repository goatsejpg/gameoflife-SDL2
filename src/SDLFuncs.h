#ifndef SDLFuncs_H
#define SDLFuncs_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Structs.h"
#include "Globals.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* fileName) {
	SDL_Surface* s = SDL_LoadBMP(fileName);
	if (s == NULL) {
		std::cerr << "SDL: BMP " << fileName << " couldn't be found." << std::endl;
	}
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	if (t == NULL) {
		std::cerr << "SDL: Surface " << fileName << "couldn't be converted to texture." << std::endl;
	} else {
		std::cerr << "SDL: BMP " << fileName << " successfuly converted to texture." << std::endl;
	}
	SDL_FreeSurface(s);
	return t;
}

void setColor (SDL_Renderer* renderer, const SDL_Color& c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0xff);
}

void renderFilledQuad (SDL_Renderer* r, const Vec2& POS, const Vec2& DIMENSION) {
	for (int y = POS.y; y < POS.y+DIMENSION.y; ++y) {
		SDL_RenderDrawLine(r, POS.x, y, POS.x+DIMENSION.x, y);
	}
}

void renderText (SDL_Renderer* r, TTF_Font* font, const char* text, const Vec2& POS, const SDL_Color& c) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, c); 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(r, textSurface);

	SDL_Rect rect = {POS.x, POS.y, 0, 0};
	TTF_SizeText(font, text, &rect.w, &rect.h);

	SDL_RenderCopy(r, texture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}

void renderTexture (SDL_Renderer* r, SDL_Texture* t, Vec2 POS, Vec2 DIMENSION) {
	SDL_Rect rect = {POS.x, POS.y, DIMENSION.x, DIMENSION.y};
	SDL_RenderCopy(r, t, NULL, &rect);
}

#endif // SDLFuncs_H
