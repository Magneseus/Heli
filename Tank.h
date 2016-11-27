#pragma once

#include "Enemy.h"

class Tank : public Enemy
{
public:
	Tank(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~Tank();

	void update(Ogre::Real& deltaTime);

private:

};