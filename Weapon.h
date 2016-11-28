#pragma once

#include <Ogre.h>

class Weapon
{
public:
	Weapon(Ogre::SceneManager* scnMan);
	virtual ~Weapon() = 0;

	virtual void fire(Ogre::Real& deltaTime) = 0;

protected:
	Ogre::SceneManager* scnMan;

	Ogre::Real fireCooldown;
	Ogre::Real fireTime;

};