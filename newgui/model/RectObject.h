/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#ifndef RECTOBJECT_H
#define RECTOBJECT_H

#include "AbstractObject.h"

/// forward declarations
class b2FixtureDef;

/**
  * class RectObject
  *
  * This class defines a rectangular object. It can have any size - it
  * works around the limitation of Box2D that the width/height should not be more than 10 or less than 1/10th.
  * It can have a mass, but it doesn't need to: If it does not have a mass, it won't move in the simulation.
  * Many things will use this class as a base - it should replace Floor and Wall for example.
  *
  *
  * Parameters that can be set through Properties are defined in Property.h
  */

class RectObject : public AbstractObject
{
public:

	// Constructors/Destructors
	//

	/**
	 * Empty Constructor
	 */
	RectObject ( );

	/// elaborate constructor, only used by the ConcreteRectObject thingies
	RectObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness);

	/**
	 * Empty Destructor
	 */
	virtual ~RectObject ( );

	// Public attribute accessor methods
	//

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return theNameString;
	}

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if the property mass was set
	virtual b2BodyType getObjectType(void) const;

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return theToolTipString;
	}

	/// parse all properties
	/// partially overridden from AbstractObject
	virtual void  parseProperties(void);

	/// @returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return resizableInfo;	}


	/**
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( qreal new_var );

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( qreal new_var );

	/// overridden from AbstractObject: allow running adjustParameters() first
	virtual ViewObject* createViewObject(float aDefaultDepth = 2.0);

protected:
	/// this member fixes up the physical model based on new width or height
	/// do not override unless Klaas okays it
	virtual void adjustParameters(void);

	virtual void initAttributes ( );

	static const qreal ASPECT_RATIO;

	/// TODO/FIXME: promote this member to AbstractObject???
	/// TODO/FIXME: add documentation for this member!!!
	void setFriction(b2FixtureDef* aFixtureDef);

private:
	/// adjustParameters for tall objects (where height/width > ASPECT_RATIO)
	/// only expected to be called from adjustParameters()
	void adjustTallParametersPart();
	/// adjustParameters for wide objects (where width/height > ASPECT_RATIO)
	/// only expected to be called from adjustParameters()
	void adjustWideParametersPart();

protected:
	QString theNameString;
	QString theToolTipString;

	SizeDirections resizableInfo;
};

#endif // RECTOBJECT_H