#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 11

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleParticles;
class ModuleSceneSpace;
class ModuleSceneIntro;
class ModulePlayer;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleEnemies;
class Module;

/*
//PM
class PM_ModuleSceneLevel1;											//PM
class PM_ModuleEnemies;												//PM
class PM_ModulePlatform;											//PM
class PM_ModulePlayer;												//PM
*/


class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleSceneSpace* scene_space;
	ModuleSceneIntro* scene_intro;
	ModuleCollision* collision;
	ModulePlayer* player;
	ModuleFadeToBlack* fade;
	ModuleParticles* particles;
	ModuleEnemies* enemies;

	/*
	//PM
	PM_ModuleSceneLevel1* PM_scenelevel1;									//PM
	PM_ModuleEnemies* PM_enemies;											//PM
	PM_ModulePlatform* PM_platform;											//PM
	PM_ModulePlayer* PM_player;												//PM
	*/


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__