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
#include <QCheckBox>
#include <QMenuBar>
#include <QToolBar>
#include <QDialog>
#include <QDoubleSpinBox>
#include <boost/thread/thread.hpp>

class AddPointDialog : public QDialog {
    Q_OBJECT
    QHBoxLayout *mainLayout;
    QVBoxLayout *vLayout;
    QPushButton *buttonY, *buttonN;
    QDoubleSpinBox *xpos, *ypos;
    QLabel *xlabel, *ylabel;
public:
    AddPointDialog (QWidget *parent);
    static std::pair< bool, std::pair<double, double> > getPoint(QWidget *parent);
};

class TableView : public QTableView {
    Q_OBJECT
public:
    TableView(QWidget *parent);
    QModelIndexList selectedIndexes() const;
};


class View2D : public QWidget {
    Q_OBJECT
    QVBoxLayout *outerLayout;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *leftBottomLayout;
    QGridLayout *canvasLayout;
    QGridLayout *guideLayout;

    QCheckBox *isShowInputPoints;
    QCheckBox *isShowDelaunayEdges;
    QCheckBox *isShowVoronoiEdges;
    QCheckBox *isShowMSTreeEdges;

    Canvas *canvas;
    Guide *guide;
    QPushButton *buttonZoomIn;
    QPushButton *buttonZoomOut;
    QPushButton *buttonErase;
    QLabel *label;
    QLineEdit *command;
    TableView *pointTable;
    QStandardItemModel *pointTableModel;
    Spantree solver;
    QGraphicsItemGroup *inputPoints;
    QGraphicsItemGroup *delaunayEdges;
    QGraphicsItemGroup *voronoiEdges;
    QGraphicsItemGroup *mSTreeEdges;
    QPen pointPen, edgePen;


    QMenuBar *menubar;
    QToolBar *toolbar;
    QMenu *menuFile, *menuEdit;
    QAction *actionLoad, *actionAdd;

    boost::thread algorithmThread, t;
    void erasePointThread(std::set<int> pts);
    void loadFileThread(std::string);
    void addPointThread(double x, double y);
    void updateGraphItem();

public:
    QMenuBar* getMenuBar();
    explicit View2D(QWidget *parent = 0);
    ~View2D();

signals:
    void datasetChange();
    void drawInputPoints(Spantree::PointIds);
    void drawDelaunayEdges(Spantree::Graph);
    void drawVoronoiEdges(Spantree::Graph);
    void drawMSTreeEdges(Spantree::Graph);

public slots:
    void addPoint(double x, double y);
    void loadFile();
    void updateItem();
    void switchInputPoints();
    void switchDelaunayEdges();
    void switchVoronoiEdges();
    void switchMSTreeEdges();
    void drawInput(Spantree::PointIds pts);
    void drawDelaunay(Spantree::Graph graph);
    void drawVoronoi(Spantree::Graph graph);
    void drawMSTree(Spantree::Graph graph);
    void addPointDialog();
    void erase();
};

#endif
