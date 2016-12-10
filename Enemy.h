#pragma once

#include "GameEntity.h"
#include "OGRE/OgreMath.h"

class Enemy : public GameEntity
{
public:
	Enemy(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	virtual ~Enemy();

	virtual void update(Ogre::Real& deltaTime);
	virtual void fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) = 0;

	std::vector<GameEntity*>* entlist;

protected:
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

	// firing mechanics
	Ogre::Real fireCounter;
	Ogre::Real fireStopTime;
	Ogre::Real fireCooldown;
	bool timeOutF;

	// Health
	int hp;
	virtual void onCollide(GameEntity* otherEnt, Ogre::String tag);

private:
	
};