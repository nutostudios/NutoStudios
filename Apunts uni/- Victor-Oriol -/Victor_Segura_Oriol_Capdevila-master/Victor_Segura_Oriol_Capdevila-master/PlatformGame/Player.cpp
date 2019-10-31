#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "EntityManager.h"
#include "j1Timer.h"

#include "Player.h"

#define PLAYER_SPEED 40.0f
#define JUMP_SPEED 40.0f
#define GRAVITY 60.0f


Player::Player(int x, int y) : Entity(x, y) {

	bool ret = true;
	player_appeared = false;

	//TODO : this looks ugly but it seems the only way :'c
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadConfig(config_file); //todo: make a method to get the root without passing the xml_document
	node = &node->child("entities").child("player");

	//read fxs from node
	player_jump_fx = App->audio->LoadFx(node->child("fxPlayerJump").attribute("name").as_string());
	player_dead_fx = App->audio->LoadFx(node->child("fxPlayerDead").attribute("name").as_string());
	player_life_reward_fx = App->audio->LoadFx(node->child("fxPlayerLifeReward").attribute("name").as_string());

	//read animation from node
	for (pugi::xml_node animations = node->child("animations").child("animation"); animations && ret; animations = animations.next_sibling("animation"))
	{
		p2SString tmp(animations.attribute("name").as_string());

		if (tmp == "right_idle")
			LoadAnimation(animations, &right_idle);
		else if (tmp == "left_idle")
			LoadAnimation(animations, &left_idle);
		else if (tmp == "right_run")
			LoadAnimation(animations, &right_run);
		else if (tmp == "left_run")
			LoadAnimation(animations, &left_run);
		else if (tmp == "right_jump")
			LoadAnimation(animations, &right_jump);
		else if (tmp == "left_jump")
			LoadAnimation(animations, &left_jump);
		else if (tmp == "right_dead")
			LoadAnimation(animations, &right_death_blink);
		else if (tmp == "left_dead")
			LoadAnimation(animations, &left_death_blink);
		else if (tmp == "right_shoot")
			LoadAnimation(animations, &right_shoot);
		else if (tmp == "left_shoot")
			LoadAnimation(animations, &left_shoot);

	}

	if (lives_implement_load != 0)
		p_lives = lives_implement_load;
	if (score_implement_load != 0)
		score = score_implement_load;
	if (time_implement_load != 0)
		timer = time_implement_load;

	LOG("Creating player collider");
	collider = App->collision->AddCollider({ 0, 0, 16, 12 }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entitymanager);

	if (collider == nullptr)
		LOG("Error adding player collider");

	//todo: need to set it? the scene does it already

	animation = &right_idle;
	LOG("TIMER: %i", timer);
}

void Player::LoadAnimation(pugi::xml_node animation_node, Animation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

Player::~Player()
{
	LOG("Freeing the player");

	LOG("Unloading player sound fx");
	lives_implement_load = p_lives;
	score_implement_load = score;
	time_implement_load = timer;
}

// Called each loop iteration
void Player::Update(float dt)
{
	if (player_appeared == false)
	{
		real_timer.Start();
		player_appeared = true;
		timer = real_timer.Read();
		LOG("TIMER: %i", timer);
	}

	timer = real_timer.Read();
	//LOG("timer: %i", timer);
	current_dt = dt;
	current_state = PlayerState::ST_UNKNOWN;

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

	if (!isDead)
	{ //MOVEMENT / GRAVITY FUNCTIONALITY

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
			god_mode = !god_mode;
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			god_mode_fly = !god_mode_fly;

			if (!god_mode_fly)
				god_mode = false;
		}


		if (god_mode_fly && !god_mode)
			god_mode = true;


		if (score % 10 == 0 && score != last_score && App->entitymanager->GetPlayer()->p_lives != 6)
		{
			App->entitymanager->GetPlayer()->p_lives++;
			App->audio->PlayFx(player_life_reward_fx);
		}

		last_score = score;

		if (!god_mode_fly)
		{

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !didDoubleJump)
			{
				if (!isJumping)
				{
					App->audio->PlayFx(player_jump_fx);
					isJumping = true;
				}
				else
				{
					App->audio->PlayFx(player_jump_fx);
					didDoubleJump = true;
				}
				jumpTimer = SDL_GetTicks();
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				this->position.x += canGoRight();
				SetPlayerStates(ST_RUN_RIGHT, LAST_ST_RUN_RIGHT);
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				this->position.x -= canGoLeft();
				SetPlayerStates(ST_RUN_LEFT, LAST_ST_RUN_LEFT);
			}

			bool isGettingHigh = false;


			if (currentTime <= jumpTimer + 500 && isJumping)
				isGettingHigh = true;

			if (!isGettingHigh)
			{
				if (float gravity = gravityHaveToWork())
				{
					this->position.y += gravity;
					isJumping = true;
				}
			}

			if (isJumping && isGettingHigh)
				this->position.y -= canGoUp();
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				SetPlayerStates(ST_IDLE_RIGHT, LAST_ST_IDLE_RIGHT);
				this->position.x += ceil(PLAYER_SPEED * current_dt);
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				SetPlayerStates(ST_IDLE_LEFT, LAST_ST_IDLE_LEFT);
				this->position.x -= ceil(PLAYER_SPEED * current_dt);
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				this->position.y -= ceil(PLAYER_SPEED * current_dt);
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				this->position.y += ceil(PLAYER_SPEED * current_dt);

		}

	}


	//SEARCH THE STATE AND SET THE ANIMATION
	SetPlayerAnimation(current_state, last_state);

	//DEAD ANIMATION WITH TIMER
	if (isDead && currentTime < deadTime + 1000)
	{
		if (last_state == LAST_ST_RUN_RIGHT)
			animation = &right_death_blink;
		else
			animation = &left_death_blink;
	}
	else if (isDead)
	{
		p_lives--;
		isDead = false;
		collider->type = COLLIDER_PLAYER;
	}

	// Draw everything --------------------------------------
	SDL_Rect r = animation->GetCurrentFrame(dt);

	// Update player collider
	collider->SetPos(position.x, position.y - r.h);
	collider->rect.w = r.w;
	collider->rect.h = r.h;

	/*
	if (!App->render->Blit(graphics, (int)position.x, (int)position.y - r.h, &r)) {
	LOG("Cannot blit the texture in j1Player %s\n", SDL_GetError());
	}
	*/

	currentTime = SDL_GetTicks();
}

void Player::OnCollision(Collider* collider) {

}

float Player::gravityHaveToWork()
{

	fPoint tmpPosLeft;
	tmpPosLeft.x = position.x;
	tmpPosLeft.y = position.y - 1;


	fPoint tmpPosRight;
	tmpPosRight.x = position.x + collider->rect.w - 1;
	tmpPosRight.y = position.y - 1;



	return ceil(GRAVITY*current_dt);
}

float Player::canGoLeft()
{
	bool ret = true;

	fPoint tmpPosUp;
	tmpPosUp.x = position.x;
	tmpPosUp.y = position.y - collider->rect.h;


	fPoint tmpPosDown;
	tmpPosDown.x = position.x;
	tmpPosDown.y = position.y - 1;


	return ceil(PLAYER_SPEED*current_dt);
}

float Player::canGoUp()
{
	bool ret = true;

	fPoint tmpPosLeft;
	tmpPosLeft.x = position.x;
	tmpPosLeft.y = position.y - collider->rect.h;

	

	fPoint tmpPosRight;
	tmpPosRight.x = position.x + collider->rect.w - 1;
	tmpPosRight.y = position.y - collider->rect.h;


	return ceil(JUMP_SPEED*current_dt);
}

bool Player::PlayerCanShootRight()
{
	return (last_state == PlayerLastState::LAST_ST_RUN_RIGHT
		|| last_state == PlayerLastState::LAST_ST_IDLE_RIGHT
		|| last_state == PlayerLastState::LAST_ST_SHOOT_RIGHT);
}

bool Player::PlayerCanShootLeft()
{
	return (last_state == PlayerLastState::LAST_ST_RUN_LEFT
		|| last_state == PlayerLastState::LAST_ST_IDLE_LEFT
		|| last_state == PlayerLastState::LAST_ST_SHOOT_LEFT);
}

float Player::canGoRight()
{
	fPoint tmpPosUp;
	tmpPosUp.x = position.x + collider->rect.w - 1;
	tmpPosUp.y = position.y - collider->rect.h;


	fPoint tmpPosDown;
	tmpPosDown.x = position.x + collider->rect.w - 1;
	tmpPosDown.y = position.y - 1;


	return ceil(PLAYER_SPEED*current_dt);
}

float Player::DistanceToWall(SDL_Rect wall, SDL_Rect player, Direction direction)
{
	switch (direction)
	{
	case Direction::RIGHT:
		return player.x + player.w - wall.x;
		break;
	case Direction::LEFT:
		return wall.x + wall.w - player.x;
		break;
	case Direction::UP:
		return player.y - wall.y;
		break;
	case Direction::DOWN:
		return player.y + player.h - wall.y;
		break;
	case Direction::NO_DIR:
	default:
		LOG("YOU ARE DOING IT WRONG!");
		break;
	}
}


void Player::SetPlayerAnimation(PlayerState current_state, PlayerLastState last_state)
{
	switch (current_state)
	{
	case Player::ST_UNKNOWN:
		switch (last_state)
		{
		case Player::LAST_ST_UNKNOWN:
			break;
		case Player::LAST_ST_IDLE_RIGHT:
			break;
		case Player::LAST_ST_IDLE_LEFT:
			break;
		case Player::LAST_ST_RUN_RIGHT:
			if (isJumping)
				animation = &right_jump;
			else
				animation = &right_idle;
			break;
		case Player::LAST_ST_RUN_LEFT:
			if (isJumping)
				animation = &left_jump;
			else
				animation = &left_idle;
			break;
		case Player::LAST_ST_SHOOT_RIGHT:
			if (isJumping)
				animation = &right_jump;
			else
				animation = &right_idle;
			break;
		case Player::LAST_ST_SHOOT_LEFT:
			if (isJumping)
				animation = &left_jump;
			else
				animation = &left_idle;
			break;
		default:
			break;
		}
		break;
	case Player::ST_IDLE_RIGHT:
		break;
	case Player::ST_IDLE_LEFT:
		break;
	case Player::ST_RUN_RIGHT:
		if (isJumping)
			animation = &right_jump;
		else
			animation = &right_run;
		break;
	case Player::ST_RUN_LEFT:
		if (isJumping)
			animation = &left_jump;
		else
			animation = &left_run;
		break;
	case Player::ST_SHOOT_RIGHT:
		if (isJumping)
			animation = &right_jump;
		else
			animation = &right_shoot;
		break;
	case Player::ST_SHOOT_LEFT:
		if (isJumping)
			animation = &left_jump;
		else
			animation = &left_shoot;
		break;
	default:
		break;
	}
}

void Player::SetPlayerStates(PlayerState current_state, PlayerLastState last_state)
{
	this->current_state = current_state;
	this->last_state = last_state;
}

void Player::SetPlayerAnimationsSpeed(float dt)
{
	right_idle.speed = right_idle_vel * dt;
	left_idle.speed = left_idle_vel * dt;
	right_run.speed = right_run_vel * dt;;
	left_run.speed = left_run_vel * dt;;
	right_jump.speed = right_jump_vel * dt;
	left_jump.speed = left_jump_vel * dt;
	right_death_blink.speed = right_death_blink_vel * dt;
	left_death_blink.speed = left_death_blink_vel * dt;
	right_shoot.speed = right_shoot_vel * dt;
	left_shoot.speed = left_shoot_vel * dt;
}

bool Player::Load(pugi::xml_node& load)
{
	bool ret = true;

	if (!load.child("position").empty())
	{
		position_implement_load.x = load.child("position").attribute("x").as_float();
		position_implement_load.y = load.child("position").attribute("y").as_float() - 2.0f;
	}

	if (!load.child("info").empty()) {
		lives_implement_load = load.child("info").attribute("lives").as_int();
		score_implement_load = load.child("info").attribute("score").as_int();
		time_implement_load = load.child("info").attribute("time").as_int();
	}

	return ret;
}

void Player::ImplementLoad()
{
	p_lives = lives_implement_load;
	score = score_implement_load;
	position.x = position_implement_load.x;
	position.y = position_implement_load.y;

	real_timer.Start();
	player_appeared = true;
	timer = real_timer.Read();
	LOG("TIMER: %i", timer);
}

bool Player::Save(pugi::xml_node& save) const
{
	bool ret = true;

	if (save.child("position").empty())
	{
		pugi::xml_node&  tmpsave = save.append_child("position");
		tmpsave.append_attribute("x").set_value(position.x);
		tmpsave.append_attribute("y").set_value(position.y);
	}
	else
	{
		save.child("position").attribute("x").set_value(position.x);
		save.child("position").attribute("y").set_value(position.y);
	}

	if (save.child("info").empty())
	{
		pugi::xml_node&  tmpsave = save.append_child("info");
		tmpsave.append_attribute("lives").set_value(p_lives);
		tmpsave.append_attribute("score").set_value(score);
		tmpsave.append_attribute("time").set_value(timer);
	}
	else
	{
		save.child("info").attribute("lives").set_value(p_lives);
		save.child("info").attribute("score").set_value(score);
		save.child("info").attribute("time").set_value(timer);
	}

	return ret;
}

void Player::ResetData()
{
	lives_implement_load = 0;
	score_implement_load = 0;
	time_implement_load = 0;
}

void Player::SetEntitiesSpeed(float dt)
{
	right_idle_vel = right_idle.speed;
	left_idle_vel = left_idle.speed;
	right_jump_vel = right_jump.speed;
	left_jump_vel = left_jump.speed;
	right_run_vel = right_run.speed;
	left_run_vel = left_run.speed;
	right_death_blink_vel = right_death_blink.speed;
	left_death_blink_vel = left_death_blink.speed;

	key_entities_speed = true;
}