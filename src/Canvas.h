#ifndef __canvas_h__
#define __canvas_h__

#include "spantree.h"
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>

class CanvasPainter : public QGraphicsScene {
    Q_OBJECT

    QGraphicsItemGroup *pointGroup, *edgeGroup;
    void clearPoints();
    void clearEdges();
    QPointF getCenter();
    void setViewCenter(QPointF ctr);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

public:
    CanvasPainter(QWidget *parent = 0);
    void drawPoints(Spantree::PointIds points);
    void drawEdges(Spantree::Graph graph);

signals:
    void addPoint(double x, double y);
};



class Canvas : public QGraphicsView {
    Q_OBJECT
    CanvasPainter *painter;

public:
    Canvas(QWidget *parent = 0);
    CanvasPainter* getPainter();
};

#endif // CANVAS_H
