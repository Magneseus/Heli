#pragma once

#include <Ogre.h>

/*
This is a header of functions that will return the point of intersection
of shapes in 3d.
*/

typedef Ogre::Real sReal;

enum ShapeType
{
	SPHERE,
	OBB,
	NONE
};

struct bProjection
{
	sReal min;
	sReal max;
};

// A class representing a virtual collision shape, not to be instantiated
class CollisionShape
{
public:
	virtual bool intersects(CollisionShape* other) = 0;

	ShapeType type;
};

// A class representing a sphere
class sSphere : public CollisionShape
{
public:
	sSphere(Ogre::Sphere);

	virtual bool intersects(CollisionShape* other);

	inline void translate(const Ogre::Vector3& trans)
	{
		sphere.setCenter(sphere.getCenter() + trans);
	}

	inline void rootTranslate(const Ogre::Vector3& trans) { rpos += trans; }
	inline void setRootPosition(Ogre::Vector3 _rpos) { rpos = _rpos; }

	inline void setCenter(Ogre::Vector3 _center) { sphere.setCenter(_center); }
	inline const Ogre::Vector3& getCenter() { return sphere.getCenter(); };
	inline sReal getRadius() { return sphere.getRadius(); };

private:
	Ogre::Vector3 rpos;
	Ogre::Sphere sphere;
};

// A class representing an oriented bounding box
class sBox : public CollisionShape
{
public:
	sBox(Ogre::Vector3 p, Ogre::Vector3 x, Ogre::Vector3 y, Ogre::Vector3 z);

	virtual bool intersects(CollisionShape* other);

	bProjection project(const Ogre::Vector3&);

	void rootTranslate(const Ogre::Vector3& trans);
	void translate(const Ogre::Vector3& trans);
	void rotate(const Ogre::Quaternion& quat);
	void refreshPoints();

	// Setters
	void setPosition(Ogre::Vector3 _pos);
	void setRootPosition(const Ogre::Vector3& _rpos);
	void setOrientation(Ogre::Quaternion _orient);
	
	// Getters
	inline const std::vector<Ogre::Vector3>& getCorners() { return corners; }
	inline const Ogre::Vector3& getPosition()  { return pos; }
	inline const Ogre::Vector3& getRootPosition() { return rpos; }
	inline Ogre::Vector3 getUpAxis()    { return upVec.normalisedCopy(); }
	inline Ogre::Vector3 getOuterAxis() { return otVec.normalisedCopy(); }
	inline Ogre::Vector3 getRightAxis() { return rtVec.normalisedCopy(); }

private:
	std::vector<Ogre::Vector3> corners;
	
	Ogre::Vector3 pos;
	Ogre::Vector3 rpos;
	Ogre::Quaternion orient;

	Ogre::Vector3 upVec; Ogre::Vector3 origUpVec;
	Ogre::Vector3 otVec; Ogre::Vector3 origOtVec;
	Ogre::Vector3 rtVec; Ogre::Vector3 origRtVec;
};


// Class holding a list of CollisionShape(s) for use in a GameEntity
class CollisionBounds
{
public:
	CollisionBounds();
	CollisionBounds(std::vector<CollisionShape*> newShapes);

	inline bool intersects(CollisionBounds& other)
	{
		std::vector<CollisionShape*> oshapes = other.getShapes();

		// Check all combinations of shapes for intersection
		for (auto it = shapes.begin(); it != shapes.end(); ++it)
		{
			for (auto itt = oshapes.begin(); itt != oshapes.end(); ++itt)
			{
				// If any are intersecting, return true
				if ((*it)->intersects((*itt)))
					return true;
			}
		}

		return false;
	}

	inline std::vector<CollisionShape*>& getShapes()
	{
		return shapes;
	}

	inline void setShapes(std::vector<CollisionShape*> newShapes)
	{
		shapes = newShapes;
	}

	inline void addShape(CollisionShape* shape)
	{
		shapes.push_back(shape);
	}

	Ogre::Vector3 groupPos;
	Ogre::Quaternion groupOrient;

private:
	std::vector<CollisionShape*> shapes;
};




/*       SHAPE INTERSECTIONS       */

bool sphereToSphere(sSphere* S1, sSphere* S2);
bool sphereToBox(sSphere* S, sBox* B);
bool boxToBox(sBox* B1, sBox* B2);