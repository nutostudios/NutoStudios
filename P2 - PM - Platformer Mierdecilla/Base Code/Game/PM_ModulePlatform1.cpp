/*

#include "Application.h"
#include "PM_ModulePlatform1.h"
#include "ModuleCollision.h"

Platform1::Platform1(int x, int y) : Platform(x, y)
{
	plat.PushBack({ 0,0,0,0 });
	plat.speed = 0.2f;

	animation = &plat;

	collider = App->collision->AddCollider({ 0, 0, 24, 24 }, COLLIDER_TYPE::COLLIDER_PLATFORM, (Module*)App->PM_platform);

	original_y = y;
	original_x = x;

}

//void Platform1::Move()
//{
//	if (going_up)
//	{
//		if (wave > 1.0f)
//			going_up = false;
//		else
//			wave += 0.05f;
//	}
//	else
//	{
//		if (wave < -1.0f)
//			going_up = true;
//		else
//			wave -= 0.05f;
//	}
//
//	position.y = int(float(original_y) + (25.0f * sinf(wave)));
//	position.x -= 1;
//}

*/
