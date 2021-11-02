#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "emeraldpiece.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"
#include "emeraldmanager.hpp"
#include "../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> EmeraldPiece::models;

float EmeraldPiece::baseScale = 1.0f;
float EmeraldPiece::lastPieceCollectedTimestamp = 0.0f;

EmeraldPiece::EmeraldPiece()
{

}

EmeraldPiece::EmeraldPiece(
    float x, float y, float z,
    int pieceNumber, int isDiggable,
    int hardModePiece)
{
    position.set(x, y, z);
    this->pieceNumber = pieceNumber;
    this->isDiggable = (isDiggable != 0);
    this->hardModePiece = (hardModePiece != 0);
    collectTimer = 0.0f;
    this->baseAlpha = 0.85f;
    scale = EmeraldPiece::baseScale;
    EmeraldPiece::lastPieceCollectedTimestamp = Global::gameClock;
}

void EmeraldPiece::step()
{
    extern float dt;

    increaseRotation(0, 60*10*dt, 0);

    if (collectTimer == 0)
    {
        bool collected = false;

        if (isDiggable)
        {
            if (Global::gameMainPlayer->canGrabDiggablePiece())
            {
                Vector3f diff = Global::gameMainPlayer->position - position;

                if (diff.lengthSquared() < 40*40)
                {
                    collected = true;
                }
            }
        }
        
        Vector3f diffToPlayer = Global::gameMainPlayer->getCenterPosition() - position;
        if (diffToPlayer.lengthSquared() < (4.0f + 5.37f)*(4.0f + 5.37f)) //4 = emerald hitbox, 5.37 = knuckles hitbox
        {
            collected = true;
        }

        if (collected)
        {
            AudioPlayer::play(35, &position);

            for (int i = 0; i < 10; i++)
            {
                Vector3f pos(
                    getX() + Maths::random() * 8 - 4,
                    getY() + Maths::random() * 8 - 4,
                    getZ() + Maths::random() * 8 - 4);
            
                Vector3f vel(Maths::random()-0.5f, 0.4f, Maths::random()-0.5f);
                vel.scale(60);

                ParticleMaster::createParticle(ParticleResources::textureSparkleGreen, &pos, &vel,
                    0.025f*60*60, 0.5f, 0, 7, -(7.0f / 0.5f), false, false, 0.3f, true);
            }

            EmeraldManager::collectPiece(this);
            collectTimer = collectTimerMax;

            Global::gameScore += std::max(0, 2000 - 20*((int)(Global::gameClock - EmeraldPiece::lastPieceCollectedTimestamp)));

            EmeraldPiece::lastPieceCollectedTimestamp = Global::gameClock;
        }
    }
    else
    {
        position = Global::gameMainPlayer->getCenterPosition();
        position.y += 10;

        if (collectTimer <= collectTimerMax/2)
        {
            scale = EmeraldPiece::baseScale - EmeraldPiece::baseScale*(((collectTimerMax/2)-collectTimer))/(collectTimerMax/2);
        }

        collectTimer-=dt;
        if (collectTimer <= 0)
        {
            Global::deleteEntity(this);
            return;
        }
    }

    updateTransformationMatrix();
}

Vector3f EmeraldPiece::getHintLocation()
{
    hintCameraCenter = position;
    if (!isDiggable)
    {
        Vector3f dir(1000, 0, 0);
        Vector3f yAxis(0, 1, 0);
        Vector3f shortestDir(&dir);

        for (int i = 0; i < 8; i++)
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
            dir = Maths::rotatePoint(&dir, &yAxis, (2*Maths::PI)/8);
        }

        if (shortestDir.lengthSquared() >= 999*999) //no walls were found, make a completely random point
        {
            Vector3f off = Maths::randomPointOnSphere();
            off.y = 0;
            off.setLength(5.0f);
            return position + off;
        }
        else //we found a wall that was close, put it somewhere in that direction
        {
            float range = Maths::PI*0.9f;
            shortestDir.setLength(5.0f);
            shortestDir = Maths::rotatePoint(&shortestDir, &yAxis, Maths::random()*range - range/2);

            return position + shortestDir;
        }
    }
    else
    {
        Vector3f yAxis(0, 1, 0);
        Vector3f shortestDir(2000, 0, 0);

        Vector3f dir(1000, -1000, 0);
        for (int i = 0; i < 8; i++)
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
            dir = Maths::rotatePoint(&dir, &yAxis, (2*Maths::PI)/8);
        }

        dir.set(1000, 0, 0);
        for (int i = 0; i < 8; i++)
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
            dir = Maths::rotatePoint(&dir, &yAxis, (2*Maths::PI)/8);
        }

        dir.set(1000, 1000, 0);
        for (int i = 0; i < 8; i++)
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
            dir = Maths::rotatePoint(&dir, &yAxis, (2*Maths::PI)/8);
        }

        dir.set(0, -1000, 0);
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
        }

        dir.set(0, 1000, 0);
        {
            Vector3f checkPos = position + dir;
            if (CollisionChecker::checkCollision(&position, &checkPos))
            {
                Vector3f collidePos(CollisionChecker::getCollidePosition());
                Vector3f thisDir = collidePos - position;
                if (thisDir.lengthSquared() < shortestDir.lengthSquared())
                {
                    shortestDir = thisDir;
                }
            }
        }

        if (shortestDir.lengthSquared() >= 1999*1999) //no walls were found, make a completely random point. shouldnt be diggable but whatever
        {
            Vector3f off = Maths::randomPointOnSphere();
            off.y = 0;
            off.setLength(5.0f);
            return position + off;
        }
        else //we found a wall that was close, put it somewhere in that direction
        {
            Vector3f newCenter = position + shortestDir;
            Vector3f perpen = Maths::calculatePerpendicular(&shortestDir);
            perpen.normalize();
            shortestDir.normalize();

            Vector3f newDir = Maths::rotatePoint(&shortestDir, &perpen, Maths::PI/2);
            newDir = Maths::rotatePoint(&newDir, &shortestDir, Maths::random()*2*Maths::PI);

            newDir.setLength(20.0f);

            hintCameraCenter = newCenter + shortestDir.scaleCopy(2.0f);

            return hintCameraCenter + newDir + shortestDir.scaleCopy(5.0f);
        }
    }
}

int EmeraldPiece::getPieceNumber()
{
    return pieceNumber;
}

bool EmeraldPiece::isEmeraldPiece() const
{
    return true;
}

bool EmeraldPiece::isHardModePiece()
{
    return hardModePiece;
}

std::list<TexturedModel*>* EmeraldPiece::getModels()
{
    return &EmeraldPiece::models;
}

void EmeraldPiece::loadStaticModels()
{
    if (EmeraldPiece::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading EmeraldPiece static models...\n");
    #endif

    ModelLoader::loadModel(&EmeraldPiece::models, "res/Models/Objects/Emerald/", "Emerald");
}

void EmeraldPiece::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting EmeraldPiece static models...\n");
    #endif

    Entity::deleteModels(&EmeraldPiece::models);
}
