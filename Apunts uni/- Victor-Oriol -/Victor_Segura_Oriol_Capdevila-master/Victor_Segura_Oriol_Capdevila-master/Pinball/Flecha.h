#ifndef __FLECHA_H__
#define __FLECHA_H__

#include "Cosa.h"

class Flecha : public Cosa
{
private:
	iPoint original_pos;
	Animation hit;

public:

	Flecha(int x, int y, SDL_Texture* thistexture);

	void Move();
};

#endif // __ENEMY_BROWNSHIP_H__