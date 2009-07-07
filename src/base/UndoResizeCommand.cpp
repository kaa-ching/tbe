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

#include "UndoResizeCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"

#include <cmath>

// Constructors/Destructors
//  

UndoResizeCommand::UndoResizeCommand (
		DrawObject* aDrawObjectPtr, 
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			theDrawObjectPtr(aDrawObjectPtr)
{
	assert(aDrawObjectPtr);
	assert(aBaseObjectPtr);
	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theNewCenter = theBaseObjectPtr->getTempCenter();
	theOldSize = QPointF(theBaseObjectPtr->getTheWidth(),theBaseObjectPtr->getTheHeight());
	setText("Resize " + theBaseObjectPtr->getName());
}

UndoResizeCommand::~UndoResizeCommand ( )
{
	DEBUG5("~UndoResizeCommand() for %p - %s\n", this, text().toAscii().constData());
	// there's nothing to do here...
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

bool UndoResizeCommand::isResized(void)
{
	if (theNewSize.isNull())
		return false;

	QPointF theDelta = theOldSize-theNewSize;
	if (fabs(theDelta.x()) < Position::minimalMove && fabs(theDelta.y()) < Position::minimalMove)
		return false;
	return true;
}


void UndoResizeCommand::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.x());
	theBaseObjectPtr->setTheHeight(theNewSize.y());

	theBaseObjectPtr->setTempCenter(theNewCenter);
	theBaseObjectPtr->setOrigCenter(theNewCenter);

	theBaseObjectPtr->reset();
}

void UndoResizeCommand::undo ()
{
	theBaseObjectPtr->setTheWidth(theOldSize.x());
	theBaseObjectPtr->setTheHeight(theOldSize.y());

	theBaseObjectPtr->setTempCenter(theOldCenter);
	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->reset();
	theDrawObjectPtr->focusRemove();
}

void UndoResizeCommand::setDelta(qreal anAnchorPos, QPointF aDelta)
{
	aDelta.setY(-aDelta.y());
	theNewSize = theOldSize + anAnchorPos*aDelta;
	theNewCenter = theOldCenter + 0.5*aDelta;
	redo();
}
