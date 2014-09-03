#ifndef __mainwindow_h__
#define __mainwindow_h__

#include "view2d.h"
#include "spantree.h"
#include "canvas.h"
#include <QGroupBox>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    View2D *view2D;
};

#endif 
