#ifndef TIMER_H
#define TIMER_H

class GUIText;
class GUINumber;
class FontType;

class Timer
{
private:
	// Parts of the timer
	GUIText* colon;
	GUIText* dot;
	GUINumber* minutes;
	GUINumber* seconds;
	GUINumber* centiseconds;

public:
    float totalTime;
	bool frozen;

	Timer(FontType* font, float posX, float posY, float scale, int alignment, bool visible);
	~Timer();
	float getTime();
	void setTime(float newTime);
	void refresh();
	void increment();
	void freeze(bool freezeStatus);
};

#endif
