#include "mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setCentralWidget(new QWidget(this));

    mainLayout = new QGridLayout(this->centralWidget());
    canvas = new QGraphicsView(this->centralWidget());
    scene = new QGraphicsScene;

    canvas->setScene(scene);
    label = new QLabel("Command: ", this->centralWidget());
    command = new QLineEdit(this->centralWidget());
    buttonLoad = new QPushButton("Load", this->centralWidget());

    mainLayout->addWidget(canvas, 0, 0, 1, 3);
    mainLayout->addWidget(label, 1, 0);
    mainLayout->addWidget(command, 1, 1);
    mainLayout->addWidget(buttonLoad, 1, 2);

    this->centralWidget()->setLayout(mainLayout);

    QObject::connect(buttonLoad, SIGNAL(clicked()), this, SLOT(slotLoad()));
}

void MainWindow::slotLoad() {
    QString filename = QFileDialog::getOpenFileName(this, "Load", QDir::homePath(), "Text files (*.txt)");
    std::ifstream reader(filename.toStdString());
    double x, y;
    std::vector< std::pair<double, double> > points;
    while (reader >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }
    solver.insert(points);
    auto ret = solver.get_tree();
    for (auto edge : ret) {

        scene->addLine(edge.first.first, edge.first.second,
                       edge.second.first, edge.second.second);

        std::cout << edge.first.first << " " << edge.first.second << " " <<
                     edge.second.first << " " << edge.second.second << std::endl;
    }
}

MainWindow::~MainWindow() {

}
