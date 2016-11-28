#pragma once

#include "GameEntity.h"
#include "OGRE/OgreMath.h"

class Enemy : public GameEntity
{
public:
	Enemy(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	virtual ~Enemy();

	virtual void update(Ogre::Real& deltaTime);

protected:
	// Firing function
	virtual void fire() = 0;

	GameEntity* PlayerEnt;
	Ogre::SceneNode* turretNode;

	// Movement vars
	Ogre::Real acceleration;
	Ogre::Real maxSpeed;
	Ogre::Real curSpeed;
	Ogre::Real turningSpeed;
	Ogre::Real turretTurningSpeed;

	Ogre::Real minDistance;
	Ogre::Real prefDistance;
	Ogre::Real timeOutWhenCaughtUp;
	Ogre::Real timerC;
	bool timeOutC;

private:
	
};