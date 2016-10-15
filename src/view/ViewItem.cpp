/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObject.h"
#include "ResizeAwareQuickWidget.h"
#include "ViewItem.h"

ViewItem::ViewItem(QQuickItem *aParentPtr)
                       : QQuickItem(aParentPtr)
{
    // Nothing to do here...
}

// Maybe still TODO: create a destructor, because we need to take the AO from the world upon destruction.

void
ViewItem::adjustObjectDrawingFromAO()
{
    AbstractObjectPtr myAOPtr(theAOPtr);
    if (nullptr == myAOPtr)
        return;

    theWidthInM = myAOPtr->getTempWidth();
    theHeightInM= myAOPtr->getTempHeight();

    Position myPos = myAOPtr->getTempCenter();
    theXinM = myPos.x - 0.5 * theWidthInM;
    theYinM = myPos.y + 0.5 * theHeightInM;
    theAngleInDegrees = myPos.angleInQDegrees();

    emit sizeChanged();
    emit angleChanged();
}

void ViewItem::setParents(QQuickItem *aParentPtr, AbstractObjectPtr anAOPtr)
{
    setParentItem(aParentPtr);
    setParent(aParentPtr);
    theAOPtr = anAOPtr;
}