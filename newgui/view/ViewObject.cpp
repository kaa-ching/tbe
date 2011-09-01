/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#include "ViewObject.h"
#include "AbstractObject.h"

#include "tbe_global.h"

ViewObject::ViewObject(AbstractObject* anAbstractObjectPtr) :
	QGraphicsPixmapItem(NULL), theAbstractObjectPtr(anAbstractObjectPtr)
{
	// nothing to do yet :-)
	Q_ASSERT(anAbstractObjectPtr!=NULL);
}

ViewObject::ViewObject(AbstractObject* anAbstractObjectPtr, const QString& anImageName) :
	QGraphicsPixmapItem(NULL), theAbstractObjectPtr(anAbstractObjectPtr)
{
	DEBUG5ENTRY;
	Q_ASSERT(anAbstractObjectPtr!=NULL);
	Q_ASSERT(anImageName.isEmpty() == false);

	// FIXME/TODO: Introduce multiple images handling
	// FIXME/TODO: Introduce SVG rendering - or maybe still use QGraphicsSvgItem for that?
	QPixmap* myPixmapPtr = new QPixmap(anImageName);
	Q_ASSERT(myPixmapPtr!=NULL);
	setPixmap(*myPixmapPtr);

	// set transformations: origin, scale, rotate
	setTransformOriginPoint(boundingRect().width()/2.0,boundingRect().height()/2.0);
	QTransform theTransform;
	theTransform.scale(theAbstractObjectPtr->getTheWidth()/boundingRect().width(),
					   theAbstractObjectPtr->getTheHeight()/boundingRect().height());
	theTransform.translate(-boundingRect().width()/2.0,-boundingRect().height()/2.0);
	setTransform(theTransform,false);

	// and get us to the final starting position...
	setPos(theAbstractObjectPtr->getOrigCenter().toQPointF());
	setRotation(theAbstractObjectPtr->getOrigCenter().angleInDegrees());
}

ViewObject::~ViewObject()
{
	// nothing to do yet :-)
}

