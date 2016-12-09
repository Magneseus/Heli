#pragma once

#include "GameEntity.h"
#include "Weapon.h"
#include "OGRE/OgreMath.h"

class Enemy : public GameEntity
{
public:
	Enemy(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	virtual ~Enemy();

	virtual void update(Ogre::Real& deltaTime);

protected:
	// Weapon & firing
	Weapon* weapon;

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

	// Health
	int hp;
	virtual void onCollide(GameEntity* otherEnt, Ogre::String tag);

private:
	
};