#include "GameEntity.h"

GameEntity::GameEntity(Ogre::SceneManager* scnMan_, Ogre::SceneNode* scnNode_)
	: scnMan(scnMan_),
	model(scnNode_)
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