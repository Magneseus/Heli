#pragma once

#include "Ogre.h"

/* Our exception type */
class OgreAppException : public std::exception
{
private:
	std::string message_;
public:
	OgreAppException(std::string message) : message_(message) {};
	virtual const char* what() const throw() { return message_.c_str(); };
};


// Shape functions
//void CreateCylinderEntity(Ogre::SceneManager* mScn, Ogre::String cyl_name, float cyl_radius, float cyl_height, int cyl_num_samples);
void CreateCubeEntity(Ogre::SceneManager* mScn, Ogre::String cb_name);