#ifndef TIMER_H
#define TIMER_H

#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/guitext.h"

class Timer {
private:
	// Parts of the timer
	GUIText* colon;
	GUIText* dot;
	GUINumber* minutes;
	GUINumber* seconds;
	GUINumber* centiseconds;

	float totalTime;
	bool frozen;
	
public:
	Timer(FontType*, float, float, float, int, bool);
	~Timer();
	float getTime();
	void setTime(float);
	void refresh();
	void increment();
	void freeze(bool);
};

#endif