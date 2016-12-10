#include "Tank.h"

// Tank projectile class
class TankShell : public GameEntity
{
public:
	TankShell(Ogre::SceneManager*, GameEntity*, Ogre::SceneNode*,Ogre::Vector3);

	virtual void update(Ogre::Real& deltaTime);

	virtual void onCollide(GameEntity* otherEnt, Ogre::String tag);

private:
	Ogre::SceneNode* PlayerEnt;
	GameEntity* parent;
	Ogre::Vector3 vel;

	Ogre::Real maxLife;
	Ogre::Real curLife;
};

TankShell::TankShell(Ogre::SceneManager* _scnMan, GameEntity* _parent, Ogre::SceneNode* _node, Ogre::Vector3 _vel)
	: GameEntity(_scnMan, _node)
{
	// Set vars
	parent = _parent;
	vel = _vel;

	maxLife = 20.0;
	curLife = 0.0;

	// Add the mesh
	Ogre::Entity* tmpEnt = _scnMan->createEntity("tank_shell.mesh");
	model->attachObject(tmpEnt);
	model->setScale(0.25, 0.25, 0.25);

	// Set the tag
	tag = Ogre::String("tank_shell");

	// Add the collision box
	sBox* colBox = new sBox(
		Ogre::Vector3(-0.25, -0.25, -0.5),
		Ogre::Vector3(0.5, 0.0, 0.0),
		Ogre::Vector3(0.0, 0.5, 0.0),
		Ogre::Vector3(0.0, 0.0, 1.0));

	col.addShape(colBox);
}


void TankShell::update(Ogre::Real& deltaTime)
{
	curLife += deltaTime;
	if (curLife > maxLife)
	{
		col.clear();
		model->setVisible(false);
	}

	// Update position
	model->setPosition(model->getPosition() + (vel * deltaTime));

	// Update collision boxes
	col.setPosition(model->_getDerivedPosition());
	col.setOrientation(model->_getDerivedOrientation());

	//col.DebugDraw();
}


void TankShell::onCollide(GameEntity* otherEnt, Ogre::String tag)
{
	if (otherEnt != parent)
	{
		col.clear();
		model->setVisible(false);
	}
}






////////////////// TANK CLASS //////////////////////////////

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

	// Firing mechanics
	fireCounter = 0.0;
	fireStopTime = 0.0;
	fireCooldown = 2.0;
	timeOutF = false;

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

void Tank::fire(const Ogre::Real& deltaTime, const Ogre::Vector3& vec1, const Ogre::Vector3& vec2)
{
	if (!timeOutF && entlist != NULL)
	{
		timeOutF = true;
		fireStopTime = fireCounter + fireCooldown;

		// Make the scene node for the bullet
		Ogre::SceneNode* shellNode = scnMan->getRootSceneNode()->createChildSceneNode();
		shellNode->setPosition(turretNode->_getDerivedPosition());
		shellNode->setOrientation(turretNode->_getDerivedOrientation());

		// Calculate velocity
		Ogre::Vector3 velocityS = vec2 - vec1;
		velocityS.normalise();
		velocityS = velocityS * 30.0;

		// Make the tank shell
		TankShell* t = new TankShell(scnMan, this, shellNode, velocityS);
		entlist->push_back(t);
	}
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

	//col.DebugDraw(Ccolor);
}