#include "GameEntity.h"

 // Static vars
OgreRay* GameEntity::ORay = NULL;

GameEntity::GameEntity(Ogre::SceneManager* scnMan_, Ogre::SceneNode* scnNode_)
	: scnMan(scnMan_),
	model(scnNode_),
	doesTick(true),
	isCollided(false)
{

}


GameEntity::~GameEntity()
{
	// Destroy the scene node
	scnMan->destroySceneNode(model);
	model = NULL;
	scnMan = NULL;
}



void GameEntity::update(Ogre::Real& deltaTime)
{

}

void GameEntity::onCollide()
{

}


bool GameEntity::collide(GameEntity* otherEnt)
{
	if (otherEnt->col.intersects(this->col))
	{
		otherEnt->onCollide();
		otherEnt->isCollided = true;
		this->onCollide();
		this->isCollided = true;

		return true;
	}

	return false;
}


///////////////////// Getters & Setters ////////////////////////////

Ogre::SceneNode* GameEntity::getSceneNode()
{
	return model;
}

Ogre::SceneNode* GameEntity::setSceneNode(Ogre::SceneNode* newModel)
{
	Ogre::SceneNode* oldModel = model;
	model = newModel;
	return oldModel;
}