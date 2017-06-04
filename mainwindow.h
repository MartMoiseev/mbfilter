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

    void on_checkBox_1_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);
    void on_checkBox_5_clicked(bool checked);
    void on_checkBox_6_clicked(bool checked);
    void on_checkBox_7_clicked(bool checked);
    void on_checkBox_8_clicked(bool checked);
    void on_checkBox_9_clicked(bool checked);
    void on_checkBox_10_clicked(bool checked);
    void on_checkBox_11_clicked(bool checked);
    void on_checkBox_12_clicked(bool checked);
    void on_checkBox_13_clicked(bool checked);
    void on_checkBox_14_clicked(bool checked);
    void on_checkBox_15_clicked(bool checked);
    void on_checkBox_16_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_spinBox_valueChanged(int arg1);


private:
    Ui::MainWindow      *ui;
};

#endif // MAINWINDOW_H
