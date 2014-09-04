#ifndef __canvas_h__
#define __canvas_h__

#include "spantree.h"
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QPen>
#include <mutex>

class CanvasPainter : public QGraphicsScene {
    Q_OBJECT

    QPointF getCenter();
    void setViewCenter(QPointF ctr);
    std::mutex all_mutex;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

public:
    void eraseItems(QGraphicsItemGroup *items);
    void showItems(QGraphicsItemGroup *items);
    CanvasPainter(QWidget *parent = 0);
    QGraphicsItemGroup* drawPoints(Spantree::PointIds points);
    QGraphicsItemGroup* drawEdges(Spantree::Graph graph);

signals:
    void addPoint(double x, double y);
};



class Canvas : public QGraphicsView {
    Q_OBJECT
    CanvasPainter *painter;

public:
    Canvas(QWidget *parent = 0);
    CanvasPainter* getPainter();
    void zoom(int flag);

public slots:
    void zoomIn();
    void zoomOut();
};

#endif // CANVAS_H
