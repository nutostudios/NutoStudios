#ifndef __FLECHA_D_
#define __FLECHA_D_

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class Flecha : public Module
{
public:
	Flecha();
	~Flecha();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* texture = nullptr;
	iPoint position;
	Animation* current_animation;
	Animation idle;
	Animation hit;
	Animation rep;
};


#endif // !__FELCHA_D_
