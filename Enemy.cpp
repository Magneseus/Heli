#include "Enemy.h"

Enemy::Enemy(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode)
	: GameEntity(_scnMan, _scnNode)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("Tank.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("Tank_Turret.mesh");
	turretNode->attachObject(tmpEnt);


}

Enemy::~Enemy()
{

}

void Enemy::update(Ogre::Real& deltaTime)
{

}