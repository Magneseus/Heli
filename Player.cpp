#include "Player.h"


Player::Player(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode)
	: GameEntity(_scnMan, _scnNode)
{
	// Add the helicopter mesh to the player's node
	Ogre::Entity* tmpEnt = scnMan->createEntity("Heli.mesh");
	model->attachObject(tmpEnt);

	// Attach the main and rear rotor nodes
	mainRotor = model->createChildSceneNode();
	tmpEnt = scnMan->createEntity("Heli_Rotor.mesh");
	mainRotor->attachObject(tmpEnt);
	mainRotor->setPosition(Ogre::Real(0.025), Ogre::Real(1.520), Ogre::Real(0.055));

	rearRotor = model->createChildSceneNode();
	tmpEnt = scnMan->createEntity("Heli_Rear_Rotor.mesh");
	rearRotor->attachObject(tmpEnt);
	rearRotor->setPosition(Ogre::Real(0.100), Ogre::Real(2.290), Ogre::Real(10.420));

	model->setPosition(0, 0, 0);

	// Create the camera nodes
	FPcameraNode = model->createChildSceneNode();
	FPcameraNode->setInheritOrientation(true);
	FPcameraNode->translate(Ogre::Vector3(0.0, 1.0, -10.0));

	TPcameraNode = model->createChildSceneNode();
	TPcameraNode->setInheritOrientation(false);
	
	// Movement vars
	PitchDir = 0;
	RollDir = 0;
	YawDir = 0;
	LiftDir = 0;

	velocity = Ogre::Vector3(0.0, 0.0, 0.0);
	acceleration = Ogre::Real(5.0);
	rotAcceleration = Ogre::Real(1.0);

	//pitchAxis = Ogre::Vector3::UNIT_X;
	//rollAxis = Ogre::Vector3::UNIT_Z;
	//yawAxis = Ogre::Vector3::UNIT_Y;
	
	//curPitch = Ogre::Radian(Ogre::Real(0.0));
	//curRoll = Ogre::Radian(Ogre::Real(0.0));
	//curYaw = Ogre::Radian(Ogre::Real(0.0));
}


Player::~Player()
{

}



void Player::update(Ogre::Real& deltaTime)
{
	// Rotating the rotors
	Ogre::Real mut(20.0);

	Ogre::Quaternion mainRotorQuat;
	mainRotorQuat.FromAngleAxis(Ogre::Radian(deltaTime * mut), Ogre::Vector3(0, 1, 0));

	Ogre::Quaternion rearRotorQuat;
	rearRotorQuat.FromAngleAxis(Ogre::Radian(deltaTime * mut), Ogre::Vector3(1, 0, 0));

	mainRotor->rotate(mainRotorQuat);
	rearRotor->rotate(rearRotorQuat);


	 
	/*                          Movement                                     */
	{
		// Set the momentum
		Ogre::Vector3 accel = Ogre::Vector3(0.0, 0.0, 0.0);
		accel.y += acceleration * deltaTime * LiftDir;
		accel.y = Ogre::Math::Clamp(accel.y, minLift, maxLift);

		// Generate the rotation quaternions
		Ogre::Quaternion _q;
		_q.FromAngleAxis(
			Ogre::Radian(rotAcceleration * deltaTime * PitchDir),
			Ogre::Vector3::UNIT_X);
		orientationQ = orientationQ * _q;

		_q.FromAngleAxis(
			Ogre::Radian(rotAcceleration * deltaTime * RollDir),
			Ogre::Vector3::UNIT_Z);
		orientationQ = orientationQ * _q;

		_q.FromAngleAxis(
			Ogre::Radian(rotAcceleration * deltaTime * YawDir),
			Ogre::Vector3::UNIT_Y);
		orientationQ = orientationQ * _q;

		// Always be moving back towards level (except for yaw)
		Ogre::Quaternion levelQ;
		levelQ.FromAngleAxis(orientationQ.getYaw(), Ogre::Vector3::UNIT_Y);

		orientationQ = Ogre::Quaternion::Slerp(0.002f, orientationQ, levelQ);

		// Calculate the new acceleration
		accel = orientationQ * accel;

		// Apply gravity
		//accel += Ogre::Vector3(0.0, -0.1, 0.0) * deltaTime;

		// Apply to the velocity
		velocity += accel * deltaTime;

		// Slow down the velocity if no acceleration
		if (accel.squaredLength() > Ogre::Real(0.0))
		{
			velocity = Ogre::Math::lerp(
				velocity, 
				Ogre::Vector3(velocity.x, 0.0, velocity.z), 
				Ogre::Real(0.005));
		}

		// Generate the proper quaternions
		/*
		Ogre::Quaternion _pitch;
		curPitch += Ogre::Radian(rotAcceleration * deltaTime * PitchDir);
		curPitch = Ogre::Math::Clamp(curPitch, minPitch, maxPitch);
		_pitch.FromAngleAxis(curPitch, pitchAxis);

		Ogre::Quaternion _roll;
		curRoll += Ogre::Radian(rotAcceleration * deltaTime * RollDir);
		curRoll = Ogre::Math::Clamp(curRoll, minRoll, maxRoll);
		_roll.FromAngleAxis(curRoll, rollAxis);

		Ogre::Quaternion _yaw;
		curYaw += Ogre::Radian(rotAcceleration * deltaTime * YawDir);
		_yaw.FromAngleAxis(curYaw, yawAxis);

		std::cout << "P: " << pitchAxis << std::endl;
		std::cout << "R: " << rollAxis << std::endl;
		std::cout << "Y: " << yawAxis << std::endl << std::endl;


		// Generate the resulting combined Quaternion
		Ogre::Quaternion endQ = _pitch * _roll *_yaw;

		// Calculate the new velocity
		newVel = _pitch * _roll * newVel;
		velocity += newVel;
		// Apply gravity
		//velocity += Ogre::Vector3(0.0, -0.1, 0.0) * deltaTime;

		// Update the axes
		pitchAxis = endQ * Ogre::Vector3::UNIT_X;
		pitchAxis.normalise();
		rollAxis = endQ * Ogre::Vector3::UNIT_Z;
		rollAxis.normalise();
		yawAxis = endQ * Ogre::Vector3::UNIT_Y;
		yawAxis.normalise();
		*/

		// Apply the orientation and translation
		model->setOrientation(orientationQ);
		model->translate(velocity);
	}
}

Ogre::SceneNode* Player::getFPCameraNode() { return FPcameraNode; }
Ogre::SceneNode* Player::getTPCameraNode() { return TPcameraNode; }