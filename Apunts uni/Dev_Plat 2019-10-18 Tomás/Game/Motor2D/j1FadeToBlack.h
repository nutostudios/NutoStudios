#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__
#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class FadeToBlack : public j1Module {
public:
	FadeToBlack();
	~FadeToBlack();
	bool Start();
	bool Update;
	bool FadingToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);

private:
	enum fade_step {
		none,
		fade_to_black,
		fade_from_black
	}
	
	current_step = fade_step::none;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;
};

#endif //__MODULEFADETOBLACK_H__