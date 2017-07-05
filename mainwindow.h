#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QSlider>
#include <QSplitter>

#include "chartview.h"
#include "data.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void addLayout(Canal* canal, Canal* canalNew);

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

private:
    Ui::MainWindow*     _ui = nullptr;
    Data*               _data = nullptr;
    QSplitter*          _splitter = nullptr;
};

#endif // MAINWINDOW_H
