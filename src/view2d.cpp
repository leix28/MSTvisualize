#include "view2d.h"
#include <fstream>
#include <QFileDialog>
#include <QStandardItem>
#include <QHeaderView>
#include <ctime>

void View2D::loadFileThread(std::string filename) {

    std::ifstream reader(filename);
    if (!reader.is_open()) return;
    double x, y;

    std::vector< std::pair<double, double> > points;
    while (reader >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }

    solver.clear();
    if (isShowInputPoints->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(inputPoints);
    delete inputPoints;
    inputPoints = 0;
    if (isShowDelaunayEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(delaunayEdges);
    delete delaunayEdges;
    delaunayEdges = 0;
    if (isShowMSTreeEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(mSTreeEdges);
    delete mSTreeEdges;
    mSTreeEdges = 0;
    if (isShowVoronoiEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(voronoiEdges);
    delete voronoiEdges;
    voronoiEdges = 0;

    solver.insert(points);

    emit datasetChange();

}

void View2D::updateGraphItem() {
    isShowInputPoints->setEnabled(0);
    isShowDelaunayEdges->setEnabled(0);
    isShowVoronoiEdges->setEnabled(0);
    isShowMSTreeEdges->setEnabled(0);
    emit drawInputPoints(solver.getPointIds());
    Spantree::Graph graph = solver.getDelaunay();
    emit drawDelaunayEdges(graph);
    graph = solver.getMSTree();
    emit drawMSTreeEdges(graph);
    graph = solver.getVoronoi();
    emit drawVoronoiEdges(graph);
}

QMenuBar *View2D::getMenuBar() {
    return menubar;
}

View2D::View2D(QWidget *parent) : QWidget(parent) {
    canvas = new Canvas(this);
    scene = new QGraphicsScene;
    label = new QLabel("Command: ", this);
    command = new QLineEdit(this);



    isShowInputPoints = new QCheckBox("Input Points", this);
    isShowInputPoints->setCheckState(Qt::Checked);
    isShowDelaunayEdges = new QCheckBox("Delaunay Graph", this);
    isShowDelaunayEdges->setCheckState(Qt::Unchecked);
    isShowVoronoiEdges = new QCheckBox("Voronoi Graph", this);
    isShowVoronoiEdges->setCheckState(Qt::Unchecked);
    isShowMSTreeEdges = new QCheckBox("Minimum Spanning Tree", this);
    isShowMSTreeEdges->setCheckState(Qt::Unchecked);


    buttonZoomIn = new QPushButton("Zoom In", this);
    buttonZoomIn->setAutoRepeat(1);
    buttonZoomOut = new QPushButton("Zoom Out", this);
    buttonZoomOut->setAutoRepeat(1);
    pointTable = new QTableView(this);

    outerLayout = new QVBoxLayout();
    mainLayout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    leftBottomLayout = new QHBoxLayout();

    inputPoints = delaunayEdges = voronoiEdges = mSTreeEdges = 0;

    menubar = new QMenuBar();
    toolbar = new QToolBar();
    menuFile = new QMenu("File", menubar);
    menubar->addMenu(menuFile);
    actionLoad = new QAction("Load", menuFile);
    actionLoad->setIcon(QIcon(":/load.icns"));
    menuFile->addAction(actionLoad);
    toolbar->addAction(actionLoad);
    menuEdit = new QMenu("Edit", menubar);
    menubar->addMenu(menuEdit);
    actionAdd = new QAction("Add", menuEdit);
    actionAdd->setIcon(QIcon(":/add.icns"));
    menuEdit->addAction(actionAdd);
    toolbar->addAction(actionAdd);

    outerLayout->addWidget(toolbar);
    outerLayout->addLayout(mainLayout);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    leftLayout->addWidget(canvas);
    leftLayout->addLayout(leftBottomLayout);
    rightLayout->addWidget(isShowInputPoints);
    rightLayout->addWidget(isShowDelaunayEdges);
    rightLayout->addWidget(isShowMSTreeEdges);
    rightLayout->addWidget(isShowVoronoiEdges);
    rightLayout->addWidget(pointTable);
    leftBottomLayout->addWidget(label);
    leftBottomLayout->addWidget(command);
    leftBottomLayout->addWidget(buttonZoomIn);
    leftBottomLayout->addWidget(buttonZoomOut);

    pointTable->setFixedWidth(300);
    pointTable->verticalHeader()->hide();
    pointTableModel = new QStandardItemModel(pointTable);
    pointTableModel->setHorizontalHeaderItem(0, new QStandardItem("Index"));
    pointTableModel->setHorizontalHeaderItem(1, new QStandardItem("X"));
    pointTableModel->setHorizontalHeaderItem(2, new QStandardItem("Y"));
    this->setLayout(outerLayout);

    qRegisterMetaType<Spantree::PointIds>("Spantree::PointIds");
    qRegisterMetaType<Spantree::Graph>("Spantree::Graph");

    QObject::connect(actionLoad, SIGNAL(triggered()), this, SLOT(loadFile()));

    QObject::connect(isShowInputPoints, SIGNAL(stateChanged(int)), this, SLOT(switchInputPoints()));
    QObject::connect(isShowDelaunayEdges, SIGNAL(stateChanged(int)), this, SLOT(switchDelaunayEdges()));
    QObject::connect(isShowMSTreeEdges, SIGNAL(stateChanged(int)), this, SLOT(switchMSTreeEdges()));
    QObject::connect(isShowVoronoiEdges, SIGNAL(stateChanged(int)), this, SLOT(switchVoronoiEdges()));
    QObject::connect(buttonZoomIn, SIGNAL(clicked()), this->canvas, SLOT(zoomIn()));
    QObject::connect(buttonZoomOut, SIGNAL(clicked()), this->canvas, SLOT(zoomOut()));

    QObject::connect(this, SIGNAL(datasetChange()), this, SLOT(updateItem()));

    QObject::connect(canvas->getPainter(), SIGNAL(addPoint(double, double)), this, SLOT(addPoint(double, double)));
    QObject::connect(this, SIGNAL(drawInputPoints(Spantree::PointIds)), this, SLOT(drawInput(Spantree::PointIds)));
    QObject::connect(this, SIGNAL(drawDelaunayEdges(Spantree::Graph)), this, SLOT(drawDelaunay(Spantree::Graph)));
    QObject::connect(this, SIGNAL(drawMSTreeEdges(Spantree::Graph)), this, SLOT(drawMSTree(Spantree::Graph)));
    QObject::connect(this, SIGNAL(drawVoronoiEdges(Spantree::Graph)), this, SLOT(drawVoronoi(Spantree::Graph)));
}

View2D::~View2D() {
    solver.stop = 1;
    algorithmThread.join();
}

void View2D::addPoint(double x, double y) {
    Spantree::Points pts;
    pts.push_back(std::make_pair(x, y));
    solver.insert(pts);
    emit datasetChange();
}

void View2D::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Load", QDir::homePath(), "Text files (*.txt)");
    boost::thread t = boost::thread(boost::bind(&View2D::loadFileThread, this, filename.toStdString()));
    t.detach();
}

void View2D::updateItem() {
    solver.stop = 1;
    algorithmThread.join();
    solver.stop = 0;
    algorithmThread = boost::thread(boost::bind(&View2D::updateGraphItem, this));

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

}

void View2D::switchInputPoints() {
    if (isShowInputPoints->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(inputPoints);
    else
        canvas->getPainter()->eraseItems(inputPoints);
}

void View2D::switchDelaunayEdges() {
    if (isShowDelaunayEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(delaunayEdges);
    else
        canvas->getPainter()->eraseItems(delaunayEdges);
}

void View2D::switchVoronoiEdges() {
    if (isShowVoronoiEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(voronoiEdges);
    else
        canvas->getPainter()->eraseItems(voronoiEdges);
}

void View2D::switchMSTreeEdges() {
    if (isShowMSTreeEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(mSTreeEdges);
    else
        canvas->getPainter()->eraseItems(mSTreeEdges);
}

void View2D::drawInput(Spantree::PointIds pts) {
    inputPoints = canvas->getPainter()->drawPoints(pts);
    if (isShowInputPoints->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(inputPoints);
    isShowInputPoints->setEnabled(1);
}

void View2D::drawDelaunay(Spantree::Graph graph) {
    delaunayEdges = canvas->getPainter()->drawEdges(graph);
    if (isShowDelaunayEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(delaunayEdges);
    isShowDelaunayEdges->setEnabled(1);
}

void View2D::drawVoronoi(Spantree::Graph graph) {
    voronoiEdges = canvas->getPainter()->drawEdges(graph);
    if (isShowVoronoiEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(voronoiEdges);
    isShowVoronoiEdges->setEnabled(1);
}

void View2D::drawMSTree(Spantree::Graph graph) {
    mSTreeEdges = canvas->getPainter()->drawEdges(graph);
    if (isShowMSTreeEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(mSTreeEdges);
    isShowMSTreeEdges->setEnabled(1);
}
