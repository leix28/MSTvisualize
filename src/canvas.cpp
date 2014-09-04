#include "canvas.h"

CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(-500, -500, 1000, 1000, parent) {
    std::cout << sceneRect().width() << std::endl;
    //setBackgroundBrush(Qt::blue);
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

QPointF CanvasPainter::getCenter() {
    const QGraphicsView* view = (QGraphicsView*) parent();
    return view->mapToScene(view->size().width() / 2 - 8, view->size().height() / 2 - 6);
}

void CanvasPainter::setViewCenter(QPointF ctr) {
    QGraphicsView* view = (QGraphicsView*) parent();
    setSceneRect(itemsBoundingRect());
    QRectF rect = sceneRect();
    rect.setLeft(rect.left() - 200);
    rect.setRight(rect.right() + 200);
    rect.setTop(rect.top() - 200);
    rect.setBottom(rect.bottom() + 200);

    rect.setLeft(std::min(rect.left(), ctr.rx() - view->size().width() / 2));
    rect.setRight(std::max(rect.right(), ctr.rx() + view->size().width() / 2));
    rect.setTop(std::min(rect.top(), ctr.ry() - view->size().height() / 2));
    rect.setBottom(std::max(rect.bottom(), ctr.ry() + view->size().height() / 2));

    setSceneRect(rect);
    view->centerOn(ctr);
}

void CanvasPainter::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    emit addPoint(mouseEvent->scenePos().rx(), mouseEvent->scenePos().ry());
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void CanvasPainter::drawPoints(Spantree::PointIds points) {
    QPointF ctr = getCenter();

    clearPoints();
    clearEdges();

    for (auto entry : points) {
        pointGroup->addToGroup(new QGraphicsEllipseItem(entry.second.first - 2, entry.second.second - 2, 4, 4));
    }
    addItem(pointGroup);
    setViewCenter(ctr);
}

void CanvasPainter::drawEdges(Spantree::Graph graph) {
    QPointF ctr = getCenter();
    clearEdges();
    for (auto entry : graph.second) {
        edgeGroup->addToGroup(
                    new QGraphicsLineItem(graph.first[entry.first].first,
                                          graph.first[entry.first].second,
                                          graph.first[entry.second].first,
                                          graph.first[entry.second].second));
    }
    addItem(edgeGroup);
    setViewCenter(ctr);
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent) {
    painter = new CanvasPainter(this);
    setScene(painter);
}

CanvasPainter *Canvas::getPainter() {
    return painter;
}



