#ifndef PARTICLERESOURCES_H
#define PARTICLERESOURCES_H

#include <vector>

class ParticleTexture;

class ParticleResources
{
public:
    static ParticleTexture* textureExhaust;
    static ParticleTexture* particleExhaustArwing;
    static ParticleTexture* textureDust;
    static ParticleTexture* textureDirt;
    static ParticleTexture* textureSnowDrop;
    static ParticleTexture* textureDustCloud;
    static ParticleTexture* textureSnowball;
    static ParticleTexture* textureStar;
    static ParticleTexture* textureSparkleYellow;
    static ParticleTexture* textureSparkleGreen;
    static ParticleTexture* textureSparkleRed;
    static ParticleTexture* textureSparkleBlue;
    static ParticleTexture* textureSparkleLightBlue;
    static ParticleTexture* textureSparkleWhite;
    static ParticleTexture* textureWaterDrop;
    static ParticleTexture* textureWhiteTrail;
    static ParticleTexture* textureLightBlueTrail;
    static ParticleTexture* textureBlueTrail;
    static ParticleTexture* textureBlackTrail;
    static ParticleTexture* textureGrayTrail;
    static ParticleTexture* texturePinkTrail;
    static ParticleTexture* textureDarkGreenTrail;
    static ParticleTexture* textureOrangeTrail;
    static ParticleTexture* textureRedTrail;
    static ParticleTexture* textureSplash;
    static ParticleTexture* textureBubble;
    static ParticleTexture* textureExplosion1;
    static ParticleTexture* textureExplosion2;
    static ParticleTexture* textureExplosion3;
    static ParticleTexture* textureBlackFade;
    static ParticleTexture* textureBlackFadeOut;
    static ParticleTexture* textureTear1;
    static ParticleTexture* textureTear2;
    static ParticleTexture* textureWhiteFadeOutAndIn;
    static ParticleTexture* textureBlackFadeOutAndIn;
    static ParticleTexture* textureInWater;
    static ParticleTexture* textureBlueLine;
    static ParticleTexture* texturePollen;

    static void loadParticles();
};
#endif
