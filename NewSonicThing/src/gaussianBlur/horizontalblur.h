#ifndef HORIZONTALBLUR_H
#define HORIZONTALBLUR_H

class ImageRenderer;
class HorizontalBlurShader;

#include <glad/glad.h>

class HorizontalBlur 
{
private:
	ImageRenderer* renderer;
	HorizontalBlurShader* shader;
	
public:
	HorizontalBlur(int targetFboWidth, int targetFboHeight);
	
	void render(GLuint texture);
	
	int getOutputTexture();
	
	void cleanUp();
};
#endif