#ifndef VERTICALBLUR_H
#define VERTICALBLUR_H

class ImageRenderer;
class VerticalBlurShader;

#include <glad/glad.h>

class VerticalBlur 
{
private:
	ImageRenderer* renderer;
	VerticalBlurShader* shader;
	
public:
	VerticalBlur(int targetFboWidth, int targetFboHeight);
	
	void render(GLuint texture);
	
	int getOutputTexture();
	
	void cleanUp();
};
#endif