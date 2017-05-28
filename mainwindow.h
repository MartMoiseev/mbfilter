#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <chunk.h>
#include <chart.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void array();

private slots:
    void on_actionOpen_triggered();
    void on_pushButton_clicked();

    void on_xZoom_valueChanged(double arg1);

    void on_yZoom_valueChanged(double arg1);

    void on_widthZoom_valueChanged(double arg1);

private:
    Ui::MainWindow      *ui;
};

#endif // MAINWINDOW_H
