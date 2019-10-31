#ifndef __FLECHA3_D_
#define __FLECHA3_D_

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class Flecha3 : public Module
{
public:
	Flecha3();
	~Flecha3();

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


#endif // !__FELCHA3_D_
