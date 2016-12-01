#include "Collision.h"


/*       SHAPE INTERSECTIONS       */

// True if the squared distance is less than the squared radius
bool sphereToSphere(sSphere* S1, sSphere* S2)
{
	sReal sd = S1->getCenter().squaredDistance(S2->getCenter());
	sReal sr = Ogre::Math::Sqr(S1->getRadius() + S2->getRadius());

	return sd < sr;
}

// TODO
bool sphereToBox(sSphere* S, sBox* B)
{
	// Find closest vertex to sphere's center
	Ogre::Vector3 sCenter = S->getCenter();
	sReal dist = sReal(99999.0);
	for (char i = 0; i < 8; ++i)
	{

	}

	return false;
}


// Given by using SAT on the 15 axes
bool boxToBox(sBox* B1, sBox* B2)
{
	std::vector<Ogre::Vector3> axes;

	// Add the face normals to the list
	axes.push_back(B1->getUpAxis());
	axes.push_back(B1->getOuterAxis());
	axes.push_back(B1->getRightAxis());
	axes.push_back(B2->getUpAxis());
	axes.push_back(B2->getOuterAxis());
	axes.push_back(B2->getRightAxis());

	// Add the edge cross pairings
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j < 6; ++j)
		{
			Ogre::Vector3 a = axes[i].crossProduct(axes[j]);
			a.normalise();
			axes.push_back(a);
		}
	}

	// Now that we have the axes we need to check their SAs
	for (int i = 0; i < axes.size(); ++i)
	{
		bProjection bp1 = B1->project(axes[i]);
		bProjection bp2 = B2->project(axes[i]);

		// If the projections do not overlap, the shapes do not intersect
		if (bp1.min > bp2.max || bp1.max < bp2.min)
		{
			return false;
		}
	}

	// If all projections overlap, we have an intersection
	return true;
}





// Sphere Class implementations

sSphere::sSphere(Ogre::Sphere _sphere)
	: sphere(_sphere)
{
	this->type = ShapeType::SPHERE;
}

bool sSphere::intersects(CollisionShape* other)
{
	if (other->type == ShapeType::OBB)
	{
		sBox *bOther = dynamic_cast<sBox *>(other);
		if (NULL != bOther)
		{
			return sphereToBox(this, bOther);
		}
	}
	else if (other->type == ShapeType::SPHERE)
	{
		sSphere *sOther = dynamic_cast<sSphere *>(other);
		if (NULL != sOther)
		{
			return sphereToSphere(this, sOther);
		}
	}

	return false;
}




// OBB Class implementations

sBox::sBox(Ogre::Vector3 p, Ogre::Vector3 x, Ogre::Vector3 y, Ogre::Vector3 z)
	: pos(p),
	rtVec(x),
	upVec(y),
	otVec(z),
	origRtVec(x),
	origUpVec(y),
	origOtVec(z)
{
	this->type = ShapeType::OBB;

	corners = std::vector<Ogre::Vector3>();
	for (int i = 0; i < 8; ++i)
		corners.push_back(Ogre::Vector3(0.0, 0.0, 0.0));
	refreshPoints();
}

// Axis must be a unit vector
bProjection sBox::project(const Ogre::Vector3& axis)
{
	bProjection bp;
	bp.min = axis.dotProduct(corners[0]);
	bp.max = bp.min;

	for (int i = 1; i < 8; ++i)
	{
		sReal dotProd = axis.dotProduct(corners[i]);
		if (dotProd < bp.min)
		{
			bp.min = dotProd;
		}
		if (dotProd > bp.max)
		{
			bp.max = dotProd;
		}
	}

	return bp;
}

void sBox::refreshPoints()
{
	Ogre::Vector3 qpos = pos;
	qpos = orient * qpos;

	corners[0] = qpos + rpos;
	corners[1] = qpos + rpos + upVec;
	corners[2] = qpos + rpos + rtVec;
	corners[3] = qpos + rpos + otVec;
	corners[4] = qpos + rpos + upVec + otVec;
	corners[5] = qpos + rpos + upVec + rtVec;
	corners[6] = qpos + rpos + otVec + rtVec;
	corners[7] = qpos + rpos + upVec + otVec + rtVec;
}

bool sBox::intersects(CollisionShape* other)
{
	if (other->type == ShapeType::OBB)
	{
		sBox *bOther = dynamic_cast<sBox *>(other);
		if (NULL != bOther)
		{
			return boxToBox(this, bOther);
		}
	}
	else if (other->type == ShapeType::SPHERE)
	{
		sSphere *sOther = dynamic_cast<sSphere *>(other);
		if (NULL != sOther)
		{
			return sphereToBox(sOther, this);
		}
	}

	return false;
}

void sBox::translate(const Ogre::Vector3& trans)
{
	pos += trans;
	refreshPoints();
}

void sBox::rootTranslate(const Ogre::Vector3& trans)
{
	rpos += trans;
	refreshPoints();
}

void sBox::rotate(const Ogre::Quaternion& quat)
{
	// Set the new quaternion
	orient = quat * orient;

	// Rotate the original vectors
	upVec = orient * origUpVec;
	otVec = orient * origOtVec;
	rtVec = orient * origRtVec;

	refreshPoints();
}

void sBox::setPosition(Ogre::Vector3 _pos)
{
	pos = _pos;
	refreshPoints();
}

void sBox::setRootPosition(const Ogre::Vector3& _rpos)
{
	rpos = _rpos;
	refreshPoints();
}

void sBox::setOrientation(Ogre::Quaternion _orient)
{
	// Change the quaternion
	orient = _orient;

	// Rotate the original vectors
	upVec = orient * origUpVec;
	otVec = orient * origOtVec;
	rtVec = orient * origRtVec;

	refreshPoints();
}




// Collision Bounds

CollisionBounds::CollisionBounds()
{

}

CollisionBounds::CollisionBounds(std::vector<CollisionShape*> newShapes)
	: shapes(newShapes)
{

}