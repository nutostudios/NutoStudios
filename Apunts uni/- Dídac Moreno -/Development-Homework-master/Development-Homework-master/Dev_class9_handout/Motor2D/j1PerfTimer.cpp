// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

// ---------------------------------------------
j1PerfTimer::j1PerfTimer()
{
	// TODO 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
	// they are simple, one line each!


	//Uint64 SDL_GetPerformanceCounter(void)
    //Uint64 SDL_GetPerformanceFrequency(void)
	frequency = SDL_GetPerformanceFrequency();
	Start();
}

// ---------------------------------------------
void j1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double j1PerfTimer::ReadMs() const
{
	return (ReadTicks() * 1000000 / frequency) ;
}

// ---------------------------------------------
uint64 j1PerfTimer::ReadTicks() const
{
	return (SDL_GetPerformanceCounter() - started_at);
}


