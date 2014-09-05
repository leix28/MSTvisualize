#ifndef __canvas_h__
#define __canvas_h__

#include "spantree.h"
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QPen>
#include <mutex>

class Canvas;

class CanvasPainter : public QGraphicsScene {
    Q_OBJECT
    friend class Canvas;
    friend class Guide;
    QPointF getCenter();
    void setViewCenter(QPointF ctr);
    std::mutex all_mutex;
    Canvas *view;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

public:
    void eraseItems(QGraphicsItemGroup *items);
    void showItems(QGraphicsItemGroup *items);
    CanvasPainter(QWidget *parent = 0);
    QGraphicsItemGroup* drawPoints(Spantree::PointIds points, QPen pen);
    QGraphicsItemGroup* drawEdges(Spantree::Graph graph, QPen pen);

signals:
    void addPoint(double x, double y);
};

class Guide : public QGraphicsView {
    Q_OBJECT
    Canvas *view;

public:
    Guide(QWidget *parent);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
public slots:
    void autoFit();
};



class Canvas : public QGraphicsView {
    Q_OBJECT
    CanvasPainter *painter;
    Guide *guide;

    friend class CanvasPainter;

public:
    Canvas(QWidget *parent = 0);
    CanvasPainter* getPainter();
    void zoom(int flag);
    Guide* getGuide();

protected:
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent *event);
public slots:
    void zoomIn();
    void zoomOut();
};

#endif // CANVAS_H
