#pragma once

#include "Enemy.h"

class Jeep : public Enemy
{
public:
	Jeep(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~Jeep();

	void update(Ogre::Real& deltaTime);

protected:
	virtual void fire();

private:

};