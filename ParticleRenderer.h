#pragma once

#include <Ogre.h>
#include <iostream>


class Particle
{
public:
	Particle(Ogre::SceneManager* _scnMan, Ogre::String particleType, Ogre::Real _lifetime, Ogre::Real _starttime, Ogre::Vector3 vec1, Ogre::Vector3 vec2);
	~Particle();

	inline Ogre::String getParticleMaterialName() { return particleMaterialName; }
	inline bool getExpired(Ogre::Real time) { return (time - startTimer) > lifetime; }

private:
	Ogre::SceneManager* scnMan;
	Ogre::SceneNode* pnode;
	Ogre::Real startTimer;
	Ogre::Real lifetime;
	Ogre::String particleMaterialName;
};

class ParticleRenderer
{
public:
	ParticleRenderer(Ogre::SceneManager* _scnMan);

	void update(const Ogre::Real& deltaTime);
	void addParticle(Ogre::String particleType, Ogre::Real time, Ogre::Vector3 vec1, Ogre::Vector3 vec2);

private:
	Ogre::Real timer;
	std::vector<Particle*> particles;
	Ogre::SceneManager* scnMan;
};