#include "view2d.h"
#include <fstream>
#include <QFileDialog>
#include <QStandardItem>
#include <QHeaderView>
#include <ctime>
#include <thread>

void View2D::loadFileThread(std::string filename) {

    std::ifstream reader(filename);
    if (!reader.is_open()) return;
    double x, y;

    std::vector< std::pair<double, double> > points;
    while (reader >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }

    solver.clear();
    solver.insert(points);

    emit loadFileThreadFinish();

}

View2D::View2D(QWidget *parent) : QWidget(parent) {

    canvas = new Canvas(this);
    scene = new QGraphicsScene;
    label = new QLabel("Command: ", this);
    command = new QLineEdit(this);
    buttonLoad = new QPushButton("Load", this);
    buttonDelaunay = new QPushButton("Delaunay Diagram", this);
    buttonVoronoi = new QPushButton("Voronoi Diagram", this);
    buttonMSTree = new QPushButton("Minimum Spanning Tree", this);
    pointTable = new QTableView(this);

    mainLayout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    leftBottomLayout = new QHBoxLayout();

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    leftLayout->addWidget(canvas);
    leftLayout->addLayout(leftBottomLayout);
    rightLayout->addWidget(buttonLoad);
    rightLayout->addWidget(buttonDelaunay);
    rightLayout->addWidget(buttonVoronoi);
    rightLayout->addWidget(buttonMSTree);
    rightLayout->addWidget(pointTable);
    leftBottomLayout->addWidget(label);
    leftBottomLayout->addWidget(command);

    pointTable->setFixedWidth(300);
    pointTable->verticalHeader()->hide();
    pointTableModel = new QStandardItemModel(pointTable);
    pointTableModel->setHorizontalHeaderItem(0, new QStandardItem("Index"));
    pointTableModel->setHorizontalHeaderItem(1, new QStandardItem("X"));
    pointTableModel->setHorizontalHeaderItem(2, new QStandardItem("Y"));
    this->setLayout(mainLayout);

    QObject::connect(buttonLoad, SIGNAL(clicked()), this, SLOT(loadFile()));
    QObject::connect(buttonDelaunay, SIGNAL(clicked()), this, SLOT(drawDelaunay()));
    QObject::connect(buttonVoronoi, SIGNAL(clicked()), this, SLOT(drawVoronoi()));
    QObject::connect(buttonMSTree, SIGNAL(clicked()), this, SLOT(drawMSTree()));
    QObject::connect(this, SIGNAL(loadFileThreadFinish()), this, SLOT(loadFileThreadUIResponce()));

    QObject::connect(canvas->getPainter(), SIGNAL(addPoint(double, double)), this, SLOT(addPoint(double, double)));
}

void View2D::addPoint(double x, double y) {
    Spantree::Points pts;
    pts.push_back(std::make_pair(x, y));
    solver.insert(pts);
    emit loadFileThreadFinish();
}

void View2D::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Load", QDir::homePath(), "Text files (*.txt)");
    std::thread t = std::thread(std::bind(&View2D::loadFileThread, this, filename.toStdString()));
    t.detach();
}

void View2D::loadFileThreadUIResponce() {
    canvas->getPainter()->drawPoints(solver.getPointIds());
    update();

    pointTable->setModel(0);
    pointTableModel->clear();
    int cnt = 0;
    for (auto entry : solver.getPointIds()) {
        pointTableModel->setItem(cnt, 0, new QStandardItem(QString::number(entry.first)));
        pointTableModel->setItem(cnt, 1, new QStandardItem(QString::number(entry.second.first, 'g', 3)));
        pointTableModel->setItem(cnt, 2, new QStandardItem(QString::number(entry.second.second, 'g', 3)));
        cnt++;
    }
    pointTable->setModel(pointTableModel);
    update();
}

void View2D::drawDelaunay() {
    auto delaunay = solver.getDelaunay();
    canvas->getPainter()->drawEdges(delaunay);
}

void View2D::drawVoronoi() {
    auto voronoi = solver.getVoronoi();
    canvas->getPainter()->drawEdges(voronoi);
}

void View2D::drawMSTree() {
    auto mst = solver.getMSTree();
    canvas->getPainter()->drawEdges(mst);
}
