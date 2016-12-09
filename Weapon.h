#pragma once

#include <Ogre.h>

class Weapon
{
public:
	Weapon(Ogre::SceneManager* scnMan);
	virtual ~Weapon() = 0;

	virtual void fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) = 0;

protected:
	Ogre::SceneManager* scnMan;

	Ogre::Real fireCooldown;
	Ogre::Real fireTime;

};