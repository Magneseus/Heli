#include "Jeep.h"


Jeep::Jeep(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: Enemy(_scnMan, _scnNode, _PlayerEnt)
{
	// Load in the tank mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("Jeep.mesh");
	model->attachObject(tmpEnt);

	// Attach the turret
	turretNode = model->createChildSceneNode();
	tmpEnt = _scnMan->createEntity("Jeep_Turret.mesh");
	turretNode->attachObject(tmpEnt);
	turretNode->setPosition(Ogre::Vector3(0.0, 1.91132, -0.76466));

	acceleration = Ogre::Real(40.0);
	maxSpeed = Ogre::Real(30.0);
	curSpeed = Ogre::Real(0.0);
	turningSpeed = Ogre::Real(2.0);
	turretTurningSpeed = Ogre::Real(1.1);
	minDistance = Ogre::Real(10.0);
	prefDistance = Ogre::Real(200.0);

	timeOutWhenCaughtUp = Ogre::Real(4.0);

	// Firing mechanics
	fireCounter = 0.0;
	fireStopTime = 0.0;
	fireCooldown = 0.1;
	timeOutF = false;

	// Collisions vars
	sBox* colBox;

	colBox = new sBox(
		Ogre::Vector3(-1.5, 0.0, -2.5),
		Ogre::Vector3(3.0, 0.0, 0.0),
		Ogre::Vector3(0.0, 2.0, 0.0),
		Ogre::Vector3(0.0, 0.0, 5.0));
	col.addShape(colBox);

	this->hp = 100;
}

Jeep::~Jeep()
{

}

void Jeep::fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2)
{
	if (!timeOutF)
	{
		timeOutF = true;
		fireStopTime = fireCounter + fireCooldown;

		// Run the hitscan
		Ogre::Vector3 result;
		Ogre::MovableObject* resultObj = NULL;

		std::vector<Ogre::MovableObject*> igList;
		igList.push_back(turretNode->getAttachedObject(0));
		igList.push_back(model->getAttachedObject(0));

		ORay->RaycastFromPoint(turretNode->_getDerivedPosition(), -turretNode->_getDerivedOrientation().zAxis(), result, igList, resultObj);

		// END OF TESTING

	// If we're not pointing at the sky fire a laser effect towards the target
		if (resultObj != NULL)
		{
			PRend->addParticle("laser", 1.0, turretNode->_getDerivedPosition(), result);
		}
		else
		{
			PRend->addParticle("laser", 1.0, turretNode->_getDerivedPosition(),
				turretNode->_getDerivedPosition() + (result * 500.0));
		}

		// If we're actually pointing at something
		if (resultObj != NULL)
		{
			Ogre::SceneNode* s = PlayerEnt->getSceneNode();

			// If we hit the player
			if (s->getAttachedObject(0) == resultObj)
			{
				PlayerEnt->onCollide(this, "elaser");
			}
		}
	}
}

void Jeep::update(Ogre::Real& deltaTime)
{
	Enemy::update(deltaTime);

	// Update collision boxes
	col.setPosition(model->_getDerivedPosition());
	col.setOrientation(model->_getDerivedOrientation());

	// check if we're colliding and display
	Ogre::ColourValue Ccolor = Ogre::ColourValue::Green;
	if (isCollided)
		Ccolor = Ogre::ColourValue::Red;

	//col.DebugDraw(Ccolor);
}