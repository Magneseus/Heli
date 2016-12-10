#include "ParticleRenderer.h"




// Particles

Particle::Particle(Ogre::SceneManager* _scnMan, Ogre::String particleType, Ogre::Real _lifetime,  Ogre::Real _starttime, Ogre::Vector3 vec1, Ogre::Vector3 vec2)
	: scnMan(_scnMan),
	lifetime(_lifetime),
	startTimer(_starttime)
{
	pnode = scnMan->getRootSceneNode()->createChildSceneNode();
	pnode->setPosition(vec1);

	Ogre::ManualObject* manObj = scnMan->createManualObject();
	manObj->setDynamic(false);

	if (particleType == Ogre::String("laser"))
	{
		// Set the material name
		particleMaterialName = Ogre::String("laser_mat");

		/* Create point list for the object */
		manObj->begin("laser_mat", Ogre::RenderOperation::OT_POINT_LIST);

		// Set the end point and timer
		manObj->position(vec2);
		manObj->normal(Ogre::Vector3(startTimer, 0.0, 0.0));

		manObj->end();

		pnode->attachObject(manObj);
	}
	else
	{
		std::cout << "This particle effect does not exist.\n";
	}
}

Particle::~Particle()
{
	scnMan->destroySceneNode(pnode);
	pnode = NULL;
}







// PARTICLE RENDERER

ParticleRenderer::ParticleRenderer(Ogre::SceneManager* _scnMan)
	: scnMan(_scnMan),
	timer(0.0)
{

}

void ParticleRenderer::update(const Ogre::Real& deltaTime)
{
	timer += deltaTime;

	for (auto it = particles.begin(); it != particles.end();)
	{
		Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName((*it)->getParticleMaterialName()));
		mat->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer);

		// If the particle has expired, delete the memory and remove the vector index
		if ((*it)->getExpired(timer))
		{
			delete (*it);
			(*it) = NULL;
			it = particles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ParticleRenderer::addParticle(Ogre::String particleType, Ogre::Real time, Ogre::Vector3 vec1, Ogre::Vector3 vec2)
{
	particles.push_back(new Particle(scnMan, particleType, time, timer, vec1, vec2));
}