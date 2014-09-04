#include "canvas.h"
#include <ctime>
#include <iostream>

CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(parent) {

}


void Canvas::clearPoints() {
    delete pointGroup;
    pointGroup = new QGraphicsItemGroup();
}

void Canvas::clearEdges() {
    delete edgeGroup;
    edgeGroup = new QGraphicsItemGroup();
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent){
    painter = new CanvasPainter(this);
    this->setScene(painter);
    edgeGroup = new QGraphicsItemGroup();
    pointGroup = new QGraphicsItemGroup();
}


void Canvas::drawPoints(Spantree::PointIds points) {
    clearPoints();
    clearEdges();

    for (auto entry : points) {
        pointGroup->addToGroup(new QGraphicsEllipseItem(entry.second.first - 2, entry.second.second - 2, 4, 4));
    }
    painter->addItem(pointGroup);
    painter->setSceneRect(painter->itemsBoundingRect());
}

void Canvas::drawEdges(Spantree::Graph graph) {
    clearEdges();
    for (auto entry : graph.second) {
        edgeGroup->addToGroup(
                    new QGraphicsLineItem(graph.first[entry.first].first,
                                          graph.first[entry.first].second,
                                          graph.first[entry.second].first,
                                          graph.first[entry.second].second));
    }
    painter->addItem(edgeGroup);
    painter->setSceneRect(painter->itemsBoundingRect());
}

