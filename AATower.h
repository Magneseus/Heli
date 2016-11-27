#pragma once

#include "Enemy.h"

class AATower : public Enemy
{
public:
	AATower(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~AATower();

	void update(Ogre::Real& deltaTime);

private:

};