#pragma once

#include "GameEntity.h"
#include "OGRE/OgreMath.h"

class Enemy : public GameEntity
{
public:
	Enemy(Ogre::SceneManager*, Ogre::SceneNode*);
	~Enemy();

	void update(Ogre::Real& deltaTime);

private:
	Ogre::SceneNode* turretNode;
};