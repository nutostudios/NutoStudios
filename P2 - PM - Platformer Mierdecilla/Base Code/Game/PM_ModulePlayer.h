#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

//PM
#define JUMP_TIME 3000
//

struct SDL_Texture;
struct Collider;


//PM
enum states											//PM
{
	ST_UNKNOWN,

	ST_IDLE_LEFT,
	ST_IDLE_RIGHT,
	ST_WALK_LEFT,
	ST_WALK_RIGHT,
	ST_JUMP_LEFT,
	ST_JUMP_RIGHT,
	ST_FALL_LEFT,				//POTENCIALMENTE 1
	ST_FALL_RIGHT,				//POTENCIALMENTE 1
	ST_DIE_LEFT,
	ST_DIE_RIGHT,
	ST_WIN,
	ST_HURT,

};
//

//PM
struct InputPlayer											//PM
{
	bool A_DOWN;
	bool D_DOWN;
	bool W_DOWN;

};
//

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation up;
	Animation down;

	Animation idlePM_L;											//PM
	Animation walkPM_L;											//PM
	Animation jumpPM_L;											//PM
	Animation diePM_L;											//PM
	Animation fallPM_L;											//PM
	Animation hurtPM_L;											//PM	//mismo sprite que die, solo que solo se mueve horizontalmente

	Animation idlePM_R;											//PM
	Animation walkPM_R;											//PM
	Animation jumpPM_R;											//PM
	Animation diePM_R;											//PM
	Animation fallPM_R;											//PM
	Animation hurt_R;											//PM	//mismo sprite que die, solo que solo se mueve horizontalmente

	Animation winPM;											//PM

	iPoint Player;												//PM
	states currentstate;										//PM
	InputPlayer inputplayer;									//PM


	iPoint position;
	Collider* col;
	bool destroyed = false;

	//PM
	bool collided = false;
	bool gmode = false;
	//

};

#endif