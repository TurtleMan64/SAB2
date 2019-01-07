#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

class Fbo;

class ImageRenderer 
{
private:
	Fbo* fbo = nullptr;
	
public:
	ImageRenderer(int width, int height);
	
	ImageRenderer();
	
	void renderQuad();
	
	int getOutputTexture();
	
	void cleanUp();
};
#endif