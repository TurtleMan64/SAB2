#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

class HorizontalBlur;
class VerticalBlur;
class CombineFilter;

#include <vector>
#include "../models/models.h"

class PostProcessing 
{
private:
	static std::vector<float> POSITIONS;
	static RawModel quadModel;
	static HorizontalBlur* hBlur;
	static VerticalBlur*   vBlur;
	static HorizontalBlur* hBlur2;
	static VerticalBlur*   vBlur2;
	static CombineFilter* combineFilter;
	
public:

	static void init();
	
	static void doPostProcessing(int colourTexture, int brightTexture);
	
	static void cleanUp();
	
	static void start();
	
	static void end();
};
#endif