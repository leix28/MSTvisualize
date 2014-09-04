#include "canvas.h"


CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(-500, -500, 1000, 1000, parent) {
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

void CanvasPainter::eraseItems(QGraphicsItemGroup *items) {
    all_mutex.lock();
    QPointF ctr = getCenter();
    if (items)
        removeItem(items);
    setViewCenter(ctr);
    all_mutex.unlock();
}


void CanvasPainter::showItems(QGraphicsItemGroup *items) {
    all_mutex.lock();
    QPointF ctr = getCenter();
    addItem(items);
    setViewCenter(ctr);
    all_mutex.unlock();
}

QGraphicsItemGroup *CanvasPainter::drawPoints(Spantree::PointIds points) {
    all_mutex.lock();
    QGraphicsItemGroup *pointGroup = new QGraphicsItemGroup();
    for (auto entry : points) {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(entry.second.first - 2, entry.second.second - 2, 4, 4);
        pointGroup->addToGroup(item);
    }
    all_mutex.unlock();
    return pointGroup;
}

QGraphicsItemGroup* CanvasPainter::drawEdges(Spantree::Graph graph) {
    all_mutex.lock();
    QGraphicsItemGroup *edgeGroup = new QGraphicsItemGroup();
    for (auto entry : graph.second) {
        QGraphicsLineItem *item = new QGraphicsLineItem(graph.first[entry.first].first,
                                                        graph.first[entry.first].second,
                                                        graph.first[entry.second].first,
                                                        graph.first[entry.second].second);
        edgeGroup->addToGroup(item);
    }
    all_mutex.unlock();
    return edgeGroup;
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent) {
    painter = new CanvasPainter(this);
    setScene(painter);
}

CanvasPainter *Canvas::getPainter() {
    return painter;
}

void Canvas::zoom(int flag) {
    if (flag > 0)
        scale(1.1, 1.1);
    else
        scale(0.9, 0.9);
}

void Canvas::zoomIn() {
    zoom(1);
}

void Canvas::zoomOut() {
    zoom(-1);
}
