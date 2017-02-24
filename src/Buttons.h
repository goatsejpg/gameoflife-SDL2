#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Structs.h"
#include "SDLFuncs.h"
#include "Globals.h"

using Vi = const Vec2INT&;
using ST = SDL_Texture*;

class Button {
public:
	Button(Vi P, ST press, ST regular, Vi D = Vec2INT(80,80)
	): _POS(P), _DIMENSION(D), _pressedTexture(press),
	_Texture(regular) {}

	Vec2INT POS() {return _POS;}
	void POS(Vec2INT i) {_POS = i;}
	Vec2INT DIMENSION() {return _DIMENSION;}
	void DIMENSION(Vec2INT i) {_DIMENSION = i;}
	bool pressed () {return _pressed;}
	void pressed (bool i) {_pressed = i;}
	bool checkPress(Vec2INT mouse) {
		if ((_POS.x <= mouse.x) && 
		       (_POS.x+_DIMENSION.x >= mouse.x) &&
		       (_POS.y <= mouse.y) && 
		       (_POS.y+_DIMENSION.y >= mouse.y)) {
			_pressed = true;
			return true;
		} else {
			return false;
		}
	}

	virtual void action() = 0;
	virtual void render(SDL_Renderer* r) = 0;

	virtual ~Button() {
		std::cerr << "Destroying button at " << this << std::endl;
		SDL_DestroyTexture(_pressedTexture);
		SDL_DestroyTexture(_Texture);
	}
protected:
	bool _pressed = false;
	Vec2INT _POS;
	Vec2INT _DIMENSION;
	SDL_Texture* _pressedTexture, *_Texture;
};

class Pause : public Button {
public:
	Pause(Vi P, ST pressed, ST regular, bool* p, Vi D = Vec2INT(80,80)
	): Button(P,pressed,regular,D), pauseValue(p) {}

	void action() {
		*pauseValue = !(*pauseValue);
	}
	void render(SDL_Renderer* r) {	
		if (*pauseValue) {
			renderTexture(r, _pressedTexture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		} else {
			renderTexture(r, _Texture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		}
	}
	virtual ~Pause() = default;
private:
	bool* pauseValue;
};

class SpeedButton : public Button {
public:
	SpeedButton(Vi P, ST pressed, ST regular, char d, signed char* p, Vi D = Vec2INT(80,80)
	): Button(P,pressed,regular,D), speedValue(p), difference(d) {}

	void action() {
		*speedValue += difference;
		if (*speedValue < 0) {
			*speedValue = 0;
		} else {
			if (*speedValue > 32) {
				*speedValue = 32;
			}
		}
	}
	void render(SDL_Renderer* r) {	
		if (_pressed) {
			renderTexture(r, _pressedTexture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		} else {
			renderTexture(r, _Texture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		} 
	}
	
	virtual ~SpeedButton() = default;
private:
	char difference;
	signed char* speedValue;
};

class ResetButton : public Button {
public:
	ResetButton(Vi P, ST pressed, ST regular, Vi D = Vec2INT(120,80)): Button(P,pressed,regular,D){}
	
	void action() {
		for (std::size_t x = 0; x < MATRIXSIZE.x; ++x) {
			for (std::size_t y = 0; y < MATRIXSIZE.y; ++y) {
				MATRIX[x][y] = false;
				MATRIXNEXT[x][y] = false;
			}
		}
	}
	void render(SDL_Renderer* r) {	
		if (_pressed) {
			renderTexture(r, _pressedTexture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		} else {
			renderTexture(r, _Texture, Vec2INTtoVec2(_POS), Vec2INTtoVec2(_DIMENSION));
		} 
	}
	virtual ~ResetButton() = default;
};

class Display {
public:
	Display(const Vec2& P, SDL_Renderer* r, signed char* vp, std::string displayN, TTF_Font* nf, TTF_Font* f): value(vp),
	_POS(P), displayName(displayN), font(f), nameFont(nf) {
		texture = loadTexture(r, "../resources/bmp/display.bmp");
	}

	Vec2 POS() {return _POS;}
	void POS(Vec2 i) {_POS = i;}
	Vec2 DIMENSION() {return _DIMENSION;}
	void DIMENSION(Vec2 i) {_DIMENSION = i;}

	void render (SDL_Renderer* r) {
		renderTexture(r, texture, _POS, _DIMENSION);
		renderText(r, font, std::to_string(*value).c_str(),Vec2(_POS.x+15,_POS.y+15),BLACK);
		renderText(r, nameFont, displayName.c_str(), Vec2(_POS.x+15, _POS.y+3),BLACK);
	}

	virtual ~Display () {
		std::cerr << "Destroying display at " << this << std::endl;
		SDL_DestroyTexture(texture);
	}
private:
	Vec2 _POS;
	Vec2 _DIMENSION = Vec2(142,80);
	signed char* value;
	std::string displayName;
	SDL_Texture* texture;
	TTF_Font* font;
	TTF_Font* nameFont;
};

#endif // BUTTONS_Hi
