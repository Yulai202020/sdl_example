#pragma once

#include "structs.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Animation {
public:
	int currentIndex;
	Animation();
	~Animation();
	void showAnimation(animationProperties animation, SDL_FRect& src, int sizeSprite);
	bool showAnimationOneTime(animationProperties animation, SDL_FRect& src, int sizeSprite);
	bool reverseShowAnimation(animationProperties animation, SDL_FRect& src, int sizeSprite);
	
	void setCurrentIndex(int index) { currentIndex = index; };
private:
	int currentReverseIndex = -2;
	Uint64 lastUpdate;
};

