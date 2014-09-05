#include "canvas.h"
#include <QDebug>

CanvasPainter::CanvasPainter(QWidget *parent) : QGraphicsScene(parent) {
    view = (Canvas*)parent;
}



QPointF CanvasPainter::getCenter() {
    return view->mapToScene(view->size().width() / 2 - 8, view->size().height() / 2 - 6);
}

void CanvasPainter::setViewCenter(QPointF ctr) {
    setSceneRect(itemsBoundingRect());
    QRectF rect = sceneRect();
    rect.setLeft(rect.left() - 200);
    rect.setRight(rect.right() + 200);
    rect.setTop(rect.top() - 200);
    rect.setBottom(rect.bottom() + 200);

    rect.setLeft(std::min(rect.left(), ctr.rx() - view->size().width() / view->transform().m11() / 2 - 200));
    rect.setRight(std::max(rect.right(), ctr.rx() + view->size().width() / view->transform().m11() / 2 + 200));
    rect.setTop(std::min(rect.top(), ctr.ry() - view->size().height() / view->transform().m22() / 2 - 200));
    rect.setBottom(std::max(rect.bottom(), ctr.ry() + view->size().height() / view->transform().m22() / 2 + 200));

    setSceneRect(rect);
    view->centerOn(ctr);
    view->guide->autoFit();
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

QGraphicsItemGroup *CanvasPainter::drawPoints(Spantree::PointIds points, QPen pen) {
    all_mutex.lock();
    QGraphicsItemGroup *pointGroup = new QGraphicsItemGroup();
    for (auto entry : points) {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(entry.second.first - 2, entry.second.second - 2, 4, 4);
        item->setPen(pen);
        pointGroup->addToGroup(item);
    }
    all_mutex.unlock();
    return pointGroup;
}

QGraphicsItemGroup* CanvasPainter::drawEdges(Spantree::Graph graph, QPen pen) {
    all_mutex.lock();
    QGraphicsItemGroup *edgeGroup = new QGraphicsItemGroup();
    for (auto entry : graph.second) {
        QGraphicsLineItem *item = new QGraphicsLineItem(graph.first[entry.first].first,
                                                        graph.first[entry.first].second,
                                                        graph.first[entry.second].first,
                                                        graph.first[entry.second].second);
        item->setPen(pen);
        edgeGroup->addToGroup(item);
    }
    all_mutex.unlock();
    return edgeGroup;
}

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent) {
    painter = new CanvasPainter(this);
    setScene(painter);
    guide = new Guide(this);
    guide->setFixedSize(200, 200);
    guide->setScene(painter);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    guide->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

CanvasPainter* Canvas::getPainter() {
    return painter;
}

void Canvas::zoom(int flag) {
    if (flag > 0) {
        auto ctr = painter->getCenter();
        scale(1.1, 1.1);
        painter->setViewCenter(ctr);
    } else {
        auto ctr = painter->getCenter();
        scale(0.9, 0.9);
        painter->setViewCenter(ctr);
    }
}

Guide* Canvas::getGuide() {
    return guide;
}

void Canvas::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    painter->setViewCenter(painter->getCenter());
    guide->autoFit();
}

void Canvas::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
    guide->viewport()->update();
}

void Canvas::zoomIn() {
    zoom(1);
    guide->autoFit();
}

void Canvas::zoomOut() {
    zoom(-1);
    guide->autoFit();
}


Guide::Guide(QWidget *parent) : QGraphicsView(parent){
    view = (Canvas*)parent;
}

void Guide::paintEvent(QPaintEvent *event) {
    QPainter painter(this->viewport());
    painter.drawRect(mapFromScene(view->mapToScene(0, 0, view->size().width(), view->size().height()).boundingRect()).boundingRect());
    QGraphicsView::paintEvent(event);
}


void Guide::mouseMoveEvent(QMouseEvent * event) {
    view->getPainter()->setViewCenter(mapToScene(event->pos()));
    QGraphicsView::mouseMoveEvent(event);
}

void Guide::autoFit() {
    auto rect = view->getPainter()->sceneRect();
    resetTransform();
    scale((size().width() - 10) / (double)rect.width(),
          (size().height() - 10) / (double)rect.height());
    viewport()->update();
}
