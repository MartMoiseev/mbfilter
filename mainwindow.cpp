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
    ui->graphicsView->Test();
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
