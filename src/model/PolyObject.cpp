/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "PolyObject.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawPolyObject.h"
#include "Property.h"

static const char* DEFAULT_POLYOBJECT_NAME = "PolyObject";


//// this class' ObjectFactory
class PolyObjectFactory : public ObjectFactory
{
public:
	PolyObjectFactory(void)
	{	announceObjectType(DEFAULT_POLYOBJECT_NAME, this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new PolyObject()); }
};
static PolyObjectFactory theRectObjectFactory;


/** the AbstractPolyObjectFactory
 *  Note that it is slightly more complex than usual, because it is generalised
 *  to create any type of PolyObject. Below the declaration, there will be several
 *  global instances each identifying one rectobject type
 */
class AbstractPolyObjectFactory : public ObjectFactory
{
public:
	AbstractPolyObjectFactory(
		const QString& anInternalName,
		const QString& aDisplayName,
		const QString& aTooltip,
		const QString& anImageName,
		const QString& anOutline,
		qreal aWidth,
		qreal aHeight,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theOutline(anOutline),
			  theWidth(aWidth), theHeight(aHeight),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual BaseObject* createObject(void) const
	{	return fixObject(new PolyObject(theDisplayName, theTooltip,
										theImageName, theOutline,
										theWidth, theHeight, theMass,
										theBounciness)); }
private:
		QString theDisplayName;
		QString theTooltip;
		QString theImageName;
		QString theOutline;
		qreal theWidth;
		qreal theHeight;
		qreal theMass;
		qreal theBounciness;
};

static AbstractPolyObjectFactory theBowlingPinFactory(
	"BowlingPin",
	QObject::tr("Bowling Pin"),
	QObject::tr("Bowling pins are meant to be run "
				"over - and most people prefer to do that using "
				"a Bowling Ball."),
	"BowlingPin",
	"(0.02,0.17)=(-0.02,0.17)=(-0.045,0.14)=(-0.04,0.065)=(0.0,0.04)"
	"=(0.04,0.065)=(0.045,0.14);"
	"(-0.04,0.02)=(-0.06,-0.04)=(-0.06,-0.11)=(-0.035,-0.17)"
	"=(0.035,-0.17)=(0.06,-0.11)=(0.06,-0.04)=(0.04,0.02)",
	0.12, 0.34, 1.5, 0.4 );

static AbstractPolyObjectFactory theSkyhookFactory(
	"Skyhook",
	QObject::tr("Skyhook"),
	QObject::tr("A skyhook just hangs in the air. And you can hang a "
				"lot of weight on it!"),
	"Skyhook",
	"(-0.03,-0.07)=(0.01,-0.11)=(0.05,-0.11)=(0.1,-0.05)=(0.1,-0.02)"
	"=(0.08,0.00)=(-0.02,-0.03);(-0.10,0.12)",
	0.20, 0.23, 0.0, 0.4 );

static AbstractPolyObjectFactory theWeightFactory(
	"Weight",
	QObject::tr("Weight"),
	QObject::tr("A serious mass. As heavy as it looks!"),
	"Weight",
	"(-0.20,-0.20)=(0.20,-0.20)=(0.06,0.20)=(-0.06,0.20)",
	0.40, 0.40, 10.0, 0.3 );

static AbstractPolyObjectFactory theLeftRampFactory(
	"LeftRamp",
	QObject::tr("Left Ramp"),
	QObject::tr("This is a ramp. The left is lower than the right, so things slide to the left."),
	"LeftRamp",
	"(-0.5,-0.46)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
	1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theRightRampFactory(
	"RightRamp",
	QObject::tr("Right Ramp"),
	QObject::tr("This is a ramp. The left is higher than the right, so things slide to the right."),
	"RightRamp",
	"(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
	1.0, 1.0, 0.0, 0.2 );

// the quarter arc is based on five similarly-sized segments, it is more-or-less smooth
// on both the inside and the outside - you can use both if you want :-)
static AbstractPolyObjectFactory theQuarterArcFactory(
	"QuarterArc",
	QObject::tr("Quarter Arc"),
	QObject::tr("This is a quarter arc. Or ninety degrees, or 1.57 radians if you want."),
	"QuarterArc",
	"(0.100,-.200)=(0.200,-.200)=(0.180,-.076)=(0.085,-.107);"
	"(0.085,-.107)=(0.180,-.076)=(0.124,0.035)=(0.043,-.024);"
	"(0.043,-.024)=(0.124,0.035)=(0.035,0.124)=(-.024,0.043);"
	"(-.024,0.043)=(0.035,0.124)=(-.076,0.180)=(-.107,0.085);"
	"(-.107,0.085)=(-.076,0.180)=(-.200,0.200)=(-.200,0.100)",
	0.4, 0.4, 0.0, 0.1 );

// Constructors/Destructors
//

PolyObject::PolyObject ( ) 
{
	DEBUG5("PolyObject::PolyObject()\n");
	PolyObject(DEFAULT_POLYOBJECT_NAME, "", DEFAULT_POLYOBJECT_NAME,
			   "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
				1.0, 1.0, 0, 0.3);
}

PolyObject::PolyObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				const QString& anOutline,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness )
	: theNameString(aDisplayName), theToolTipString(aTooltip)
{
	DEBUG5("PolyObject::PolyObject(%s, %f, %f)\n", ASCII(aDisplayName), aWidth, aHeight);
	theProps.setDefaultPropertiesString(
		Property::FRICTION_STRING + QString(":/") +
		Property::IMAGE_NAME_STRING + QString(":") + aImageName + QString("/") +
		Property::MASS_STRING + ":" + QString::number(aMass) + QString("/") +
		Property::ROTATABLE_STRING + QString(":false/") +
		Property::POLYGONS_STRING + QString(":") + anOutline + QString("/")
		);

	BaseObject::setTheWidth(aWidth);
	BaseObject::setTheHeight(aHeight);
	setTheBounciness(aBounciness);
	rotatableInfo = false;	// this is the default, will be fixed in parseProperties

}

PolyObject::~PolyObject ( )
{
	DEBUG5("PolyObject::~PolyObject()\n");
}

//
// Methods
//


// Accessor methods
//

void PolyObject::setTheWidth ( qreal new_var )
{
	BaseObject::setTheWidth(new_var);
	clearShapeList();
	fillShapeList();
}

void PolyObject::setTheHeight ( qreal new_var )
{
	BaseObject::setTheHeight(new_var);
	clearShapeList();
	fillShapeList();
}

// Other methods
//

DrawObject*  PolyObject::createDrawObject(void)
{
	QString myImageName;
	theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName);
	if (myImageName.isEmpty())
		myImageName = getName();
	theDrawObjectPtr = new DrawPolyObject(this, myImageName);
	setDrawObjectZValue(3.0);
	return theDrawObjectPtr;
}

void PolyObject::fillShapeList(void)
{
	DEBUG5("void PolyObject::fillShapeList(void)\n");

	float myMass;
	theProps.property2Float(Property::MASS_STRING, &myMass);

	QString myPolygons;
	theProps.property2String(Property::POLYGONS_STRING, &myPolygons);
	QStringList myPolygonList = myPolygons.split(";",QString::SkipEmptyParts);

	if (theAABB.isInitialised()==false)
		theAABB = AABB(myPolygons);
	if (theAABB.isInitialised()==false)
		return;
	Vector myScale(getTheWidth()/theAABB.getOrigWidth(), getTheHeight()/theAABB.getOrigHeight());

	QStringList::iterator i = myPolygonList.begin();
	while (i!=myPolygonList.end())
	{
		b2PolygonDef* myPolyDef = new b2PolygonDef();

		QStringList myCoordList = (*i).split("=",QString::SkipEmptyParts);
		// if we find a shape with less than three vertexes, let's ignore it
		// this is very convenient if you need to adapt the AABB :-)
		myPolyDef->vertexCount = myCoordList.count();
		if (myCoordList.count()>=3)
		{
			for (int j=0; j<myCoordList.count(); j++)
			{
				assert (j < b2_maxPolygonVertices);
				Vector myCoord;
				bool isDone = myCoord.fromString(myCoordList.at(j));
				assert (isDone == true);
				UNUSED_VAR(isDone);
				Vector myScaledCoord = myScale*myCoord;
				myPolyDef->vertices[j]=myScaledCoord.toB2Vec2();
			}

			// get mass:  no mass -> no density -> no motion
			if (myMass != 0.0)
				myPolyDef->density = myMass / getTheWidth()*getTheHeight();
			myPolyDef->userData = this;
			setFriction(myPolyDef);
			theShapeList.push_back(myPolyDef);
		}
		++i;
	}
}

void PolyObject::parseProperties(void)
{

	BaseObject::parseProperties();
	theProps.property2Bool(Property::ROTATABLE_STRING, &rotatableInfo, false);

	clearShapeList();
	fillShapeList();
	createPhysicsObject();

}

void  PolyObject::setFriction(b2PolygonDef* aBoxDef)
{
	// only set friction if it is special
	if (theProps.getPropertyNoDefault(Property::FRICTION_STRING).isEmpty())
		return;

	float myFriction = 0;
	if (theProps.property2Float(Property::FRICTION_STRING, &myFriction))
		aBoxDef->friction = myFriction;
	else
		assert(false);
}




PolyObject::AABB::AABB(void) :
		theOrigWidth(UNDEFINED), theOrigHeight(UNDEFINED)
{
}

PolyObject::AABB::AABB(QString& aPolygonDefs) :
		theOrigWidth(UNDEFINED), theOrigHeight(UNDEFINED)
{
	DEBUG5("PolyObject::AABB::AABB(%s)\n", ASCII(aPolygonDefs));
	float myMinX = 0.0f;
	float myMaxX = 0.0f;
	float myMinY = 0.0f;
	float myMaxY = 0.0f;

	if (aPolygonDefs.isEmpty())
	{
		DEBUG4("ignoring empty polygon def\n");
		return;
	}

	QStringList myPolygonList = aPolygonDefs.split(";",QString::SkipEmptyParts);
	QStringList::iterator i = myPolygonList.begin();
	while (i!=myPolygonList.end())
	{
		QStringList myCoordList = (*i).split("=",QString::SkipEmptyParts);
		for (int j=0; j<myCoordList.count(); j++)
		{
			Vector myCoord;
			bool isDone = myCoord.fromString(myCoordList.at(j));
			assert (isDone == true);
			UNUSED_VAR(isDone);

			if (myMinX > myCoord.dx)
				myMinX = myCoord.dx;
			if (myMinY > myCoord.dy)
				myMinY = myCoord.dy;
			if (myMaxX < myCoord.dx)
				myMaxX = myCoord.dx;
			if (myMaxY < myCoord.dy)
				myMaxY = myCoord.dy;
		}
		++i;
	}

	assert (myMaxX != myMinX);
	assert (myMaxY != myMinY);

	theOrigWidth  = myMaxX - myMinX;
	theOrigHeight = myMaxY - myMinY;
	DEBUG5("  AABB for this PolyObject: %fx%f\n", theOrigWidth, theOrigHeight);
}

float PolyObject::AABB::getOrigWidth()
{
	assert(isInitialised());
	return theOrigWidth;
}

float PolyObject::AABB::getOrigHeight()
{
	assert(isInitialised());
	return theOrigHeight;
}

bool PolyObject::AABB::isInitialised()
{
	return theOrigHeight != UNDEFINED;
}
