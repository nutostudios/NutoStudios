#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ChainList.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	arrows_left.PushBack({ 58,83,29,34 });
	arrows_left.PushBack({ 29,83,29,34 });
	arrows_left.PushBack({ 0,83,29,34 });
	arrows_left.speed = 0.08F;

	arrows_right.PushBack({ 62,117,31,36 });
	arrows_right.PushBack({ 31,117,31,36 });
	arrows_right.PushBack({ 0,117,31,36 });
	arrows_right.speed = 0.08F;

	arrows_down.PushBack({ 0, 153, 22, 39 });
	arrows_down.PushBack({ 23, 153, 22, 39 });
	arrows_down.PushBack({ 46, 153, 22, 39 });
	arrows_down.speed = 0.08F;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Initialize textures & audio
	scenario = App->textures->Load("pinball/scenario.png");
	upper_scenario = App->textures->Load("pinball/upper_scenario.png");
	ball = App->textures->Load("pinball/ball.png");
	background_elements = App->textures->Load("pinball/background_elements.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	lv2_fx = App->audio->LoadFx("pinball/lv2.wav");
	bumper_fx = App->audio->LoadFx("pinball/bumper_fx.wav");
	littlebumper_fx=App->audio->LoadFx("pinball/littlebump_fx.wav");
	death_fx = App->audio->LoadFx("pinball/death_fx.wav");
	arrows_fx = App->audio->LoadFx("pinball/redarrows_fx.wav");
	greenbump_fx= App->audio->LoadFx("pinball/greenbump_fx.wav");
	plunger_fx = App->audio->LoadFx("pinball/plunger.wav");
	flipper_fx = App->audio->LoadFx("pinball/flipper.wav");
	ramp_fx = App->audio->LoadFx("pinball/ramp.wav");
	red_light_fx = App->audio->LoadFx("pinball/red_light_fx.wav");
	lightson_fx = App->audio->LoadFx("pinball/lightson_fx.wav");
	combo_fx = App->audio->LoadFx("pinball/combo_fx.wav");


	//Play BSO
	App->audio->PlayMusic("pinball/bso_music.ogg");

	//Create Physbodys
	ret = LoadMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(ball);
	App->textures->Unload(background_elements);
	App->textures->Unload(upper_scenario);
	App->textures->Unload(scenario);
	return true;
}
update_status ModuleSceneIntro::PreUpdate()
{

	//Flippers Movement
	if (!death)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {


			pb_left_flipper->body->ApplyAngularImpulse(-2.0F, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			App->audio->PlayFx(flipper_fx);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {


			pb_right_flipper->body->ApplyAngularImpulse(2.0F, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			plunger_joint->SetMotorSpeed((0.0F, 1.0F));
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			plunger_joint->SetMotorSpeed((0.0F, -40.0F));
			App->audio->PlayFx(plunger_fx);
		}
	}

	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Print background layer
	App->renderer->Blit(scenario, 0, 0, NULL, 1.0f);
	App->renderer->Blit(background_elements, 174, 435, &arrows_left.GetCurrentFrame(), 1.0F);
	App->renderer->Blit(background_elements, 305, 368, &arrows_right.GetCurrentFrame(), 1.0F);
	App->renderer->Blit(background_elements, 370, 546, &arrows_down.GetCurrentFrame(), 1.0F);

	// All draw functions ------------------------------------------------------

	if (pb_plunger != NULL)
	{
		int x, y;
		pb_plunger->GetPosition(x, y);
		SDL_Rect rect = { 96,83,19,80 };
		App->renderer->Blit(background_elements, x + 412, y + 651, &rect, 1.0f);
	}

	if (pb_right_bumper != NULL && pb_right_bumper->light == true)
	{
		int x, y;
		pb_right_bumper->GetRealPosition(x, y);
		SDL_Rect rect = { 135,192,65,125 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_right_bumper, 400);
	}

	if (pb_left_bumper != NULL && pb_left_bumper->light == true)
	{
		int x, y;
		pb_left_bumper->GetRealPosition(x, y);
		SDL_Rect rect = { 135,78,66,112 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_left_bumper, 400);
	}

	if (pb_launch_ramp != NULL && pb_launch_ramp->light == true)
	{
		int x, y;
		pb_launch_ramp->GetRealPosition(x, y);
		SDL_Rect rect = { 72,2,92,69 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_launch_ramp, 200);
	}

	if (pb_ramp != NULL && pb_ramp->light == true)
	{
		int x, y;
		pb_ramp->GetRealPosition(x, y);
		SDL_Rect rect = { 69, 266, 66, 100 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_ramp, 200);
	}

	if (pb_ramp_red->body->IsActive())
	{
		int x, y;
		pb_ramp_red->GetRealPosition(x, y);
		SDL_Rect rect = { 30,198,24,18 };
		App->renderer->Blit(background_elements, x, y, &rect);
	}

	if (pb_ramp_blue->body->IsActive())
	{
		int x, y;
		pb_ramp_blue->GetRealPosition(x, y);
		SDL_Rect rect = { 5,197,18,19 };
		App->renderer->Blit(background_elements, x, y, &rect);
	}

	// Check if light = true, draw collision sprite
	p2List_item<PhysBody*>* bumper = pb_bumpers.getFirst();
	while (bumper != NULL)
	{
		if (bumper->data->light == true)
		{
			int x, y;
			bumper->data->GetRealPosition(x, y);
			SDL_Rect rect = { 8,233,61,61 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, bumper->data->GetRotation());
			Timer(bumper->data, 200);
		}
		bumper = bumper->next;
	}

	p2List_item<PhysBody*>* little_bumper = pb_little_bumpers.getFirst();
	while (little_bumper != NULL)
	{
		if (little_bumper->data->light == true)
		{
			int x, y;
			little_bumper->data->GetPosition(x, y);
			SDL_Rect rect = { 167,2,34,34 };
			App->renderer->Blit(background_elements, x, y - 2, &rect, 1.0f, little_bumper->data->GetRotation());
			Timer(little_bumper->data, 200);
		}
		little_bumper = little_bumper->next;
	}

	p2List_item<PhysBody*>* pink_light = pb_pink_lights.getFirst();
	while (pink_light != NULL)
	{
		if (pink_light->data->light == true)
		{
			int x, y;
			pink_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 91,236,23,22 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, pink_light->data->GetRotation());
		}
		pink_light = pink_light->next;
	}

	p2List_item<PhysBody*>* ovalred_light = pb_ovalred_lights.getFirst();
	while (ovalred_light != NULL)
	{
		if (ovalred_light->data->light == true)
		{
			int x, y;
			ovalred_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 5,303,14,28 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, ovalred_light->data->GetRotation());
		}
		ovalred_light = ovalred_light->next;
	}

	p2List_item<PhysBody*>* ovalblue_light = pb_ovalblue_lights.getFirst();
	while (ovalblue_light != NULL)
	{
		if (ovalblue_light->data->light == true)
		{
			int x, y;
			ovalblue_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 8,334,8,22 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, ovalblue_light->data->GetRotation());
		}

		ovalblue_light = ovalblue_light->next;
	}

	p2List_item<PhysBody*>* point_light = pb_point_lights.getFirst();
	while (point_light != NULL)
	{
		if (point_light->data->light == true)
		{
			int x, y;
			point_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 21,334,28,30 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, point_light->data->GetRotation());
		}
		point_light = point_light->next;
	}

	p2List_item<PhysBody*>* pointgreen_light = pb_pointgreen_lights.getFirst();
	while (pointgreen_light != NULL)
	{
		if (pointgreen_light->data->light == true)
		{
			int x, y;
			pointgreen_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 26,296,12,12 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, pointgreen_light->data->GetRotation());
		}
		pointgreen_light = pointgreen_light->next;
	}

	p2List_item<PhysBody*>* yellow_light = pb_yellow_lights.getFirst();
	while (yellow_light != NULL)
	{
		if (yellow_light->data->light == true)
		{
			int x, y;
			yellow_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 40,310,18,21 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, yellow_light->data->GetRotation());
		}
		yellow_light = yellow_light->next;
	}

	p2List_item<PhysBody*>* green_light = pb_green_lights.getFirst();
	while (green_light != NULL)
	{
		if (green_light->data->light == true)
		{
			int x, y;
			green_light->data->GetRealPosition(x, y);
			SDL_Rect rect = { 21,311,17,20 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, green_light->data->GetRotation());
		}

		green_light = green_light->next;
	}

	//Print Flipper
	if (pb_right_flipper != NULL)
	{
		int x, y;
		pb_right_flipper->GetPosition(x, y);
		SDL_Rect rect = { 0,45,69,36 };
		App->renderer->Blit(background_elements, 235, 740, &rect, 1.0f, pb_right_flipper->GetRotation() + 20, 65, 10);
	}

	if (pb_left_flipper != NULL)
	{
		int x, y;
		pb_left_flipper->GetPosition(x, y);
		SDL_Rect rect = { 0,0,68,36 };
		App->renderer->Blit(background_elements, 150, 740, &rect, 1.0f, pb_left_flipper->GetRotation() - 20, 10, 10);
	}

	//Print Ball
	if (pb_ball != NULL)
	{
		int x, y;
		pb_ball->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, pb_ball->GetRotation());
	}


	App->renderer->Blit(upper_scenario, 0, 0, NULL, 1.0f);


	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		pb_ball->body->GetWorld()->DestroyBody(pb_ball->body);
		pb_ball = (App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		pb_ball->listener = this;
	}

	if (ramp_red)
	{
		Timerred(pb_ramp_red, 1000, true, 1);
	}
	else
	{
		Timerred(pb_ramp_red, 1000, false, 1);
	}

	if (ramp_blue)
	{
		Timerblue(pb_ramp_blue, 1000, true, 1);
	}
	else
	{
		Timerblue(pb_ramp_blue, 1000, false, 1);
	}

	if (destroy)
	{
		pb_ball->body->GetWorld()->DestroyBody(pb_ball->body);
		pb_ball = (App->physics->CreateCircle(414, 626, 10));
		pb_ball->listener = this;
		life--;
		destroy = false;
	}

	if (teleport)
	{
		pb_ball->body->GetWorld()->DestroyBody(pb_ball->body);
		pb_ball = (App->physics->CreateCircle(109, 81, 10));
		float random_num = (rand() % 20) + 40;
		pb_ball->body->ApplyForceToCenter({ 0.0F, -random_num }, true);
		pb_ball->listener = this;
		teleport = false;
	}

	if (block)
	{
		pb_border->body->SetActive(true);
	}
	else
	{
		pb_border->body->SetActive(false);
	}

	//Check if 3 lights lighted
	p2List_item<PhysBody*>* ovalred_light_middle = pb_ovalred_lights.getFirst()->next;
	if (ovalred_light_middle != NULL && ovalred_light_middle->prev->data->light == true && ovalred_light_middle->data->light == true && ovalred_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 5000;
		}
		ovalred_light_middle->next->data->light = false;
		ovalred_light_middle->data->light = false;
		ovalred_light_middle->prev->data->light = false;
	}

	p2List_item<PhysBody*>* ovalblue_light_middle = pb_ovalblue_lights.getFirst()->next;
	if (ovalblue_light_middle != NULL && ovalblue_light_middle->prev->data->light == true && ovalblue_light_middle->data->light == true && ovalblue_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 5000;
		}
		ovalblue_light_middle->next->data->light = false;
		ovalblue_light_middle->data->light = false;
		ovalblue_light_middle->prev->data->light = false;
		
	}

	p2List_item<PhysBody*>* pointgreen_light_middle = pb_pointgreen_lights.getFirst()->next;
	if (pointgreen_light_middle != NULL && pointgreen_light_middle->prev->data->light == true && pointgreen_light_middle->data->light == true && pointgreen_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 20000;
		}
		pointgreen_light_middle->next->data->light = false;
		pointgreen_light_middle->data->light = false;
		pointgreen_light_middle->prev->data->light = false;
		
	}

	p2List_item<PhysBody*>* yellow_light_middle = pb_yellow_lights.getFirst()->next;
	if (yellow_light_middle != NULL && yellow_light_middle->prev->data->light == true && yellow_light_middle->data->light == true && yellow_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 15000;
		}
		yellow_light_middle->next->data->light = false;
		yellow_light_middle->data->light = false;
		yellow_light_middle->prev->data->light = false;
		
	}

	p2List_item<PhysBody*>* point_light_middle = pb_point_lights.getFirst()->next;
	if (point_light_middle != NULL && point_light_middle->prev->data->light == true && point_light_middle->data->light == true && point_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 15000;
		}
		point_light_middle->next->data->light = false;
		point_light_middle->data->light = false;
		point_light_middle->prev->data->light = false;
	}

	p2List_item<PhysBody*>* green_light_middle = pb_green_lights.getFirst()->next;
	if (green_light_middle != NULL && green_light_middle->prev->data->light == true && green_light_middle->data->light == true && green_light_middle->next->data->light == true || death == true)
	{
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 15000;
		}
		green_light_middle->next->data->light = false;
		green_light_middle->data->light = false;
		green_light_middle->prev->data->light = false;
	}

	p2List_item<PhysBody*>* pink_light_middle = pb_pink_lights.getFirst()->next;
	if (pink_light_middle != NULL && pink_light_middle->prev->data->light == true && pink_light_middle->data->light == true && pink_light_middle->next->data->light == true || death == true)
	{
		
		if (!death)
		{
			App->audio->PlayFx(combo_fx);
			App->ui->score_player += 15000;
		}
		pink_light_middle->next->data->light = false;
		pink_light_middle->data->light = false;
		pink_light_middle->prev->data->light = false;
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(bonus_fx);
	if (bodyA == pb_ball)
	{
		//Lights
		p2List_item<PhysBody*>* red_item = pb_red_lights.getFirst();
		while (red_item != NULL) {
			if (bodyB == red_item->data)
			{
				App->audio->PlayFx(red_light_fx);
				App->ui->score_player += 200;
			}
			red_item = red_item->next;
		}

		p2List_item<PhysBody*>* bumper_item = pb_bumpers.getFirst();
		while (bumper_item != NULL) {
			if (bodyB == bumper_item->data)
			{
				App->audio->PlayFx(littlebumper_fx);
				App->ui->score_player += 500;
				bumper_item->data->light = true;
			}
			bumper_item = bumper_item->next;
		}

		p2List_item<PhysBody*>* pink_item = pb_pink_lights.getFirst();
		while (pink_item != NULL) {
			if (bodyB == pink_item->data)
			{
				App->audio->PlayFx(lightson_fx);

				pink_item->data->light = true;
			}
			pink_item = pink_item->next;
		}

		p2List_item<PhysBody*>* yellow_item = pb_yellow_lights.getFirst();
		while (yellow_item != NULL) {
			if (bodyB == yellow_item->data)
			{
				App->audio->PlayFx(lightson_fx);

				yellow_item->data->light = true;
			}
			yellow_item = yellow_item->next;
		}

		p2List_item<PhysBody*>* green_item = pb_green_lights.getFirst();
		while (green_item != NULL) {
			if (bodyB == green_item->data)
			{
				App->audio->PlayFx(lightson_fx);

				green_item->data->light = true;
			}
			green_item = green_item->next;
		}

		p2List_item<PhysBody*>* point_item = pb_point_lights.getFirst();
		while (point_item != NULL) {
			if (bodyB == point_item->data)
			{
				App->audio->PlayFx(lightson_fx);

				point_item->data->light = true;
			}
			point_item = point_item->next;
		}
		
		p2List_item<PhysBody*>* ovalred_item = pb_ovalred_lights.getFirst();
		while (ovalred_item != NULL) {
			if (bodyB == ovalred_item->data)
			{
				App->audio->PlayFx(lightson_fx);

				ovalred_item->data->light = true;
			}
			ovalred_item = ovalred_item->next;
		}

		p2List_item<PhysBody*>* pointgreen_item = pb_pointgreen_lights.getFirst();
		while (pointgreen_item != NULL) {
			if (bodyB == pointgreen_item->data)
			{
				pointgreen_item->data->light = true;
			}
			pointgreen_item = pointgreen_item->next;
		}

		p2List_item<PhysBody*>* ovalblue_item = pb_ovalblue_lights.getFirst();
		while (ovalblue_item != NULL) {
			if (bodyB == ovalblue_item->data)
			{
				
				App->audio->PlayFx(lightson_fx);
				ovalblue_item->data->light = true;
			}
			ovalblue_item = ovalblue_item->next;
		}
		
		p2List_item<PhysBody*>* little_bumper_item = pb_little_bumpers.getFirst();
		while (little_bumper_item != NULL) {
			if (bodyB == little_bumper_item->data)
			{
				App->audio->PlayFx(littlebumper_fx);
				App->ui->score_player += 500;
				little_bumper_item->data->light = true;
			}
			little_bumper_item = little_bumper_item->next;
		}

		if (bodyB == pb_left_bumper)
		{
			App->audio->PlayFx(greenbump_fx);
			pb_left_bumper->light = true;
		}
		if (bodyB == pb_right_bumper)
		{
			App->audio->PlayFx(greenbump_fx);
			pb_right_bumper->light = true;
		}
		if (bodyB == pb_launch_ramp)
		{
			pb_launch_ramp->light = true;
			App->audio->PlayFx(ramp_fx);
		}
		if (bodyB == pb_ramp)
		{
			pb_ramp->light = true;
			App->audio->PlayFx(ramp_fx);
		}
		if (bodyB == pb_right_push)
		{
			bodyA->body->ApplyLinearImpulse({ -0.02f,-2.0F }, bodyA->body->GetLocalCenter(), true);
			ramp_red = true;
		}
		if (bodyB == pb_left_push)
		{
			bodyA->body->ApplyLinearImpulse({ 0.01f,-2.0F }, bodyA->body->GetLocalCenter(), true);
			ramp_blue = true;
		}
		if (bodyB == pb_wormhole_entry)
		{
			teleport = true;
			App->ui->score_player += 1000;
		}
		if (bodyB == pb_ramp_sensor)
		{
			block = true;
		}
		if (bodyB == pb_ramp_leave)
		{
			block = false;
		}
		if (bodyB == pb_border)
		{
			bodyA->body->ApplyLinearImpulse({ 0.5F, 0.5F }, bodyA->body->GetLocalCenter(), true);
		}
		
		//Death
		if (bodyB == pb_death_sensor)
		{
			App->audio->PlayFx(death_fx);
			destroy = true;
			ramp_blue = false;
			ramp_red = false;
			
		}
	}
}

//Load Map
bool ModuleSceneIntro::LoadMap()
{
	//Create Ball
	pb_ball = (App->physics->CreateCircle(414, 626, 10));
	pb_ball->body->SetBullet(true);
	pb_ball->body->GetFixtureList()->SetFriction(0.4F);
	pb_ball->listener = this;
	life = 3;

	//Define Map Physbodys
	pb_background = App->physics->CreateChain(0, 0, scenario_points, 318);
	pb_background->body->SetType(b2_staticBody);

	pb_purple_ramp = App->physics->CreateChain(0, 0, purple_ramp, 72);
	pb_purple_ramp->body->SetType(b2_staticBody);

	pb_right_flipper = App->physics->CreateRectangle(270, 746, 62, 13);
	pb_left_flipper = App->physics->CreateRectangle(180,750,62,13);

	pb_right_bumper = App->physics->CreateChain(0, 0, right_bumper_coll, 6, 277, 606);
	pb_right_bumper->body->SetType(b2_staticBody);
	pb_right_bumper->body->GetFixtureList()->SetRestitution(1.2F);
	pb_right_bumper->body->GetFixtureList()->SetFriction(0.5F);

	pb_left_bumper = App->physics->CreateChain(0, 0, left_bumper_coll, 6, 105, 605);
	pb_left_bumper->body->SetType(b2_staticBody);
	pb_left_bumper->body->GetFixtureList()->SetRestitution(1.2F);
	pb_left_bumper->body->GetFixtureList()->SetFriction(0.3F);

	pb_plunger = App->physics->CreateChain(0, 0, plunger, 10);

	pb_border = App->physics->CreateRectangle(30, 370, 20, 20);
	pb_border->body->SetType(b2_staticBody);
	pb_border->body->SetActive(false);

	pb_ramp_blue = App->physics->CreateChain(0, 0, blue_ramp, 8, 17, 711);
	pb_ramp_blue->body->SetType(b2_staticBody);
	pb_ramp_blue->body->SetActive(false);
	pb_ramp_red = App->physics->CreateChain(0, 0, red_ramp, 8, 373, 729);
	pb_ramp_red->body->SetType(b2_staticBody);
	pb_ramp_red->body->SetActive(false);

	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_10000, 14));
	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_launch_ramp, 56));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_bumper, 22));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_bumper, 20));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_blocker, 10));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_1, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_2, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, block_left, 8));
	pb_background_elements.add(App->physics->CreateChain(0, 0, block_right, 8));
	pb_background_elements.add(App->physics->CreateRectangle(33, 423, 6, 10));
	pb_background_elements.add(App->physics->CreateRectangle(64, 436, 6, 15));
	
	p2List_item<PhysBody*>* back_elem = pb_background_elements.getFirst();
	while (back_elem != NULL)
	{
		back_elem->data->body->SetType(b2_staticBody);
		back_elem = back_elem->next;
	}

	//Define Ramp parameters
	pb_launch_ramp = App->physics->CreateChain(0, 0, launch_ramp_sensor, 8, 77, 277);
	pb_launch_ramp->body->SetType(b2_staticBody);
	pb_launch_ramp->body->GetFixtureList()->SetRestitution(1.2F);
	pb_launch_ramp->body->GetFixtureList()->SetFriction(0.1F);

	pb_ramp = App->physics->CreateChain(0, 0, ramp_sensor, 6, 275, 239);
	pb_ramp->body->SetType(b2_staticBody);
	pb_ramp->body->GetFixtureList()->SetRestitution(1.2F);
	pb_ramp->body->GetFixtureList()->SetFriction(0.1F);

	//Define Bumper Physbodys
	pb_bumpers.add(App->physics->CreateChain(0, 0, up_bumper, 42, 40, 40));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_1, 42, 147, 195));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_2, 44, 225, 165));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_3, 44, 181, 267));

	pb_little_bumpers.add(App->physics->CreateCircle(23, 465, 15));
	pb_little_bumpers.add(App->physics->CreateCircle(80, 479, 15));
	pb_little_bumpers.add(App->physics->CreateCircle(42, 514, 15));

	p2List_item<PhysBody*>* little_bump_elem = pb_little_bumpers.getFirst();
	while (little_bump_elem != NULL)
	{
		little_bump_elem->data->body->SetType(b2_staticBody);
		little_bump_elem->data->body->GetFixtureList()->SetRestitution(1.0F);
		little_bump_elem->data->body->GetFixtureList()->SetFriction(0.4F);
		little_bump_elem = little_bump_elem->next;
	}

	p2List_item<PhysBody*>* bump_elem = pb_bumpers.getFirst();
	while (bump_elem != NULL)
	{
		bump_elem->data->body->SetType(b2_staticBody);
		bump_elem->data->body->GetFixtureList()->SetRestitution(1.2F);
		bump_elem->data->body->GetFixtureList()->SetFriction(0.3F);
		bump_elem = bump_elem->next;
	}
	
	//Define Sensors
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 570, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 455, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 340, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 230, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(405, 150, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(350, 95, 15, 40));

	pb_pink_lights.add(App->physics->CreateRectangleSensor(314, 170, 5, 10, -20, -4));
	pb_pink_lights.add(App->physics->CreateRectangleSensor(328, 190, 5, 10, -20, -4));
	pb_pink_lights.add(App->physics->CreateRectangleSensor(334, 212, 5, 10, -20, -4));

	pb_green_lights.add(App->physics->CreateRectangleSensor(122, 174, 5, 10, -1, -1));
	pb_green_lights.add(App->physics->CreateRectangleSensor(147, 160, 5, 10, -3, 1));
	pb_green_lights.add(App->physics->CreateRectangleSensor(167, 150, 5, 10, -5, -3));

	pb_yellow_lights.add(App->physics->CreateRectangleSensor(67, 226, 5, 10, 3, -12));
	pb_yellow_lights.add(App->physics->CreateRectangleSensor(68, 253, 5, 10, 3, -13));
	pb_yellow_lights.add(App->physics->CreateRectangleSensor(70, 275, 5, 10, 4, -11));

	pb_ovalred_lights.add(App->physics->CreateRectangleSensor(325, 432, 5, 10, 4, -15));
	pb_ovalred_lights.add(App->physics->CreateRectangleSensor(325, 459, 5, 10, 4, -15));
	pb_ovalred_lights.add(App->physics->CreateRectangleSensor(326, 487, 5, 10, 4, -15));
	
	pb_pointgreen_lights.add(App->physics->CreateRectangleSensor(19, 404, 6, 6, -6, -5));
	pb_pointgreen_lights.add(App->physics->CreateRectangleSensor(48, 416, 6, 6, -6, -5));
	pb_pointgreen_lights.add(App->physics->CreateRectangleSensor(79, 426, 6, 6, -6, -5));

	pb_ovalblue_lights.add(App->physics->CreateRectangleSensor(108, 478, 5, 10, 7, -8));
	pb_ovalblue_lights.add(App->physics->CreateRectangleSensor(108, 509, 5, 10, 7, -13));
	pb_ovalblue_lights.add(App->physics->CreateRectangleSensor(108, 530, 5, 10, 7, -10));

	pb_point_lights.add(App->physics->CreateRectangleSensor(192, 90, 8, 8, -11, -16));
	pb_point_lights.add(App->physics->CreateRectangleSensor(225, 90, 8, 8, -11, -16));
	pb_point_lights.add(App->physics->CreateRectangleSensor(258, 90, 8, 8, -11, -16));
	
	pb_death_sensor = App->physics->CreateRectangleSensor(220, 805, 80, 20);
	pb_left_push = App->physics->CreateRectangleSensor(25,772,15,16);
	pb_right_push = App->physics->CreateRectangleSensor(384,772,16,16);
	pb_wormhole_entry = App->physics->CreateRectangleSensor(348, 510, 20, 20);
	pb_ramp_sensor = App->physics->CreateRectangleSensor(20, 410, 20, 20);
	pb_ramp_leave = App->physics->CreateRectangleSensor(60, 600, 20, 20);

	//Define Joints
	//Revolute Joint
	b2RevoluteJointDef left_flipper;
	b2RevoluteJointDef right_flipper;
	PhysBody* circle_left;
	circle_left = App->physics->CreateCircle(155, 745, 5);
	circle_left->body->SetType(b2_staticBody);

	PhysBody* circle_right;
	circle_right = App->physics->CreateCircle(300, 745, 5);
	circle_right->body->SetType(b2_staticBody);

	left_flipper.Initialize(pb_left_flipper->body, circle_left->body, circle_left->body->GetWorldCenter());
	left_flipper.collideConnected = false;

	right_flipper.Initialize(pb_right_flipper->body, circle_right->body, circle_right->body->GetWorldCenter());
	right_flipper.collideConnected = false;

	left_flipper.enableLimit = true;
	left_flipper.lowerAngle = -20 * DEGTORAD;
	left_flipper.upperAngle = 30 * DEGTORAD;

	right_flipper.enableLimit = true;
	right_flipper.lowerAngle = -20 * DEGTORAD;
	right_flipper.upperAngle = 20 * DEGTORAD;


	left_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_flipper);
	right_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_flipper);

	//Prismatic Joint
	b2PrismaticJointDef jointDef; 
	b2Vec2 worldAxis(0.0f, 1.0f); 
	jointDef.Initialize(App->physics->ground, pb_plunger->body, { 422, 742 }, worldAxis);

	jointDef.enableLimit = true;
	jointDef.upperTranslation = 1.0F;
	jointDef.lowerTranslation = 0.0F;

	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 500.0f;
	jointDef.motorSpeed = 0.0f;

	plunger_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&jointDef);
	return true;
}

void ModuleSceneIntro::Timerblue(PhysBody* pb, int time, bool value, int type)
{
	if (timerblue)
	{
		time_on_entry = SDL_GetTicks();
		timerblue = false;
	}
	current_time = SDL_GetTicks() - time_on_entry;
	if (current_time > time)
	{
		if (type == 0)
		{
			pb->light = value;
		}
		else if (type == 1)
		{
			pb->body->SetActive(value);
		}
		timerblue = true;
	}
}

void ModuleSceneIntro::Timerred(PhysBody* pb, int time, bool value, int type)
{
	if (timered)
	{
		time_on_entry = SDL_GetTicks();
		timered = false;
	}
	current_time = SDL_GetTicks() - time_on_entry;
	if (current_time > time)
	{
		if (type == 0)
		{
			pb->light = value;
		}
		else if (type == 1)
		{
			pb->body->SetActive(value);
		}
		timered = true;
	}
}
void ModuleSceneIntro::Timer(PhysBody* pb, int time, bool value, int type)
{
	if (timer)
	{
		time_on_entry = SDL_GetTicks();
		timer = false;
	}
	current_time = SDL_GetTicks() - time_on_entry;
	if (current_time > time)
	{
		if (type == 0)
		{
			pb->light = value;
		}
		else if (type == 1)
		{
			pb->body->SetActive(value);
		}
		timer = true;
	}
}