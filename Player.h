#pragma once

#define _PI 3.14159

#include "GameEntity.h"
#include "Weapon.h"
#include "OGRE/OgreMath.h"

class Player : public GameEntity
{
public:
	Player(Ogre::SceneManager*, Ogre::SceneNode*);
	~Player();

	void update(Ogre::Real& deltaTime);

	Ogre::SceneNode* getFPCameraNode();
	Ogre::SceneNode* getTPCameraNode();

	// Movement instructions
	int YawDir;
	int PitchDir;
	int RollDir;
	int LiftDir;

private:
	// Movement vectors
	Ogre::Vector3 velocity;
	Ogre::Real acceleration;
	Ogre::Real rotAcceleration;

	Ogre::Quaternion orientationQ;

	// Collision vars
	//TODO TEMP
	sBox* colBox;
	sBox* colBox2;

	/*
	// Rotation angles & vectors
	Ogre::Radian curPitch;
	Ogre::Radian curRoll;
	Ogre::Radian curYaw;

	Ogre::Vector3 pitchAxis;
	Ogre::Vector3 rollAxis;
	Ogre::Vector3 yawAxis;
	*/

	// Movement binding values
	Ogre::Radian maxPitch = Ogre::Radian(Ogre::Real(_PI / 8.0));
	Ogre::Radian minPitch = Ogre::Radian(Ogre::Real(-_PI / 4.0));
	Ogre::Radian maxRoll = Ogre::Radian(Ogre::Real(_PI / 2.1));
	Ogre::Radian minRoll = Ogre::Radian(Ogre::Real(-_PI / 2.1));
	Ogre::Real maxLift = Ogre::Real(10.0);
	Ogre::Real minLift = Ogre::Real(-10.0);
	

	// Scene nodes for sub-objects
	Ogre::SceneNode* FPcameraNode;
	Ogre::SceneNode* TPcameraNode;
	enum CameraMode { FirstPerson, ThirdPerson, COUNT }; // This is bad design
	CameraMode curCamMode;

	Ogre::SceneNode* mainRotor;
	Ogre::SceneNode* rearRotor;

	Ogre::SceneNode* minigunNode;
	Ogre::Real minigunRotation;
};