#ifndef MODELTEXTURES_H
#define MODELTEXTURES_H

#include <glad/glad.h>
#include "../toolbox/vector.h"

class ModelTexture
{
private:
	GLuint texID;

	float shineDamper;
	float reflectivity;
	float scrollX;
	float scrollY;
	float glowAmount;
	int hasTransparency;
	int useFakeLighting;

public:
	ModelTexture();

	ModelTexture(GLuint texID);

	GLuint getID();
	void setID(GLuint newID);

	float getShineDamper();
	void setShineDamper(float newShineDamper);

	float getReflectivity();
	void setReflectivity(float newReflectivity);

	int getHasTransparency();
	void setHasTransparency(int newHasTransparency);

	int getUsesFakeLighting();
	void setUsesFakeLighting(int newUsesFakeLighting);

	float getGlowAmount();
	void setGlowAmount(float newGlowAmount);

	void setScrollX(float speedX);
	void setScrollY(float speedY);

	float getScrollX();
	float getScrollY();

	void deleteMe();
};
#endif