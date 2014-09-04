#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);
    view2D = new View2D(this);
    this->setCentralWidget(view2D);
    this->setMenuBar(view2D->getMenuBar());
}


MainWindow::~MainWindow() {

}
