#pragma once

#include "Enemy.h"

class AATower : public Enemy
{
public:
	AATower(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~AATower();

	void update(Ogre::Real& deltaTime);
	virtual void fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);

protected:

private:

};