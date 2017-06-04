#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog.h"
#include "QFile.h"
#include <QByteArray>
#include "QDebug.h"

#include "chunk.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered()
{
    /*
     * Открываем диалоговое окно
     */
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Открыть"),
                0,
                tr("Все (*.asc *.txt);;ASC-файлы (*.asc);;Текстовые файлы (*.txt)")
    );

    /*
     * Продолжаем, если файл был выбран
     */
    if(!fileName.isEmpty())
    {
        ui->graphicsView->LoadFromFile(fileName);
        ui->graphicsView->Render();
    }
}

/**
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->Fill();
    ui->graphicsView->Render();
}

/**
 * @brief MainWindow::on_xZoom_valueChanged
 * @param arg1
 */
void MainWindow::on_xZoom_valueChanged(double arg1)
{
    ui->graphicsView->setXZoom(arg1);
}

/**
 * @brief MainWindow::on_yZoom_valueChanged
 * @param arg1
 */
void MainWindow::on_yZoom_valueChanged(double arg1)
{
    ui->graphicsView->setYZoom(arg1);
}

/**
 * @brief MainWindow::on_widthZoom_valueChanged
 * @param arg1
 */
void MainWindow::on_widthZoom_valueChanged(double arg1)
{
    ui->graphicsView->setWidthZoom(arg1);
}

void MainWindow::on_checkBox_1_clicked(bool checked)
{
    ui->graphicsView->setCanal(1, checked);
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    ui->graphicsView->setCanal(2, checked);
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    ui->graphicsView->setCanal(3, checked);
}

void MainWindow::on_checkBox_4_clicked(bool checked)
{
    ui->graphicsView->setCanal(4, checked);
}

void MainWindow::on_checkBox_5_clicked(bool checked)
{
    ui->graphicsView->setCanal(5, checked);
}

void MainWindow::on_checkBox_6_clicked(bool checked)
{
    ui->graphicsView->setCanal(6, checked);
}

void MainWindow::on_checkBox_7_clicked(bool checked)
{
    ui->graphicsView->setCanal(7, checked);
}

void MainWindow::on_checkBox_8_clicked(bool checked)
{
    ui->graphicsView->setCanal(8, checked);
}

void MainWindow::on_checkBox_9_clicked(bool checked)
{
    ui->graphicsView->setCanal(9, checked);
}

void MainWindow::on_checkBox_10_clicked(bool checked)
{
    ui->graphicsView->setCanal(10, checked);
}

void MainWindow::on_checkBox_11_clicked(bool checked)
{
    ui->graphicsView->setCanal(11, checked);
}

void MainWindow::on_checkBox_12_clicked(bool checked)
{
    ui->graphicsView->setCanal(12, checked);
}

void MainWindow::on_checkBox_13_clicked(bool checked)
{
    ui->graphicsView->setCanal(13, checked);
}

void MainWindow::on_checkBox_14_clicked(bool checked)
{
    ui->graphicsView->setCanal(14, checked);
}

void MainWindow::on_checkBox_15_clicked(bool checked)
{
    ui->graphicsView->setCanal(15, checked);
}

void MainWindow::on_checkBox_16_clicked(bool checked)
{
    ui->graphicsView->setCanal(16, checked);
}

/**
 * @brief MainWindow::on_pushButton_2_clicked
 */
void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->Test();
}

/**
 * @brief MainWindow::on_spinBox_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->graphicsView->delta = arg1;
    ui->graphicsView->Test();
}
