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

	// Attach the minigun node
	minigunNode = model->createChildSceneNode();
	tmpEnt = scnMan->createEntity("Minigun.mesh");
	minigunNode->attachObject(tmpEnt);
	minigunNode->translate(Ogre::Vector3(0.0, -0.7, -3.0));

	model->setPosition(0, 0, 0);

	minigunRotation = Ogre::Real(Ogre::Math::PI);

	// Create the camera nodes
	FPcameraNode = model->createChildSceneNode();
	FPcameraNode->setInheritOrientation(true);
	FPcameraNode->translate(Ogre::Vector3(-0.7, 0.7, -3.0));

	TPcameraNode = model->createChildSceneNode();
	TPcameraNode->setInheritOrientation(false);
	
	// Movement vars
	PitchDir = 0;
	RollDir = 0;
	YawDir = 0;
	LiftDir = 0;

	velocity = Ogre::Vector3(0.0, 0.0, 0.0);
	acceleration = Ogre::Real(800.0);
	rotAcceleration = Ogre::Real(1.0);



	// Collision values

	// TODO REMOVE THIS
	colBox2 = new sBox(
		Ogre::Vector3(-0.5, -0.8, 3.0),
		Ogre::Vector3(1.0, 0.0, 0.0),
		Ogre::Vector3(0.0, 1.5, 0.0),
		Ogre::Vector3(0.0, 0.0, 7.0));
	col.addShape(colBox2);

	colBox = new sBox(
		Ogre::Vector3(-1.5, -1.0, -3.0),
		Ogre::Vector3(3.0, 0.0, 0.0),
		Ogre::Vector3(0.0, 2.5, 0.0),
		Ogre::Vector3(0.0, 0.0, 6.0));

	col.addShape(colBox);

	colSphere = new sSphere(Ogre::Sphere(Ogre::Vector3(-4.0, 0.0, 0.0), sReal(2.0)));
	col.addShape(colSphere);

	// END OF TESTING
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


	// Rotating the Gatling gun
	
	// Get the quaternion representing our current look location
	Ogre::Quaternion lookQ;
	Ogre::Quaternion curQ;
	if (curCamMode == CameraMode::FirstPerson)
	{
		curQ = minigunNode->getOrientation();
		lookQ = FPcameraNode->getOrientation();

		curQ = Ogre::Quaternion::Slerp(
			minigunRotation * deltaTime,
			curQ,
			lookQ,
			true);

		minigunNode->setOrientation(curQ);
	}
	else if (curCamMode == CameraMode::ThirdPerson)
	{
		curQ = minigunNode->_getDerivedOrientation();
		lookQ = TPcameraNode->_getDerivedOrientation();

		curQ = Ogre::Quaternion::Slerp(
			minigunRotation * deltaTime,
			curQ,
			lookQ,
			true);

		minigunNode->_setDerivedOrientation(curQ);
	}

	/*                          Movement                                     */
	{
		// Set the momentum
		Ogre::Vector3 accel = Ogre::Vector3(0.0, 0.0, 0.0);
		accel.y += acceleration * deltaTime;
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

		orientationQ = Ogre::Quaternion::Slerp(0.5f * deltaTime, orientationQ, levelQ, true);

		// Calculate the new acceleration
		accel = orientationQ * accel;
		accel.y *= LiftDir;

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
				Ogre::Real(0.005) * deltaTime);
		}

		// Apply the orientation and translation
		model->setOrientation(orientationQ);
		model->translate(velocity * deltaTime);
	}

	// Update collision boxes
	col.setPosition(model->_getDerivedPosition());
	col.setOrientation(model->_getDerivedOrientation());

	col.DebugDraw(Ogre::ColourValue::Green);


	// TODO: Remove this code
	// TESTING 
	Ogre::Vector3 result;
	Ogre::MovableObject* resultObj = NULL;
	std::vector<Ogre::MovableObject*> igList;
	igList.push_back(minigunNode->getAttachedObject(0));
	igList.push_back(model->getAttachedObject(0));
	if (ORay->RaycastFromPoint(minigunNode->_getDerivedPosition(), -minigunNode->_getDerivedOrientation().zAxis(), result, igList, resultObj)) {
		printf("Your mouse is over the position %f,%f,%f\n", result.x, result.y, result.z);
	}
	else {
		printf("No mouse collision\n Are you looking the sky ?\n");
	}

	DebugDrawer::getSingleton().drawLine(
		minigunNode->_getDerivedPosition(),
		result,
		Ogre::ColourValue::Red);

	// END OF TESTING

}

Ogre::SceneNode* Player::getFPCameraNode()
{
	curCamMode = CameraMode::FirstPerson;
	return FPcameraNode;
}
Ogre::SceneNode* Player::getTPCameraNode()
{
	curCamMode = CameraMode::ThirdPerson;
	return TPcameraNode;
}