#include "rtable.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"
#include "rmapobjecttreewidgetitem.h"
#include "rmapregistry.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

//--------------------------------------------------------------------------------------
RTable::RTable(QWidget *parent) : RMapObject(parent)
{
    setModel(new RTableModel);
}

//--------------------------------------------------------------------------------------
RTable::~RTable()
{
    RMapRegistry::instance().addTableForDelete(model()->mapObjectID());
}

//--------------------------------------------------------------------------------------
RTable *RTable::clone() const
{
	RTable *table = new RTable(parentWidget());
	table->setModel(model()->clone());
	table->setMovePolicy(movePolicy()->clone());
	table->setResizePolicy(resizePolicy()->clone());
	table->setGeometry(geometry());
	table->setVisible(isVisible());
	return table;
}

//--------------------------------------------------------------------------------------
RTableModel *RTable::model() const
{ return static_cast<RTableModel *>(RMapObject::model()); }

//--------------------------------------------------------------------------------------
void RTable::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
    const QImage &image = model()->pictureFree();
    QPen pen;

    if (!image.isNull()) {
        painter.drawImage(rect().center() - image.rect().center(), image);
    }

    if (hasFocusMapObject()) {
        pen.setColor(Qt::blue);
        pen.setWidth(3);
    } else {
        pen.setColor(Qt::gray);
        pen.setWidth(1);
    }

    // рисуем прямоугольник вокруг столика
    painter.setPen(pen);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    // рисуем номер столика
    pen.setColor(model()->numberFontColor());
    painter.setPen(pen);
    painter.setFont(model()->numberFont());
    painter.drawText(rect(), QString().number(model()->number()), QTextOption(Qt::AlignCenter));

    // рисуем текст на столике
    pen.setColor(model()->textFontColor());
    painter.setPen(pen);
    painter.setFont(model()->textFont());
    painter.drawText(rect(), model()->text(), QTextOption(Qt::AlignBottom | Qt::AlignHCenter));
}

//--------------------------------------------------------------------------------------
void RTable::resizeEvent(QResizeEvent *event)
{
	movePolicy()->setMovePolygon(QPolygon(rect().adjusted(5, 5, -5, -5), true));
	RMapObject::resizeEvent(event);
}
