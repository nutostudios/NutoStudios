#ifndef  _MODULE_SCENECHANGE_
#define _MODULE_SCENECHANGE_

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum ListOfMapNames;

class j1MapChange : public j1Module
{
//-------------Functions-----------------
public:

	j1MapChange();

	//Destructor
	virtual ~j1MapChange();

	//Called before render is avalible
	bool Awake(pugi::xml_node&);

	//Called before the first frame
	bool Start();

	//Called each loop iteration
	bool Update(float dt);

	//Called to set up the map change and the time the game will fade
	bool ChangeMap(int newMap, float time);

	//returns true if the change process is active
	bool IsChanging() const;

private:
	
//-------------Variables-----------------
public:

	bool fading = false;

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	
	int nextMap;
	uint start_time = 0;
	uint total_time = 0;
	SDL_Rect screen;


};
#endif // ! _MODULE_SCENECHANGE_
