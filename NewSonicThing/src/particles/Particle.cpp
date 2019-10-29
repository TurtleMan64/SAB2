#include "particle.h"
#include "particletexture.h"
#include "../entities/camera.h"
#include "../toolbox/vector.h"
#include "particlemaster.h"
#include "particleresources.h"
#include "../engineTester/main.h"

extern float dt;

Particle::Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce)
{
    this->texture = texture;
    this->positionRef = &this->position;
    this->position.x = position->x;
    this->position.y = position->y;
    this->position.z = position->z;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
    this->gravityEffect = 0;
    this->lifeLength = lifeLength;
    this->rotation = 0;
    this->scaleX = scale;
    this->scaleXChange = 0;
    this->scaleY = scale;
    this->scaleYChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticle(this);
}

Particle::Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce)
{
    this->texture = texture;
    this->positionRef = &this->position;
    this->position.x = position->x;
    this->position.y = position->y;
    this->position.z = position->z;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
    this->gravityEffect = 0;
    this->lifeLength = lifeLength;
    this->rotation = 0;
    this->scaleX = scale;
    this->scaleXChange = 0;
    this->scaleY = scale;
    this->scaleYChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = opacity;
    ParticleMaster::addParticle(this);
}

Particle::Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce)
{
    this->texture = texture;
    this->positionRef = &this->position;
    this->position.x = position->x;
    this->position.y = position->y;
    this->position.z = position->z;
    this->velocity.x = velocity->x;
    this->velocity.y = velocity->y;
    this->velocity.z = velocity->z;
    this->gravityEffect = 0;
    this->lifeLength = lifeLength;
    this->rotation = 0;
    this->scaleX = scale;
    this->scaleXChange = 0;
    this->scaleY = scale;
    this->scaleYChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticle(this);
}

Particle::Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    float lifeLength, float rotation, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity)
{
    this->texture = texture;
    if (posIsRef)
    {
        this->positionRef = position;
    }
    else
    {
        this->positionRef = &this->position;
    }
    this->position.x = position->x;
    this->position.y = position->y;
    this->position.z = position->z;
    this->velocity.x = velocity->x;
    this->velocity.y = velocity->y;
    this->velocity.z = velocity->z;
    this->gravityEffect = gravityEffect;
    this->lifeLength = lifeLength;
    this->rotation = rotation;
    this->scaleX = scale;
    this->scaleXChange = scaleChange;
    this->scaleY = scale;
    this->scaleYChange = scaleChange;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = opacity;
    ParticleMaster::addParticle(this);
}

Particle::Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float rotation, float scaleX, float scaleXChange, float scaleY, float scaleYChange, 
        bool posIsRef, bool onlyRendersOnce)
{
    this->texture = texture;
    if (posIsRef)
    {
        this->positionRef = position;
    }
    else
    {
        this->positionRef = &this->position;
    }
    this->position.x = position->x;
    this->position.y = position->y;
    this->position.z = position->z;
    this->velocity.x = velocity->x;
    this->velocity.y = velocity->y;
    this->velocity.z = velocity->z;
    this->gravityEffect = gravityEffect;
    this->lifeLength = lifeLength;
    this->rotation = rotation;
    this->scaleX = scaleX;
    this->scaleXChange = scaleXChange;
    this->scaleY = scaleY;
    this->scaleYChange = scaleYChange;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticle(this);
}

void Particle::updateTextureCoordInfo()
{
    float lifeFactor = elapsedTime / lifeLength;
    int stageCount = texture->getNumberOfRows() * texture->getNumberOfRows();
    float atlasProgression = lifeFactor * stageCount;
    int index1 = (int)atlasProgression;
    int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
    this->blend = fmodf(atlasProgression, 1); 
    setTextureOffset(&texOffset1, index1);
    setTextureOffset(&texOffset2, index2);
}

void Particle::setTextureOffset(Vector2f* offset, int index)
{
    int column = index % texture->getNumberOfRows();
    int row = index / texture->getNumberOfRows();
    offset->x = (float)column / texture->getNumberOfRows();
    offset->y = (float)row / texture->getNumberOfRows();
}

float Particle::getDistance()
{
    return distance;
}

ParticleTexture* Particle::getTexture()
{
    return texture;
}

Vector3f* Particle::getPosition()
{
    return positionRef;
}

float Particle::getRotation()
{
    return rotation;
}

float Particle::getScaleX()
{
    return scaleX;
}

float Particle::getScaleY()
{
    return scaleY;
}

float Particle::getOpacity()
{
    return opacity;
}

Vector2f* Particle::getTexOffset1()
{
    return &texOffset1;
}

Vector2f* Particle::getTexOffset2()
{
    return &texOffset2;
}

float Particle::getBlend()
{
    return blend;
}

bool Particle::update(Camera* camera)
{
    velocity.y -= gravityEffect*dt;
    scaleX = fmaxf(0, scaleX + scaleXChange*dt);
    scaleY = fmaxf(0, scaleY + scaleYChange*dt);

    position.x += velocity.x*dt;
    position.y += velocity.y*dt;
    position.z += velocity.z*dt;

    Vector3f dist(&camera->eye);
    dist = dist-position;

    distance = dist.lengthSquared();

    updateTextureCoordInfo();
    elapsedTime += dt;

    bool isDead = elapsedTime < lifeLength;

    if (onlyRendersOnce)
    {
        elapsedTime = lifeLength;
    }

    return isDead;
}
