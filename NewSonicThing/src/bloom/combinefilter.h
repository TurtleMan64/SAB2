#ifndef COMBINEFILTER_H
#define COMBINEFILTER_H

class ImageRenderer;
class CombineShader;

#include <glad/glad.h>

class CombineFilter 
{
private:
	ImageRenderer* renderer;
	CombineShader* shader;
	
public:
	CombineFilter();
	
	void render(GLuint colourTexture, GLuint highlightTexture);
	
	void cleanUp();
};
#endif