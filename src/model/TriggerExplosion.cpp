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

#include "TriggerExplosion.h"
#include "tbe_global.h"

//// the DetonatorBox class' ObjectFactory
class DetonatorBoxObjectFactory : public ObjectFactory
{
public:
	DetonatorBoxObjectFactory(void)
	{	announceObjectType("DetonatorBox", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new DetonatorBox()); }
};
static DetonatorBoxObjectFactory theDetonatorBoxObjectFactory;

// FIXME: part of temporary trigger mechanism
static bool	theGlobalTrigger = false;


const qreal  DetonatorBox::ACTIVATED_TIME = 0.5;
const qreal  DetonatorBox::RINGING_TIME   = 0.5;
#define STARTDISTANCE 0.2f
const Vector DetonatorBox::HANDLEOFFSET   = Vector(0,STARTDISTANCE);

DetonatorBox::DetonatorBox()
		:	RectObject( QObject::tr("Detonator Box"),
				"Push the handle down to BOOM",
				"DetonatorBoxDone;DetonatorBoxActivated;DetonatorBoxRinging;DetonatorBoxDone",
				0.33, 0.35, 4.0, 0.0), theState(ARMED), theHandleObjectPtr(NULL)
{
}

DetonatorBox::~DetonatorBox()
{
	delete theHandleObjectPtr;
	theHandleObjectPtr = NULL;
}

void DetonatorBox::callbackStep (qreal /*aTimeStep*/, qreal aTotalTime)
{
	switch(theState)
	{
	case ARMED:
		if (isTriggered)
		{
			goToState(ACTIVATED);
			theActivationStartTime=aTotalTime;
		}
		break;
	case ACTIVATED:
		if (aTotalTime > theActivationStartTime + ACTIVATED_TIME)
		{
			goToState(RINGING);
		}
		break;
	case RINGING:
		if (aTotalTime > theActivationStartTime + ACTIVATED_TIME + RINGING_TIME)
		{
			notifyExplosions();
			goToState(DONE);
		}
		break;
	case DONE:
		break;
	}
}

void DetonatorBox::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();
	if (theHandleObjectPtr)
		theHandleObjectPtr->createPhysicsObject();
}

void DetonatorBox::deletePhysicsObject(void)
{
	if(theHandleObjectPtr)
		theHandleObjectPtr->deletePhysicsObject();
	RectObject::deletePhysicsObject();
}


DetonatorBox::States DetonatorBox::goToState(DetonatorBox::States aNewState)
{
	DEBUG4("DetonatorBox from state %d to state %d\n", theState, aNewState);
	theState = aNewState;
	return theState;
}

void DetonatorBox::notifyExplosions(void)
{
	// FIXME: part of temporary trigger mechanism
	theGlobalTrigger = true;
}

void DetonatorBox::reset(void)
{
	RectObject::reset();

	// the reset will already do a delete+create
	// of the handle objectptr, so no need to reset it specifically here...
	if (theHandleObjectPtr==NULL)
	{
		theHandleObjectPtr = new DetonatorBoxHandle(this, getOrigCenter()+HANDLEOFFSET);
		theWorldPtr->addObject(theHandleObjectPtr);
	}

	isTriggered = false;
	theActivationStartTime = 0.0f;
	theState = ARMED;
	theWorldPtr->registerCallback(this);
	// FIXME: part of temporary trigger mechanism
	theGlobalTrigger = false;
}

void DetonatorBox::setOrigCenter ( Position new_var )
{
	RectObject::setOrigCenter(new_var);

	if (theHandleObjectPtr!=NULL)
		theHandleObjectPtr->setOrigCenter(new_var+HANDLEOFFSET);
}

void DetonatorBox::setTriggered(void)
{
	// let's just be nice asynchronously
	isTriggered = true;
}


// ##########################################################################
// ##########################################################################
// ##########################################################################

DetonatorBoxHandle::DetonatorBoxHandle(DetonatorBox* aDBox, const Position& aPos)
		:	RectObject( QObject::tr("Detonator Box Handle"),
				"Push Here To BOOM",
				"DetonatorBoxHandle",
				0.25, 0.26, 0.1, 0.0), theDBoxPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(aPos);
	createPhysicsObject();
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

DetonatorBoxHandle::~DetonatorBoxHandle()
{
	theWorldPtr->removeObject(this);
}

void DetonatorBoxHandle::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
	// if down, stay down and signal the box
	if (theJointPtr->GetJointTranslation() <= theJointPtr->GetLowerLimit())
	{
		theJointPtr->SetMaxMotorForce(0);
		theDBoxPtr->setTriggered();
	}
}

DrawObject*  DetonatorBoxHandle::createDrawObject(void)
{
	RectObject::createDrawObject();
	// redo the ZValue: BaseObject will set it to 2.0 (default for DrawObjects)
	// if not in Properties, set to 1.9: the Handle draws behind the Box
	setDrawObjectZValue(1.0);
	return theDrawObjectPtr;
}

void DetonatorBoxHandle::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();

	// initialise the prismatic (translation) joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(theDBoxPtr->theB2BodyPtr, theB2BodyPtr, getOrigCenter().toB2Vec2(), Vector(0,1.0).toB2Vec2());
	myJointDef.userData = NULL;
	myJointDef.collideConnected = false;
	myJointDef.maxMotorForce = 120.0f;
	myJointDef.motorSpeed = 2.0;
	myJointDef.lowerTranslation = - STARTDISTANCE/2.0f;
	myJointDef.upperTranslation = 0.0;
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = true;

	assert(theJointPtr==NULL);
	theJointPtr = reinterpret_cast<b2PrismaticJoint*>(getB2WorldPtr()->CreateJoint(&myJointDef));
}

void DetonatorBoxHandle::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal DetonatorBoxHandle::getDistance(void)
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}

void DetonatorBoxHandle::reset(void)
{
	RectObject::reset();
	theWorldPtr->registerCallback(this);
}


// ##########################################################################
// ##########################################################################
// ##########################################################################


//// the Dynamite class' ObjectFactory
class DynamiteObjectFactory : public ObjectFactory
{
public:
	DynamiteObjectFactory(void)
	{	announceObjectType("Dynamite", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Dynamite()); }
};
static DynamiteObjectFactory theDynamiteObjectFactory;

const qreal  Dynamite::RINGING_TIME   = 0.3;
const qreal  Dynamite::DYNAMITE_MASS  = 0.8;

Dynamite::Dynamite()
		: PolyObject(QObject::tr("Dynamite"),
			 QObject::tr("invented by Alfred Nobel, this is a nice explosive."),
			 "Dynamite;DynamiteActive;DynamiteRinging;DynamiteBoom;Empty",
			 "(-0.215,-0.16)=(0.215,-0.16)=(0.215,0.02)=(-0.15,0.16)=(-0.215,0.16)",
			 0.43, 0.32, DYNAMITE_MASS, 0.1),
		  theState(WAITING), theActiveStartTime(0.0f)
{
}

Dynamite::~Dynamite()
{
}

void Dynamite::callbackStep (qreal /*aTimeStep*/, qreal aTotalTime)
{
	switch(theState)
	{
	case WAITING:
		if (theGlobalTrigger)
		{
			goToState(ACTIVE);
			theActiveStartTime=aTotalTime;
		}
		break;
	case ACTIVE:
		if (aTotalTime > theActiveStartTime + RINGING_TIME)
		{
			goToState(RINGING);
		}
		break;
	case RINGING:
		if (aTotalTime > theActiveStartTime + 2*RINGING_TIME)
		{
			theBoomStartTime = aTotalTime;
			goToState(BOOM);
		}
		break;
	case BOOM:
		manageParticles(aTotalTime-theBoomStartTime);
		if (aTotalTime > theActiveStartTime + 3*RINGING_TIME)
		{
			goToState(GONE);
		}
		break;
	case GONE:
		manageParticles(aTotalTime-theBoomStartTime);
		break;
	}
}

void Dynamite::explode(void)
{
	const int NUM_SPLATS = 12;
	for (int i=0; i< NUM_SPLATS; i++)
	{
		ExplosionSplatter* mySplatter = new ExplosionSplatter();

		// startpos = dynamite center + angle + distance from center
		Position myStart = getTempCenter();
		myStart.angle += 2*PI*(float)i/NUM_SPLATS;
		myStart = myStart + Vector(1.3*ExplosionSplatter::theRadius,0);

		// HACK HACK: A regular detonation front is close to sonic speed (1000 m/s)
		// we're not going to go anywhere near that speed here...
		mySplatter->setAll(theWorldPtr, myStart, 20.0, DYNAMITE_MASS/NUM_SPLATS, this);
		theSplatterList.push_back(mySplatter);
	}
}

Dynamite::States Dynamite::goToState(Dynamite::States aNewState)
{
	DEBUG4("Dynamite from state %d to state %d\n", theState, aNewState);

	if (theState == RINGING && aNewState == BOOM)
	{
		deletePhysicsObject();
		explode();
	}

	theState = aNewState;
	return theState;
}

void Dynamite::manageParticles(float aDeltaTime)
{
	int mySplattersLeft = theSplatterList.count();
	if (mySplattersLeft > 0)
	{
		// the following was tuned for full speed with aTimeStep equals 25 frames i.e. 0.040 s.
		//
		// The perimeter of the detonation front is linearly increasing,
		// (perimeter of a circle is 2*pi*radius, remember?)
		// so the power of the explosion decays linearly.
		//
		// let's assume the decay is complete after 2 seconds, i.e. the mass of
		// the total number of splatters left at that point is reduced to zero
		const float DECAY_TIME = 1.5;

		float myMassLeft = DYNAMITE_MASS*(DECAY_TIME-aDeltaTime)/DECAY_TIME;
		if (myMassLeft < 0.0)
		{
			// TODO/FIXME: delete all objects correctly
			theSplatterList.clear();
		}
		else
			foreach(ExplosionSplatter* e, theSplatterList)
				e->setMass( myMassLeft / mySplattersLeft );
	}
}

void Dynamite::removeMe(ExplosionSplatter* aDeadSplatterPtr)
{
	assert(aDeadSplatterPtr != NULL);
	theSplatterList.removeAll(aDeadSplatterPtr);
}


void Dynamite::reset(void)
{
	deletePhysicsObject();
	createPhysicsObject();
//	PolyObject::reset();

	theWorldPtr->registerCallback(this);
	theActiveStartTime = 0.0f;
	theState = WAITING;

	theSplatterList.clear();
}


// ##########################################################################
// ##########################################################################
// ##########################################################################

const qreal ExplosionSplatter::theRadius = 0.04;
const int   ExplosionSplatter::COLLISION_GROUP_INDEX = 3;

// note that the mass will be redone during setAll()
ExplosionSplatter::ExplosionSplatter()
		: AbstractBall("ExplosionSplatter","", "CokeSplatter",
					   theRadius, 0.001,  1.0)
{
	DEBUG5("ExplosionSplatter::ExplosionSplatter\n");

	// the first shape is already set in the AbstractBall constructor
	// we only need to set the groupIndex so the various splatters do not collide
	theShapeList[0]->filter.groupIndex = - COLLISION_GROUP_INDEX;

	// the sensor - slightly larger
	// (sensor is used for collision detection & getting rid of the object if needed)
	b2CircleDef* mySensorDef = new b2CircleDef();
	mySensorDef->radius = 1.01 * theRadius;
	mySensorDef->isSensor = true;
	mySensorDef->userData = this;
	theShapeList.push_back(mySensorDef);

}

ExplosionSplatter::~ExplosionSplatter()
{
	DEBUG5("ExplosionSplatter::~ExplosionSplatter()\n");
}


void ExplosionSplatter::callBackSensor(b2ContactPoint*)
{
	// oww we hit something.
	// that may be the end for us
	//   - depending on whether we are reversing on our path or not

	// FIXME/TODO: technically speaking, we should check if we hit
	// a static/kinetic or a dynamic object.
	//   - In case of static/kinetic: 100% bounce
	//   - In case of dynamic: stick to it so we can propel it properly
	//
	// but that's significantly more complex than this...
	//
	// IDEA: can we vary bounciness during collision detection for
	// different types of objects?

	// compare against theStartVelocityVector
	b2Vec2 myVelocity = theB2BodyPtr->GetLinearVelocity();

	if (((myVelocity.x>=0) != (theStartVelocityVector.dx>=0)) &&
		((myVelocity.y>=0) != (theStartVelocityVector.dy>=0)))
	{
		theDynamitePtr->removeMe(this);
		theWorldPtr->removeMe(this, 0.05);
	}
}


void ExplosionSplatter::setAll(World* aWorldPtr,
						  const Position& aStartPos,
						  qreal aVelocity,
						  qreal aSplatterMass,
						  Dynamite* aDynamitePtr)
{
	DEBUG5("ExplosionSplatter::setAll(%p, (%f,%f,%f), %f, %f)\n",
		   aWorldPtr, aStartPos.x, aStartPos.y, aStartPos.angle,
		   aVelocity, aSplatterMass);

	setOrigCenter(aStartPos);
	aWorldPtr->addObject(this);
	createPhysicsObject();

	qreal myAngle = aStartPos.angle;
	theStartVelocityVector = Vector(aVelocity * cos(myAngle), aVelocity * sin(myAngle));
	theB2BodyPtr->SetLinearVelocity(theStartVelocityVector.toB2Vec2());

	setMass(aSplatterMass);
	theDynamitePtr = aDynamitePtr;
}

void ExplosionSplatter::setMass( qreal aSplatterMass )
{
	b2MassData myMD;
	myMD.mass = aSplatterMass;
	myMD.center = b2Vec2(0,0);
	myMD.I = 0.0008 * aSplatterMass;
	theB2BodyPtr->SetMass(&myMD);
}