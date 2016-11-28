#include "Weapon.h"



Weapon::Weapon(Ogre::SceneManager* _scnMan)
	: scnMan(_scnMan),
	fireTime(0.0),
	fireCooldown(0.0)
{

}

Weapon::~Weapon()
{

}