#include "particle.hpp"
#include "particletexture.hpp"
#include "../entities/camera.hpp"
#include "../toolbox/vector.hpp"
#include "particlemaster.hpp"
#include "particleresources.hpp"
#include "../engineTester/main.hpp"

extern float dt;

Particle::Particle()
{

}

Particle::~Particle()
{

}

ParticleStandard::~ParticleStandard()
{
    
}

ParticleStandard::ParticleStandard(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce)
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
    this->scale = scale;
    this->scaleChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticleStandard(this);
}

ParticleStandard::ParticleStandard(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce)
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
    this->scale = scale;
    this->scaleChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = opacity;
    ParticleMaster::addParticleStandard(this);
}

ParticleStandard::ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce)
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
    this->scale = scale;
    this->scaleChange = 0;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticleStandard(this);
}

ParticleStandard::ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity)
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
    this->scale = scale;
    this->scaleChange = scaleChange;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = opacity;
    ParticleMaster::addParticleStandard(this);
}

ParticleStandard::ParticleStandard(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce)
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
    this->scale = scale;
    this->scaleChange = scaleChange;
    this->onlyRendersOnce = onlyRendersOnce;
    this->opacity = 1.0f;
    ParticleMaster::addParticleStandard(this);
}

void Particle::updateTextureCoordInfo()
{
    float lifeFactor = elapsedTime / lifeLength;
    int stageCount = texture->numberOfRows * texture->numberOfRows;
    float atlasProgression = lifeFactor * stageCount;
    int index1 = (int)atlasProgression;
    int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
    this->blend = fmodf(atlasProgression, 1); 
    setTextureOffset(&texOffset1, index1);
    setTextureOffset(&texOffset2, index2);
}

void Particle::setTextureOffset(Vector2f* offset, int index)
{
    int column = index % texture->numberOfRows;
    int row = index / texture->numberOfRows;
    offset->x = (float)column / texture->numberOfRows;
    offset->y = (float)row / texture->numberOfRows;
}

bool Particle::update()
{
    printf("default update being called\n");
    return false;
}

bool ParticleStandard::update()
{
    velocity.y -= gravityEffect*dt;
    scale = fmaxf(0, scale + scaleChange*dt);

    position.x += velocity.x*dt;
    position.y += velocity.y*dt;
    position.z += velocity.z*dt;

    updateTextureCoordInfo();
    elapsedTime += dt;

    bool stillAlive = elapsedTime < lifeLength;

    if (onlyRendersOnce)
    {
        elapsedTime = lifeLength;
    }

    return stillAlive;
}
