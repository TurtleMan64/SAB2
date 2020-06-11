#include "controllableplayer.h"
#include "../toolbox/vector.h"
#include "shieldgreen.h"
#include "shieldmagnet.h"

void          ControllablePlayer::goUp()                                   {}
void          ControllablePlayer::jump()                                   {}
void          ControllablePlayer::hitDashpad()                             {}
void          ControllablePlayer::animate()                                {}
void          ControllablePlayer::refreshCamera()                          {}
void          ControllablePlayer::setCanMoveTimer(int)                     {}
void          ControllablePlayer::setHoverTimer(float)                     {}
float         ControllablePlayer::getHoverTimer()                          {return 0;}
void          ControllablePlayer::setInWater(float)                        {}
void          ControllablePlayer::takeDamage(Vector3f*)                    {}
void          ControllablePlayer::rebound(Vector3f*)                       {}
bool          ControllablePlayer::isVulnerable()                           {return false;}
bool          ControllablePlayer::canDealDamage()                          {return false;}
bool          ControllablePlayer::canBreakObjects()                        {return false;}
void          ControllablePlayer::die()                                    {}
void          ControllablePlayer::startGrinding()                          {}
void          ControllablePlayer::stopGrinding()                           {}
bool          ControllablePlayer::isDying()                                {return false;}
Vector3f      ControllablePlayer::getCenterPosition()                      {return Vector3f(0,0,0);}
ShieldMagnet* ControllablePlayer::getShieldMagnet()                        {return nullptr;}
void          ControllablePlayer::setShieldMagnet(ShieldMagnet*)           {}
ShieldGreen*  ControllablePlayer::getShieldGreen()                         {return nullptr;}
void          ControllablePlayer::setShieldGreen(ShieldGreen*)             {}
bool          ControllablePlayer::canGrabDiggablePiece()                   {return true;}
bool          ControllablePlayer::canReadHint()                            {return false;}
void          ControllablePlayer::setReadingHint(bool)                     {}
void          ControllablePlayer::increaseCombo()                          {}
void          ControllablePlayer::setInvincibleTimer(float)                {}
void          ControllablePlayer::setSpeedshoesTimer(float)                {}
void          ControllablePlayer::startGrabbing()                          {}
void          ControllablePlayer::stopGrabbing()                           {}
float         ControllablePlayer::getDiggingTimer()                        {return 0.0f;}
void          ControllablePlayer::hitSpring(Vector3f*, float, float, bool) {}
void          ControllablePlayer::hitSpringTriple(Vector3f*, float, float) {}
void          ControllablePlayer::hitSpeedRamp(Vector3f*, float, float)    {}
void          ControllablePlayer::jumpOffPulley(Vector3f)                  {}
