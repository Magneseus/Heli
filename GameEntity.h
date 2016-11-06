#pragma once

#include <Ogre.h>

class GameEntity
{
public:
	GameEntity(Ogre::SceneManager*, Ogre::SceneNode*);
	~GameEntity();

	Ogre::SceneNode* getSceneNode();
	Ogre::SceneNode* setSceneNode(Ogre::SceneNode*);

protected:

private:
	Ogre::SceneManager* scnMan;
	Ogre::SceneNode* model;

};