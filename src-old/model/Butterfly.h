/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include "RectObject.h"
#include "World.h"


/** this class implements the Butterfly:
  * a Butterfly always flies to a flower - if it sees one
  */
class Butterfly : public RectObject, public SimStepCallbackInterface
{
public:
	Butterfly();

	virtual ~Butterfly();

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getName ( ) const
	{	return QObject::tr("Butterfly"); }

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Butterfly (Flappus Chaoticus Fragilius) - always in search of flowers."); }

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject because this class wants to register for callbacks
	virtual void createPhysicsObject(void);

	enum ButterflyStatus
	{
		STILL,				// not implemented yet
		FLAP_OPEN,
		FLAP_HALF,
		DEAD				// not implemented yet
	};

	/// @returns the state of the butterfly state machine
	ButterflyStatus getState(void)
	{	return theButterflyState; }

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return getState(); }

	/** sets up the Butterfly to fly to a Flower
	  * or flap idly if there is no flower
	  */
	void goToFlower(void);


public:
	// the following two members are part of the normal impulse reporting

	/// overridden from BaseObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from BaseObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength);

protected:
	/// suggest a new state of the butterfly state machine
	void setState(ButterflyStatus aNewStateSuggestion);

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

private:
	// Private things

	// Things from RectObject that need adjustments:

private:
	/// the weight of the butterfly (100 grams)
	static const double theButterflyMass;

	/// central variable of the Butterfly state machine
	ButterflyStatus theButterflyState;

	/// FIXME: TODO
	signed int theCountdown;

	/// if moving to a flower, this is the target position for the butterfly
	Position theTargetPos;
};

#endif // BUTTERFLY_H