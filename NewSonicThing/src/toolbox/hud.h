#ifndef HUD_H
#define HUD_H
#include "menu.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "timer.h"

class HUD : public Menu
{
private:
	float aspectRatio;
	float safeAreaX, safeAreaY;
	float s;
	float w;
	float speedometer;
	float speedometerScale;
	FontType* fontVip;

	GUINumber* numberLives;
	GUINumber* numberSpeed;
	GUIText* textSpeedUnits;
	Timer* timer;
public:
	HUD();
	~HUD();
	void draw();
	Menu* step();
	Timer* getTimer();
};

#endif