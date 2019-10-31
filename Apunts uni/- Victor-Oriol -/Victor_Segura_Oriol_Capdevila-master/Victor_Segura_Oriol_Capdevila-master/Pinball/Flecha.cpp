#include "Application.h"
#include "Globals.h"
#include "Flecha.h"

Flecha::Flecha(int x, int y, SDL_Texture* thistexture) : Cosa(x, y)
{
	tex = thistexture;
}