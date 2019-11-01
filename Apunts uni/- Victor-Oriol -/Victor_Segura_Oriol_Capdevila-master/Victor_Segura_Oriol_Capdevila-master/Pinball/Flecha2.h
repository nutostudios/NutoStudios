#ifndef __FLECHA2_D_
#define __FLECHA2_D_

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class Flecha2 : public Module
{
public:
	Flecha2();
	~Flecha2();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture * texture = nullptr;
	iPoint position;
	Animation* current_animation;
	Animation idle;
	Animation hit;
	Animation rep;
};


#endif // !__FELCHA2_D_
