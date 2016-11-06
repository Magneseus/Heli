#include "Shapes.h"

void CreateCubeEntity(Ogre::SceneManager* mScn, Ogre::String cb_name)
{
try
{
	// Make the new cube object
	Ogre::ManualObject* newCube = NULL;
	newCube = mScn->createManualObject(cb_name);
	newCube->setDynamic(false);

	newCube->begin("ObjectMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	// Colour param
	Ogre::ColourValue vertex_colour = Ogre::ColourValue(0.85f, 0.2f, 0.5f);


	// #1
	newCube->position(    Ogre::Vector3(-1, -1, -1));
	newCube->normal(      Ogre::Vector3(-1, -1, -1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #1
	newCube->position    (Ogre::Vector3(-1, -1, 1));
	newCube->normal(      Ogre::Vector3(-1, -1, 1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #2
	newCube->position(    Ogre::Vector3(-1, 1, -1));
	newCube->normal(      Ogre::Vector3(-1, 1, -1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #3
	newCube->position(    Ogre::Vector3(-1, 1, 1));
	newCube->normal(      Ogre::Vector3(-1, 1, 1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #4
	newCube->position(    Ogre::Vector3(1, -1, -1));
	newCube->normal(      Ogre::Vector3(1, -1, -1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #5
	newCube->position(    Ogre::Vector3(1, -1, 1));
	newCube->normal(      Ogre::Vector3(1, -1, 1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #7
	newCube->position(    Ogre::Vector3(1, 1, -1));
	newCube->normal(      Ogre::Vector3(1, 1, -1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);

	// #7
	newCube->position(    Ogre::Vector3(1, 1, 1));
	newCube->normal(      Ogre::Vector3(1, 1, 1).normalisedCopy());
	newCube->textureCoord(Ogre::Vector2(0, 0));
	//newCube->colour(vertex_colour);


	// Now add the triangles
	newCube->triangle(0, 1, 2);
	newCube->triangle(2, 1, 3);

	newCube->triangle(2, 6, 0);
	newCube->triangle(6, 4, 0);
	
	newCube->triangle(0, 4, 1);
	newCube->triangle(4, 5, 1);

	newCube->triangle(4, 6, 5);
	newCube->triangle(6, 7, 5);

	newCube->triangle(1, 5, 3);
	newCube->triangle(5, 7, 3);

	newCube->triangle(3, 7, 2);
	newCube->triangle(7, 6, 2);


	// Finish the mesh
	newCube->end();
	newCube->convertToMesh(cb_name);
}
catch (Ogre::Exception &e) {
	throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
}
catch (std::exception &e) {
	throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
}
}