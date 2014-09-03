#include "canvas.h"

CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(parent) {

}


void Canvas::clearPoints() {
    for (auto item : pointGroup) {
        painter->removeItem(item);
        delete item;
    }
    pointGroup.clear();
}

void Canvas::clearEdges() {
    for (auto item : edgeGroup) {
        painter->removeItem(item);
        delete item;
    }
    edgeGroup.clear();
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent){
    painter = new CanvasPainter(this);
    this->setScene(painter);
}


void Canvas::drawPoints(Spantree::PointIds points) {
    clearPoints();
    clearEdges();

    for (auto entry : points) {
        pointGroup.push_back(painter->addEllipse(entry.second.first - 2, entry.second.second - 2, 4, 4));
    }
    painter->setSceneRect(painter->itemsBoundingRect());
}

void Canvas::drawEdges(Spantree::Graph graph) {
    clearEdges();
    for (auto entry : graph.second) {
        edgeGroup.push_back(
                    painter->addLine(graph.first[entry.first].first,
                                     graph.first[entry.first].second,
                                     graph.first[entry.second].first,
                                     graph.first[entry.second].second));
    }
    painter->setSceneRect(painter->itemsBoundingRect());
}

