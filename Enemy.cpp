#include "Enemy.h"

Enemy::Enemy(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: GameEntity(_scnMan, _scnNode),
	PlayerEnt(_PlayerEnt),
	acceleration(0.0),
	maxSpeed(0.0),
	curSpeed(0.0),
	turningSpeed(0.0),
	turretTurningSpeed(0.0),
	minDistance(-1.0),
	prefDistance(1.0),
	timeOutWhenCaughtUp(0.0),
	timerC(0.0),
	timeOutC(false)
{
}

Enemy::~Enemy()
{

}

void Enemy::update(Ogre::Real& deltaTime)
{
	// Track the player and head towards them
	Ogre::Vector3 distToPlayer = PlayerEnt->getSceneNode()->_getDerivedPosition() - model->_getDerivedPosition();
	Ogre::Vector3 planarDistToPlayer = distToPlayer;
	planarDistToPlayer.y = Ogre::Real(0.0);

	Ogre::Quaternion orient = model->getOrientation();
	Ogre::Vector3 fwdVec = -orient.zAxis();
	fwdVec.y = Ogre::Real(0.0);
	fwdVec.normalise();

	// Check if we've reached the timeout limit
	if (timeOutC) timerC += deltaTime;
	if (timerC > timeOutWhenCaughtUp)
	{
		timeOutC = false;
	}

	// Check if we're outside the preferred distance band
	if (!timeOutC && 
		prefDistance > Ogre::Real(0.0) &&
		planarDistToPlayer.squaredLength() < minDistance * minDistance ||
		planarDistToPlayer.squaredLength() > prefDistance * prefDistance)
	{
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
			endOrient.FromAngleAxis(Ogre::Radian(-angDir * turningSpeed * deltaTime), 
				Ogre::Vector3::UNIT_Y);

			// Rotate in that direction
			orient = orient * endOrient;

			model->setOrientation(orient);
		}


		// After rotating we can move our enemy
		Ogre::Real dir = planarDistToPlayer.squaredLength() < minDistance*minDistance ? Ogre::Real(-1.0) : Ogre::Real(1.0);

		// Increase our speed accordingly
		curSpeed += acceleration * deltaTime * dir;
		curSpeed = Ogre::Math::Clamp(curSpeed, -maxSpeed, maxSpeed);

		// Apply velocity
		model->translate(fwdVec * curSpeed * deltaTime);
	}
	else
	{
		// If we've reached a point within the preferred band slow down to a stop
		Ogre::Real _dirMult = curSpeed < Ogre::Real(0.0) ? Ogre::Real(1.0) : Ogre::Real(-1.0);
		curSpeed += acceleration * deltaTime * _dirMult;

		// If we're close to 0 just stop
		if (!timeOutC && abs(curSpeed) < Ogre::Real(0.2))
		{
			curSpeed = Ogre::Real(0.0);
			timeOutC = true;
			timerC = Ogre::Real(0.0);
		}

		model->translate(fwdVec * curSpeed * deltaTime);
	}


	// Rotate the turret
	Ogre::Quaternion turOrient = turretNode->_getDerivedOrientation();
	
	Ogre::Vector3 dirLook = -distToPlayer.normalisedCopy();
	Ogre::Vector3 right = Ogre::Vector3(dirLook.z, .0, -dirLook.x);
	right.normalise();
	Ogre::Vector3 up = dirLook.crossProduct(right);
	Ogre::Quaternion endTurOrient(right, up, dirLook);

	
	turOrient = Ogre::Quaternion::Slerp(
		turretTurningSpeed * deltaTime,
		turOrient,
		endTurOrient,
		true);

	turretNode->_setDerivedOrientation(turOrient);
}