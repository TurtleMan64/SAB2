#ifndef PARTICLE_H
#define PARTICLE_H

class ParticleTexture;

#include "../toolbox/vector.hpp"

class Particle
{
protected:
    Vector3f position;
    //Vector3f* positionRef;
    Vector3f velocity;
    float gravityEffect;
    float lifeLength;
    bool onlyRendersOnce;
    //float rotation;
    //float scaleX;
    float scaleChange;
    //float scaleY;
    //float scaleYChange;
    //float distance;
    //ParticleTexture* texture;
    //Vector2f texOffset1;
    //Vector2f texOffset2;
    //float blend;
    float elapsedTime = 0;
    //float opacity;

    void updateTextureCoordInfo();

    void setTextureOffset(Vector2f* offset, int index);

public:
    float distance;
    ParticleTexture* texture;
    Vector3f* positionRef;
    float scale;
    float opacity;
    Vector2f texOffset1;
    Vector2f texOffset2;
    float blend;

    Particle();
    virtual ~Particle();

    //Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce);
    //
    //Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce);
    //
    //Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce);
    //
    //Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    //    float lifeLength, float rotation, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity);
    //
    //Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    //    float lifeLength, float rotation, float scaleX, float scaleXChange, float scaleY, float scaleYChange, 
    //    bool posIsRef, bool onlyRendersOnce);

    //float getDistance();
    //
    //ParticleTexture* getTexture();
    //
    //Vector3f* getPosition();
    //
    //float getRotation();
    //
    //float getScaleX();
    //
    //float getScaleY();
    //
    //float getOpacity();
    //
    //Vector2f* getTexOffset1();
    //
    //Vector2f* getTexOffset2();
    //
    //float getBlend();

    virtual bool update();
};

class ParticleStandard : public Particle
{
public:
    ~ParticleStandard();

    ParticleStandard(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce);

    ParticleStandard(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce);

    ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce);

    ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity);

    ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce);

    bool update();
};

#endif
