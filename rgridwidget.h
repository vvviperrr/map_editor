#ifndef RGRIDWIDGET_H
#define RGRIDWIDGET_H

#include <QFrame>
#include <QPolygon>

class RGridWidget : public QFrame {
    Q_OBJECT
public:
	explicit RGridWidget(QWidget *parent = 0, int size = 0);
    ~RGridWidget();

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:
	void updatePolygonData();

	int gridSize;
	QPolygon polygon;
};

#endif // RGRIDWIDGET_H
