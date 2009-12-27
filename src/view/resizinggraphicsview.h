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

// This headerfile is a bit different from other headerfiles
// because it uses only lowercase - that's to work around a
// Qt limitation.


#ifndef RESIZINGGRAPHICSVIEW_H_
#define RESIZINGGRAPHICSVIEW_H_

#include "tbe_global.h"
#include "toolbox.h"
#include "DrawWorld.h"

#include <QGraphicsView>
#include <QTimer>
#include <QDragEnterEvent>

/** a customized QGraphicsView that does not suffer from fitting problems
 */
class ResizingGraphicsView : public QGraphicsView
{
	Q_OBJECT
	
public:
	/** the constructor
	 *  it sets a lot of custom settings that we need for our GraphicsView
	 */
	ResizingGraphicsView (QWidget* aParent)
		: QGraphicsView(aParent)
	{
		setAlignment(Qt::AlignCenter);

		setInteractive(true);
		setDragMode(QGraphicsView::RubberBandDrag);
		setRubberBandSelectionMode(Qt::ContainsItemShape);

		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	    QSizePolicy vpol;
	    vpol.setHorizontalPolicy(QSizePolicy::Expanding);
	    vpol.setVerticalPolicy(QSizePolicy::Expanding);
		setSizePolicy(vpol);
	}

	virtual ~ResizingGraphicsView() {;}

	/** this override works around a quirk in QGraphicsView,
	 *  where fitInView() works - but only after adding the scene is done and rendered
	 *  so we set a singleshot timer to redo the fitInView later...
	 */
	void setScene ( QGraphicsScene * scene )
	{
		QGraphicsView::setScene(scene); 		
		QTimer::singleShot(200, this, SLOT(on_timerTick()));
	}
	
protected:
	virtual void resizeEvent(QResizeEvent * event)
	{
		QGraphicsView::resizeEvent(event);
		on_timerTick();
	}

	/// event handler override from QGraphicsView to accept drops
	/// and forward them to our graphics scene - which knows more...
	virtual void dropEvent (QDropEvent* event)
	{
		DEBUG4("void ResizingGraphicsView::dropEvent(\"%s\")\n", ASCII(event->mimeData()->formats().join(";")));
		QPointF myPos = mapToScene(event->pos());
		if (scene()!=NULL)
			reinterpret_cast<DrawWorld*>(scene())->dropEventFromView(myPos, event);
	}

	void dragEnterEvent(QDragEnterEvent *event)
	{
		if (event->mimeData()->hasFormat(TBItem::ToolboxMimeType) && scene()!=NULL)
		{
			DEBUG4("void ResizingGraphicsView::dragEnterEvent(\"%s\") - accept\n", ASCII(event->mimeData()->formats().join(";")));
			event->accept();
		}
		else
		{
			DEBUG3("void ResizingGraphicsView::dragEnterEvent(\"%s\") - denied\n", ASCII(event->mimeData()->formats().join(";")));
			event->ignore();
		}
	}

	/// apparently required to override to get D&D to work - even if empty
	virtual void dragLeaveEvent(QDragLeaveEvent*)
	{
	}

	/// apparently required to override to get D&D to work - even if empty
	virtual void dragMoveEvent(QDragMoveEvent*)
	{
	}

protected slots:
	void on_timerTick(void)
		{ if (scene()) 
			QGraphicsView::fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio); }
};

#endif /* RESIZINGGRAPHICSVIEW_H_ */
