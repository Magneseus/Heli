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


}

Tank::~Tank()
{

}

void Tank::update(Ogre::Real& deltaTime)
{

}