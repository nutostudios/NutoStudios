#ifndef __MODULEFINISH_H__
#define __MODULETFINISH_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleFinish : public Module
{
public:
	ModuleFinish(Application* app, bool start_enabled = false);
	~ModuleFinish();
	Animation* anim = nullptr;
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture * finish = nullptr;
	Animation Finish;

};
#endif