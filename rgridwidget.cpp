#include "rgridwidget.h"
#include <QPainter>
#include "rcompositemapobject.h"
#include "rmapobjectfocusholder.h"

//--------------------------------------------------------------------------------------
RGridWidget::RGridWidget(QWidget *parent, int size) :
	QFrame(parent),
	gridSize(size)
{
	setFrameShape(Panel);
	setFrameShadow(Raised);
}

//--------------------------------------------------------------------------------------
RGridWidget::~RGridWidget()
{
    RCompositeMapObject::deleteInstance();
    RMapObjectFocusHolder::instance().setFocusMapObject(0);
}

//--------------------------------------------------------------------------------------
void RGridWidget::paintEvent(QPaintEvent *event)
{	
	if (!polygon.empty()) {
		QPainter painter(this);
		QPen pen(Qt::SolidLine);
		pen.setColor(QColor(Qt::black));
		painter.setPen(pen);
		painter.drawPoints(polygon);
	}

	QFrame::paintEvent(event);
}

//--------------------------------------------------------------------------------------
void RGridWidget::resizeEvent(QResizeEvent *event)
{
	updatePolygonData();
	QFrame::resizeEvent(event);
}

//--------------------------------------------------------------------------------------
void RGridWidget::updatePolygonData()
{
	if (gridSize > 1) {
		polygon.clear();

		int left = rect().left() - (rect().left() % gridSize);
		int top = rect().top() - (rect().top() % gridSize);

		for (int x = left; x < rect().right(); x += gridSize)
			for (int y = top; y < rect().bottom(); y += gridSize)
				polygon.append(QPoint(x, y));
	}
}
