#pragma once

#include "GameEntity.h"


class Helicopter : public GameEntity
{
public:
	Helicopter();
	~Helicopter();

	void Pitch(Ogre::Real);
	void Roll(Ogre::Real);
	void Yaw(Ogre::Real);
	void Lift(Ogre::Real);

	virtual void update(Ogre::Real deltaTime);

protected:
	Ogre::Real acceleration;
	Ogre::Vector3 velocity;

private:

};