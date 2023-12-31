#include <iostream>
#include <SDL.h>
#include <string>
//#include <cstdint>
#include "timer.h"

Timer::Timer() : start_time(0), time(0)
{

}

void Timer::start()
{
	start_time = SDL_GetTicks();
}

void Timer::stop()
{
	time = SDL_GetTicks() - start_time;
}

double Timer::get_time()
{
	double res = 0;
	if(time == 0) //timer not stopped yet
		res = SDL_GetTicks() - start_time;
	else
		res = time;
	return res;
}

std::string Timer::get_time_string()
{
	return get_time_string(get_time());	
}

std::string Timer::get_time_string(int t)
{
	double min = t / 60000;
	double sec = (t / 1000) % 60;
	double ms = t % 1000;
	std::string res = std::string(min < 10 ? "0" : "") + std::to_string(min) + ":";
	res += std::string(sec < 10 ? "0" : "") + std::to_string(sec);
	res += "." + std::string(ms < 100 ? "0" : "") + std::string(ms < 10 ? "0" : "");
	res += std::to_string(ms);
	return res;
}

/*
Timer& Timer::operator=(Timer t)
{
    return *this;
}
*/