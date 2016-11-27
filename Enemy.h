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
	GameEntity* PlayerEnt;
	Ogre::SceneNode* turretNode;

private:
	
};