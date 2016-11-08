#pragma once

#include "GameEntity.h"


class Helicopter : public GameEntity
{
public:
	Helicopter();
	~Helicopter();

	void PitchUp(Ogre::Real);
	void PitchDown(Ogre::Real);
	void RollLeft(Ogre::Real);
	void RollRight(Ogre::Real);
	void YawLeft(Ogre::Real);
	void YawRight(Ogre::Real);
	
	void LiftUp(Ogre::Real);
	void LiftDown(Ogre::Real);

	virtual void update(Ogre::Real deltaTime);

protected:
	Ogre::Real acceleration;
	Ogre::Vector3 velocity;

private:

};