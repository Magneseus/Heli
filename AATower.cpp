#include "AATower.h"

AATower::AATower(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: Enemy(_scnMan, _scnNode, _PlayerEnt)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("AATower.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("AATower_Turret.mesh");
	turretNode->attachObject(tmpEnt);
	turretNode->setPosition(Ogre::Vector3(0.0, 0.85261, 0.0));

	prefDistance = Ogre::Real(-1.0);
	turretTurningSpeed = Ogre::Real(0.5);

	// Collisions vars
	sBox* colBox;

	colBox = new sBox(
		Ogre::Vector3(-2.0, 0.0, -2.0),
		Ogre::Vector3(4.0, 0.0, 0.0),
		Ogre::Vector3(0.0, 4.0, 0.0),
		Ogre::Vector3(0.0, 0.0, 4.0));
	col.addShape(colBox);

	this->hp = 200;
}

AATower::~AATower()
{

}

void AATower::fire()
{

}

void AATower::update(Ogre::Real& deltaTime)
{
	//Enemy::update(deltaTime);

	// Track the player and head towards them
	Ogre::Vector3 distToPlayer = PlayerEnt->getSceneNode()->_getDerivedPosition() - model->_getDerivedPosition();
	Ogre::Vector3 planarDistToPlayer = distToPlayer;
	planarDistToPlayer.y = Ogre::Real(0.0);

	Ogre::Quaternion orient = turretNode->getOrientation();
	Ogre::Vector3 fwdVec = -orient.zAxis();
	fwdVec.y = Ogre::Real(0.0);
	fwdVec.normalise();

	// If so, we need to rotate and move the enemy
	Ogre::Vector2 a(fwdVec.x, fwdVec.z);
	Ogre::Vector2 b(planarDistToPlayer.x, planarDistToPlayer.z);
	Ogre::Real angDir = a.x * b.y - a.y * b.x; angDir /= abs(angDir);
	Ogre::Radian angBet = a.angleTo(b);

	// If we still need to rotate
	if (angBet > Ogre::Radian(0.1))
	{
		// Generate the orientation we want to reach
		Ogre::Quaternion endOrient;
		endOrient.FromAngleAxis(
			Ogre::Radian(-angDir * turretTurningSpeed * deltaTime),
			Ogre::Vector3::UNIT_Y);

		// Rotate in that direction
		orient = orient * endOrient;

		turretNode->setOrientation(orient);
	}

	// Update collision boxes
	col.setPosition(model->_getDerivedPosition());
	col.setOrientation(model->_getDerivedOrientation());

	// check if we're colliding and display
	Ogre::ColourValue Ccolor = Ogre::ColourValue::Green;
	if (isCollided)
		Ccolor = Ogre::ColourValue::Red;

	col.DebugDraw(Ccolor);
}