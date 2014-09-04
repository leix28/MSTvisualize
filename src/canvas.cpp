#include "canvas.h"

CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(parent) {
    setBackgroundBrush(Qt::blue);
    edgeGroup = new QGraphicsItemGroup();
    pointGroup = new QGraphicsItemGroup();
}


void CanvasPainter::clearPoints() {
    delete pointGroup;
    pointGroup = new QGraphicsItemGroup();
}

void CanvasPainter::clearEdges() {
    delete edgeGroup;
    edgeGroup = new QGraphicsItemGroup();
}

void CanvasPainter::drawPoints(Spantree::PointIds points) {
    clearPoints();
    clearEdges();

    for (auto entry : points) {
        pointGroup->addToGroup(new QGraphicsEllipseItem(entry.second.first - 2, entry.second.second - 2, 4, 4));
    }
    addItem(pointGroup);
    setSceneRect(itemsBoundingRect());
}

void CanvasPainter::drawEdges(Spantree::Graph graph) {
    clearEdges();
    for (auto entry : graph.second) {
        edgeGroup->addToGroup(
                    new QGraphicsLineItem(graph.first[entry.first].first,
                                          graph.first[entry.first].second,
                                          graph.first[entry.second].first,
                                          graph.first[entry.second].second));
    }
    addItem(edgeGroup);
    setSceneRect(itemsBoundingRect());
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent) {
    painter = new CanvasPainter(this);
    setScene(painter);
}

CanvasPainter *Canvas::getPainter() {
    return painter;
}



