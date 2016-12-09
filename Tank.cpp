#include "Tank.h"

Tank::Tank(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: Enemy(_scnMan, _scnNode, _PlayerEnt)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("Tank.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("Tank_Turret.mesh");
	turretNode->attachObject(tmpEnt);

	acceleration = Ogre::Real(30.0);
	maxSpeed = Ogre::Real(10.0);
	curSpeed = Ogre::Real(0.0);
	turningSpeed = Ogre::Real(0.5);
	turretTurningSpeed = Ogre::Real(0.6);
	minDistance = Ogre::Real(50.0);
	prefDistance = Ogre::Real(300.0);

	// Collisions vars
	sBox* colBox;

	colBox = new sBox(
		Ogre::Vector3(-2.0, 0.0, -3.5),
		Ogre::Vector3(4.0, 0.0, 0.0),
		Ogre::Vector3(0.0, 2.0, 0.0),
		Ogre::Vector3(0.0, 0.0, 7.0));
	col.addShape(colBox);

	this->hp = 500;
}

Tank::~Tank()
{

}

void Tank::fire()
{

}

void Tank::update(Ogre::Real& deltaTime)
{
	Enemy::update(deltaTime);

	// Update collision boxes
	col.setPosition(model->_getDerivedPosition());
	col.setOrientation(model->_getDerivedOrientation());

	// check if we're colliding and display
	Ogre::ColourValue Ccolor = Ogre::ColourValue::Green;
	if (isCollided)
		Ccolor = Ogre::ColourValue::Red;

	col.DebugDraw(Ccolor);
}