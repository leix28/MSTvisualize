#ifndef __mainwindow_h__
#define __mainwindow_h__

#include "view2d.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    View2D *view2D;
};

#endif 
