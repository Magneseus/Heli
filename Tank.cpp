#include "Tank.h"

Tank::Tank(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: Enemy(_scnMan, _scnNode, _PlayerEnt)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("Tank.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("Tank_Turret.mesh");
	turretNode->attachObject(tmpEnt);

	acceleration = Ogre::Real(30.0);
	maxSpeed = Ogre::Real(10.0);
	curSpeed = Ogre::Real(0.0);
	turningSpeed = Ogre::Real(0.5);
	turretTurningSpeed = Ogre::Real(0.6);
	minDistance = Ogre::Real(50.0);
	prefDistance = Ogre::Real(300.0);
}

Tank::~Tank()
{

}

void Tank::fire()
{

}

void Tank::update(Ogre::Real& deltaTime)
{
	Enemy::update(deltaTime);
}