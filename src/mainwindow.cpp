#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(1280, 800);
    view2D = new View2D(this);
    this->setCentralWidget(view2D);
}


MainWindow::~MainWindow() {

}
