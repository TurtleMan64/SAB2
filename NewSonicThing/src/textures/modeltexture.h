#ifndef MODELTEXTURES_H
#define MODELTEXTURES_H

#include <glad/glad.h>
#include "../toolbox/vector.h"
#include <vector>
#include <unordered_set>

class ModelTexture
{
private:
	std::vector<GLuint> texIDs;

    //animation 
    bool isAnimated; //has more than 1 image
    char currentImageIndex; //current index of the animation
    float animatedProgress; //progress to the next image in the animation

    //for use of updating the animation values
    static std::unordered_set<ModelTexture*> animatedTextureReferences;

public:
	float shineDamper;
	float reflectivity;
	float scrollX;
	float scrollY;
	float glowAmount;
	bool hasTransparency;
	bool useFakeLighting;
    int mixingType; //interpolation. 1 = binary, 2 = linear, 3 = sinusoid
    float animationSpeed; //delta per second

	ModelTexture();

	ModelTexture(std::vector<GLuint>* texIDs);

    ModelTexture(ModelTexture* other);

    bool hasMultipleImages();

    //returns image 1
	GLuint getID();

    //returns image 2 (next image in the animation)
    GLuint getID2();

    //how much the 2nd image should be mixed with the first (for animations)
    float mixFactor();

    std::vector<GLuint>* getIDs();

    //deletes all of the texture IDs out of gpu memory
	void deleteMe();

    void addMeToAnimationsSetIfNeeded();

    //updates all of the textures animation progress by dt
    static void updateAnimations(float dt);
};
#endif
