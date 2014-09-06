#include "view2d.h"
#include <fstream>
#include <QFileDialog>
#include <QStandardItem>
#include <QHeaderView>
#include <ctime>
#include <set>

void View2D::erasePointThread(std::set<int> pts) {
    isShowInputPoints->setEnabled(0);
    isShowDelaunayEdges->setEnabled(0);
    isShowVoronoiEdges->setEnabled(0);
    isShowMSTreeEdges->setEnabled(0);

    //if (isShowInputPoints->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(inputPoints);
    if (inputPoints) delete inputPoints;
    delete highlightPoint;
    highlightPoint = 0;
    inputPoints = 0;
    //if (isShowDelaunayEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(delaunayEdges);
    if (delaunayEdges) delete delaunayEdges;
    delaunayEdges = 0;
    //if (isShowMSTreeEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(mSTreeEdges);
    if (mSTreeEdges) delete mSTreeEdges;
    mSTreeEdges = 0;
    //if (isShowVoronoiEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(voronoiEdges);
    if (voronoiEdges) delete voronoiEdges;
    voronoiEdges = 0;

    solver.erase(pts);

    emit datasetChange();
}

void View2D::loadFileThread(std::string filename) {
    isShowInputPoints->setEnabled(0);
    isShowDelaunayEdges->setEnabled(0);
    isShowVoronoiEdges->setEnabled(0);
    isShowMSTreeEdges->setEnabled(0);

    std::ifstream reader(filename);
    if (!reader.is_open()) return;
    double x, y;

    std::vector< std::pair<double, double> > points;
    while (reader >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }

    solver.clear();
    //if (isShowInputPoints->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(inputPoints);
    if (inputPoints) delete inputPoints;
    inputPoints = 0;
    delete highlightPoint;
    highlightPoint = 0;
    //if (isShowDelaunayEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(delaunayEdges);
    if (delaunayEdges) delete delaunayEdges;
    delaunayEdges = 0;
    //if (isShowMSTreeEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(mSTreeEdges);
    if (mSTreeEdges) delete mSTreeEdges;
    mSTreeEdges = 0;
    //if (isShowVoronoiEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(voronoiEdges);
    if (voronoiEdges) delete voronoiEdges;
    voronoiEdges = 0;

    solver.insert(points);

    emit datasetChange();

}

void View2D::addPointThread(double x, double y) {
    isShowInputPoints->setEnabled(0);
    isShowDelaunayEdges->setEnabled(0);
    isShowVoronoiEdges->setEnabled(0);
    isShowMSTreeEdges->setEnabled(0);

    if (isShowInputPoints->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(inputPoints);
    if (inputPoints) delete inputPoints;
    inputPoints = 0;
    if (isShowDelaunayEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(delaunayEdges);
    if (delaunayEdges) delete delaunayEdges;
    delaunayEdges = 0;
    if (isShowMSTreeEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(mSTreeEdges);
    if (mSTreeEdges) delete mSTreeEdges;
    mSTreeEdges = 0;
    if (isShowVoronoiEdges->checkState() == Qt::Checked) canvas->getPainter()->eraseItems(voronoiEdges);
    if (voronoiEdges) delete voronoiEdges;
    voronoiEdges = 0;

    Spantree::Points pts;
    pts.push_back(std::make_pair(x, y));
    solver.insert(pts);
    emit datasetChange();
}

void View2D::updateGraphItem() {
    emit drawInputPoints(solver.getPointIds());
    if (solver.stop) return;
    Spantree::Graph graph = solver.getDelaunay();
    if (solver.stop) return;
    emit drawDelaunayEdges(graph);
    if (solver.stop) return;
    graph = solver.getMSTree();
    if (solver.stop) return;
    emit drawMSTreeEdges(graph);
    if (solver.stop) return;
    graph = solver.getVoronoi();
    if (solver.stop) return;
    emit drawVoronoiEdges(graph);
}

QMenuBar *View2D::getMenuBar() {
    return menubar;
}

View2D::View2D(QWidget *parent) : QWidget(parent) {
    canvas = new Canvas(this);
    guide = canvas->getGuide();
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

    pointPen.setWidth(2);
    edgePen.setWidth(2);

    buttonErase = new QPushButton("Erase", this);

    pointTable = new TableView(this);
    pointTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pointTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pointTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    outerLayout = new QVBoxLayout();
    mainLayout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    canvasLayout = new QGridLayout();
    guideLayout = new QGridLayout();
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
    actionZoomIn = new QAction("Zoom In", menuEdit);
    actionZoomOut = new QAction("Zoom Out", menuEdit);
    actionZoomIn->setIcon(QIcon(":/zoomin.icns"));
    actionZoomOut->setIcon(QIcon(":/zoomout.icns"));
    menuEdit->addAction(actionZoomIn);
    menuEdit->addAction(actionZoomOut);
    toolbar->addAction(actionZoomIn);
    toolbar->addAction(actionZoomOut);


    outerLayout->addWidget(toolbar);
    outerLayout->addLayout(mainLayout);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    leftLayout->addLayout(canvasLayout);
    leftLayout->addLayout(leftBottomLayout);
    rightLayout->addWidget(isShowInputPoints);
    rightLayout->addWidget(isShowDelaunayEdges);
    rightLayout->addWidget(isShowMSTreeEdges);
    rightLayout->addWidget(isShowVoronoiEdges);
    rightLayout->addWidget(pointTable);
    rightLayout->addWidget(buttonErase);
    canvasLayout->addWidget(canvas, 0, 0, 2, 2);
    canvasLayout->addLayout(guideLayout, 1, 1, 1, 1);
    guideLayout->addWidget(guide);
    guideLayout->setContentsMargins(0, 0, 20, 20);
    leftBottomLayout->addWidget(label);
    leftBottomLayout->addWidget(command);

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
    QObject::connect(actionAdd, SIGNAL(triggered()), this, SLOT(addPointDialog()));
    QObject::connect(actionZoomIn, SIGNAL(triggered()), this->canvas, SLOT(zoomIn()));
    QObject::connect(actionZoomOut, SIGNAL(triggered()), this->canvas, SLOT(zoomOut()));

    QObject::connect(isShowInputPoints, SIGNAL(stateChanged(int)), this, SLOT(switchInputPoints()));
    QObject::connect(isShowDelaunayEdges, SIGNAL(stateChanged(int)), this, SLOT(switchDelaunayEdges()));
    QObject::connect(isShowMSTreeEdges, SIGNAL(stateChanged(int)), this, SLOT(switchMSTreeEdges()));
    QObject::connect(isShowVoronoiEdges, SIGNAL(stateChanged(int)), this, SLOT(switchVoronoiEdges()));

    QObject::connect(this, SIGNAL(datasetChange()), this, SLOT(updateItem()));

    QObject::connect(canvas->getPainter(), SIGNAL(addPoint(double, double)), this, SLOT(addPoint(double, double)));
    QObject::connect(this, SIGNAL(drawInputPoints(Spantree::PointIds)), this, SLOT(drawInput(Spantree::PointIds)));
    QObject::connect(this, SIGNAL(drawDelaunayEdges(Spantree::Graph)), this, SLOT(drawDelaunay(Spantree::Graph)));
    QObject::connect(this, SIGNAL(drawMSTreeEdges(Spantree::Graph)), this, SLOT(drawMSTree(Spantree::Graph)));
    QObject::connect(this, SIGNAL(drawVoronoiEdges(Spantree::Graph)), this, SLOT(drawVoronoi(Spantree::Graph)));
    QObject::connect(buttonErase, SIGNAL(clicked()), this, SLOT(erase()));

    QObject::connect(pointTable, SIGNAL(changehighlight(std::vector<int>)), this, SLOT(highlight(std::vector<int>)));

}

View2D::~View2D() {
    solver.stop = 1;
    algorithmThread.join();
}

void View2D::addPoint(double x, double y) {
    solver.stop = 1;
    algorithmThread.join();
    solver.stop = 0;
    addPointThread(x, y);
}

void View2D::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Load", QDir::homePath(), "Text files (*.txt)");
    solver.stop = 1;
    algorithmThread.join();
    solver.stop = 0;
    loadFileThread(filename.toStdString());
}

void View2D::updateItem() {
    solver.stop = 1;
    algorithmThread.join();
    solver.stop = 0;

    algorithmThread = boost::thread(boost::bind(&View2D::updateGraphItem, this));

    pointTable->setModel(0);
    pointTableModel->clear();
    pointTableModel->setHorizontalHeaderItem(0, new QStandardItem("Index"));
    pointTableModel->setHorizontalHeaderItem(1, new QStandardItem("X"));
    pointTableModel->setHorizontalHeaderItem(2, new QStandardItem("Y"));
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
    inputPoints = canvas->getPainter()->drawPoints(pts, pointPen);
    if (isShowInputPoints->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(inputPoints);
    isShowInputPoints->setEnabled(1);
}

void View2D::drawDelaunay(Spantree::Graph graph) {
    delaunayEdges = canvas->getPainter()->drawEdges(graph, edgePen);
    if (isShowDelaunayEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(delaunayEdges);
    isShowDelaunayEdges->setEnabled(1);
}

void View2D::drawVoronoi(Spantree::Graph graph) {
    voronoiEdges = canvas->getPainter()->drawEdges(graph, edgePen);
    if (isShowVoronoiEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(voronoiEdges);
    isShowVoronoiEdges->setEnabled(1);
}

void View2D::drawMSTree(Spantree::Graph graph) {
    mSTreeEdges = canvas->getPainter()->drawEdges(graph, edgePen);
    if (isShowMSTreeEdges->checkState() == Qt::Checked)
        canvas->getPainter()->showItems(mSTreeEdges);
    isShowMSTreeEdges->setEnabled(1);
}

void View2D::addPointDialog() {
    auto point = AddPointDialog::getPoint(this);
    if (point.first)
        emit addPoint(point.second.first, point.second.second);
}

void View2D::erase() {
    std::set<int> points;

    for (auto itm : pointTable->selectedIndexes()) {
        if (itm.column() == 0) {
            points.insert(itm.data().toInt());
        }
    }

    solver.stop = 1;
    algorithmThread.join();
    solver.stop = 0;

    erasePointThread(points);
}

void View2D::highlight(std::vector<int> pts) {
    delete highlightPoint;
    highlightPoint = 0;
    QPen pen(Qt::red);
    pen.setWidth(8);
    Spantree::PointIds points;
    int cnt = 0;
    for (auto i : pts) {
        points[cnt++] = solver.getPoint(i);
    }
    highlightPoint = canvas->getPainter()->drawPoints(points, pen);
    canvas->getPainter()->addItem(highlightPoint);
}

AddPointDialog::AddPointDialog(QWidget *parent) : QDialog(parent){
    buttonN = new QPushButton("No", this);
    buttonY = new QPushButton("Yes", this);
    xpos = new QDoubleSpinBox(this);
    ypos = new QDoubleSpinBox(this);
    xlabel = new QLabel("X: ", this);
    ylabel = new QLabel("Y: ", this);

    mainLayout = new QHBoxLayout();
    vLayout = new QVBoxLayout();
    vLayout->addWidget(xlabel);
    vLayout->addWidget(xpos);
    mainLayout->addLayout(vLayout);
    vLayout = new QVBoxLayout();
    vLayout->addWidget(ylabel);
    vLayout->addWidget(ypos);
    mainLayout->addLayout(vLayout);
    vLayout = new QVBoxLayout();
    vLayout->addWidget(buttonY);
    vLayout->addWidget(buttonN);
    mainLayout->addLayout(vLayout);

    xpos->setRange(-1e10, 1e10);
    xpos->setFixedWidth(100);
    ypos->setRange(-1e10, 1e10);
    ypos->setFixedWidth(100);


    this->setLayout(mainLayout);
    QObject::connect(buttonY, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(buttonN, SIGNAL(clicked()), this, SLOT(reject()));
}

std::pair< bool, std::pair<double, double> > AddPointDialog::getPoint(QWidget *parent) {
    AddPointDialog *dialog = new AddPointDialog(parent);
    if (dialog->exec() == QDialog::Accepted) {
        return std::make_pair(1, std::make_pair(dialog->xpos->value(), dialog->ypos->value()));
    } else {
        return std::make_pair(0, std::make_pair(0, 0));
    }
}


void TableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QTableView::selectionChanged(selected, deselected);
    std::vector<int> pts;
    for (auto index : selectedIndexes()) {
        if (index.column() == 0) {
            pts.push_back(index.data().toInt());
        }
    }
    emit changehighlight(pts);
}

TableView::TableView(QWidget *parent) : QTableView(parent){

}

QModelIndexList TableView::selectedIndexes() const {
    return QTableView::selectedIndexes();
}
