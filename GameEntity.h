#pragma once

#include <Ogre.h>
#include "DebugDrawer.h"
#include "OgreRay.h"
#include "Collision.h"

class GameEntity
{
public:
	static OgreRay* ORay;

public:
	GameEntity(Ogre::SceneManager*, Ogre::SceneNode*);
	virtual ~GameEntity();

	virtual void update(Ogre::Real& deltaTime);
	
	bool collide(GameEntity* otherEnt);
	virtual void onCollide(GameEntity* otherEnt, Ogre::String tag);

	Ogre::SceneNode* getSceneNode();
	Ogre::SceneNode* setSceneNode(Ogre::SceneNode*);

	bool doesTick;
	bool isCollided;

	Ogre::String tag;

protected:
	Ogre::SceneNode* model;
	Ogre::SceneManager* scnMan;

	CollisionBounds col;

private:
};