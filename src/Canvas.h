#ifndef CANVAS_H
#define CANVAS_H

#include "spantree.h"
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <vector>

class CanvasPainter : public QGraphicsScene {
    Q_OBJECT
public:
    CanvasPainter(QWidget *parent = 0);
};



class Canvas : public QGraphicsView {
    Q_OBJECT
public:
    CanvasPainter *painter;
    QGraphicsItemGroup *pointGroup, *edgeGroup;

    void clearPoints();
    void clearEdges();
public:
    Canvas(QWidget *parent = 0);
    void drawPoints(Spantree::PointIds points);
    void drawEdges(Spantree::Graph graph);
};

#endif // CANVAS_H
