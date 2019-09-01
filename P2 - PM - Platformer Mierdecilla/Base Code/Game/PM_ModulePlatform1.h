#ifndef __PLATFORM1_H__
#define __PLATFORM1_H__

#include "PM_ModulePlatform.h"

class Platform1 : public Platform
{
private:

	int original_y = 0;
	int original_x = 0;
	Animation plat;


public:

	Platform1(int x, int y);


};

#endif // __PLATFORM1_H__


//#ifndef __PLATFORM1_H__
//#define __PLATFORM1_H__
//
//#include "Enemy.h"
//
//class Platform1 : public Platform
//{
//private:
//	float wave = -1.0f;
//	bool going_up = true;
//	int original_y = 0;
//	Animation fly;
//
//public:
//
//	Enemy_RedBird(int x, int y);
//
//	void Move();
//};
//
//#endif // __PLATFORM1_H__

