#pragma once

#include "Enemy.h"

class Jeep : public Enemy
{
public:
	Jeep(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~Jeep();

	void update(Ogre::Real& deltaTime);
	virtual void fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);

protected:

private:

};