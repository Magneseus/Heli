#pragma once

#include <Ogre.h>
#include "DebugDrawer.h"

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

	virtual void rootTranslate(const Ogre::Vector3& trans);
	virtual void setRootPosition(Ogre::Vector3 _rpos);
	virtual Ogre::Vector3 getRootPosition();
	virtual void rotate(const Ogre::Quaternion& rot);
	virtual void setOrientation(Ogre::Quaternion _orient);
	virtual Ogre::Quaternion getOrientation();

	virtual void DebugDraw(Ogre::ColourValue col = Ogre::ColourValue::Red) = 0;

	ShapeType type;

protected:
	Ogre::Vector3 rpos;
	Ogre::Quaternion orient;
};

// A class representing a sphere
class sSphere : public CollisionShape
{
public:
	sSphere(Ogre::Sphere);

	// Virtual functions from CollisionShape
	virtual bool intersects(CollisionShape* other) override;
	virtual void DebugDraw(Ogre::ColourValue col = Ogre::ColourValue::Red);

	virtual Ogre::Vector3 getRootPosition() override;

	// Other movement functions
	inline void translate(const Ogre::Vector3& trans)
	{
		sphere.setCenter(sphere.getCenter() + trans);
	}
	inline void setCenter(Ogre::Vector3 _center) { sphere.setCenter(_center); }
	
	// Getters
	inline const Ogre::Vector3& getCenter() { return sphere.getCenter(); };
	inline sReal getRadius() { return sphere.getRadius(); };

private:
	Ogre::Sphere sphere;
};

// A class representing an oriented bounding box
class sBox : public CollisionShape
{
public:
	sBox(Ogre::Vector3 p, Ogre::Vector3 x, Ogre::Vector3 y, Ogre::Vector3 z);

	// Virtual functions
	virtual bool intersects(CollisionShape* other) override;
	virtual void rootTranslate(const Ogre::Vector3& trans) override;
	virtual void setRootPosition(Ogre::Vector3 _rpos) override;
	virtual void setOrientation(Ogre::Quaternion _orient) override;
	virtual void rotate(const Ogre::Quaternion& quat) override;

	virtual void DebugDraw(Ogre::ColourValue col = Ogre::ColourValue::Red);

	// Other functions
	bProjection project(const Ogre::Vector3&);
	void refreshPoints();

	// Other movement functions
	void translate(const Ogre::Vector3& trans);
	void setPosition(Ogre::Vector3 _pos);
	
	// Getters
	inline const std::vector<Ogre::Vector3>& getCorners() { return corners; }
	inline const Ogre::Vector3& getPosition() { return pos; }
	inline Ogre::Vector3 getUpAxis()    { return upVec.normalisedCopy(); }
	inline Ogre::Vector3 getOuterAxis() { return otVec.normalisedCopy(); }
	inline Ogre::Vector3 getRightAxis() { return rtVec.normalisedCopy(); }

private:
	std::vector<Ogre::Vector3> corners;
	
	Ogre::Vector3 pos;

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

	bool intersects(CollisionBounds& other);
	void DebugDraw(Ogre::ColourValue col = Ogre::ColourValue::Red);

	// Movement functions
	void setPosition(Ogre::Vector3 _pos);
	void setOrientation(Ogre::Quaternion _orient);
	
	inline const Ogre::Vector3& getPosition() { return groupPos; }
	inline const Ogre::Quaternion& getOrientation() { return groupOrient; }

	// Inline vector functions
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

private:
	std::vector<CollisionShape*> shapes;

	Ogre::Vector3 groupPos;
	Ogre::Quaternion groupOrient;
};




/*       SHAPE INTERSECTIONS       */

bool sphereToSphere(sSphere* S1, sSphere* S2);
bool sphereToBox(sSphere* S, sBox* B);
bool boxToBox(sBox* B1, sBox* B2);