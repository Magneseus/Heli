#include "Jeep.h"

Jeep::Jeep(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: Enemy(_scnMan, _scnNode, _PlayerEnt)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("Jeep.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("Jeep_Turret.mesh");
	turretNode->attachObject(tmpEnt);
	turretNode->setPosition(Ogre::Vector3(0.0, 1.91132, -0.76466));

	acceleration = Ogre::Real(40.0);
	maxSpeed = Ogre::Real(30.0);
	curSpeed = Ogre::Real(0.0);
	turningSpeed = Ogre::Real(2.0);
	turretTurningSpeed = Ogre::Real(0.9);
	minDistance = Ogre::Real(10.0);
	prefDistance = Ogre::Real(200.0);

	timeOutWhenCaughtUp = Ogre::Real(4.0);
}

Jeep::~Jeep()
{

}

void Jeep::update(Ogre::Real& deltaTime)
{
	Enemy::update(deltaTime);
}