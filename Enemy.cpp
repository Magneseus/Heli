#include "Enemy.h"

Enemy::Enemy(Ogre::SceneManager* _scnMan, Ogre::SceneNode* _scnNode, GameEntity* _PlayerEnt)
	: GameEntity(_scnMan, _scnNode),
	PlayerEnt(_PlayerEnt)
{

}

Enemy::~Enemy()
{

}

void Enemy::update(Ogre::Real& deltaTime)
{

}