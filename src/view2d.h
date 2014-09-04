#ifndef __view2d_h__
#define __view2d_h__

#include "spantree.h"
#include "canvas.h"
#include <QWidget>
#include <QGroupBox>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>

class View2D : public QWidget {
    Q_OBJECT

    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *leftBottomLayout;

    Canvas *canvas;
    QGraphicsScene *scene;
    QPushButton *buttonLoad;
    QPushButton *buttonDelaunay;
    QPushButton *buttonVoronoi;
    QPushButton *buttonMSTree;
    QLabel *label;
    QLineEdit *command;
    QTableView *pointTable;
    QStandardItemModel *pointTableModel;
    Spantree solver;

    void loadFileThread(std::string);

public:
    explicit View2D(QWidget *parent = 0);

signals:
    void loadFileThreadFinish();

public slots:
    void addPoint(double x, double y);
    void loadFile();
    void loadFileThreadUIResponce();
    void drawDelaunay();
    void drawVoronoi();
    void drawMSTree();
};

#endif
