#pragma once

#include "Enemy.h"

class Tank : public Enemy
{
public:
	Tank(Ogre::SceneManager*, Ogre::SceneNode*, GameEntity*);
	~Tank();

	void update(Ogre::Real& deltaTime);
	virtual void fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);

protected:

private:

};