#pragma once

#include "GameEntity.h"

class Player : public GameEntity
{
public:
	Player(Ogre::SceneManager*, Ogre::SceneNode*);
	~Player();

	void update(Ogre::Real& deltaTime);

private:
	Ogre::SceneNode* mainRotor;
	Ogre::SceneNode* rearRotor;
};