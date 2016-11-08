#include "Helicopter.h"

Helicopter::Helicopter(Ogre::SceneManager* scnMan_, Ogre::SceneNode* scnNode_)
	: GameEntity(scnMan_, scnNode_),
	acceleration(0.0),
	velocity(0.0, 0.0, 0.0)
{
}

Helicopter::~Helicopter()
{
	~GameEntity();
}


void Helicopter::update(Ogre::Real deltaTime)
{
	// Update position, velocity and acceleration
}


void Helicopter::Pitch(Ogre::Real amnt)
{

}

void Helicopter::Pitch(Ogre::Real amnt)
{

}

void Helicopter::Pitch(Ogre::Real amnt)
{

}

void Helicopter::Pitch(Ogre::Real amnt)
{

}