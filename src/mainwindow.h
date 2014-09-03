#ifndef __mainwindow_h__
#define __mainwindow_h__

#include "spantree.h"
#include <QMainWindow>
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
    QGridLayout *mainLayout;

    QGraphicsView *canvas;
    QGraphicsScene *scene;
    QLabel *label;
    QLineEdit *command;
    QPushButton *buttonLoad;

    spantree solver;

private slots:
    void slotLoad();
};

#endif 
