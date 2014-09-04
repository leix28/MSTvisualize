#ifndef __canvas_h__
#define __canvas_h__

#include "spantree.h"
#include <QGraphicsView>
#include <QGraphicsItemGroup>

class CanvasPainter : public QGraphicsScene {
    Q_OBJECT

    QGraphicsItemGroup *pointGroup, *edgeGroup;
    void clearPoints();
    void clearEdges();

public:
    CanvasPainter(QWidget *parent = 0);
    void drawPoints(Spantree::PointIds points);
    void drawEdges(Spantree::Graph graph);
};



class Canvas : public QGraphicsView {
    Q_OBJECT
    CanvasPainter *painter;

public:
    Canvas(QWidget *parent = 0);
    CanvasPainter* getPainter();
};

#endif // CANVAS_H
