#pragma once

#include <Ogre.h>

class GameEntity
{
public:
	GameEntity(Ogre::SceneManager*, Ogre::SceneNode*);
	virtual ~GameEntity();

	virtual void update(Ogre::Real& deltaTime);

	Ogre::SceneNode* getSceneNode();
	Ogre::SceneNode* setSceneNode(Ogre::SceneNode*);

	bool doesTick;

protected:

private:
	Ogre::SceneManager* scnMan;
	Ogre::SceneNode* model;

};