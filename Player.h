#pragma once

#include "GameEntity.h"

class Player : public GameEntity
{
public:
	Player(Ogre::SceneManager*, Ogre::SceneNode*);
	~Player();

	void update(Ogre::Real& deltaTime);

	Ogre::SceneNode* getFPCameraNode();
	Ogre::SceneNode* getTPCameraNode();

private:
	Ogre::SceneNode* FPcameraNode;
	Ogre::SceneNode* TPcameraNode;
	Ogre::SceneNode* mainRotor;
	Ogre::SceneNode* rearRotor;
};