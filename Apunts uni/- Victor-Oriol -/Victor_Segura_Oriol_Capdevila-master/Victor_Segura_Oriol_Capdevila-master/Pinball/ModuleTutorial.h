#ifndef __MODULETUTORIAL_H__
#define __MODULETUTORIAL_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleTutorial : public Module
{
public:
	ModuleTutorial(Application* app, bool start_enabled = true);
	~ModuleTutorial();
	Animation* anim = nullptr;
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * tuto = nullptr;
	Animation Tutorial;

};

#endif
