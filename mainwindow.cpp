#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog.h"
#include "QFile.h"
#include <QByteArray>
#include "QDebug.h"

#include "data.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _data = new Data();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete _data;
    delete _ui;
}

/**
 * @brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered()
{
    // Открываем диалоговое окно
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Открыть"),
                0,
                tr("Все (*.asc *.txt);;ASC-файлы (*.asc);;Текстовые файлы (*.txt)")
    );

    // Продолжаем, если файл был выбран
    if(!fileName.isEmpty())
    {
        // Загружаем файл
        this->_data->load(fileName);

        // Передаем графику указатель на канал
        _ui->chart01->setCanal(this->_data->get(0));

        // Отрисовываем график
        _ui->chart01->renderData();
    }
}

/**
 * @brief MainWindow::on_gate01_valueChanged
 * @param arg1
 */
void MainWindow::on_gate01_valueChanged(int arg1)
{
    _ui->chart01->filter(arg1);
}

/**
 * @brief MainWindow::on_filter01_clicked
 */
void MainWindow::on_filter01_clicked()
{
    _ui->chart01->filter(_ui->gate01->value());
}

/**
 * @brief MainWindow::on_cut01_clicked
 */
void MainWindow::on_cut01_clicked()
{
    _ui->chart01->cut(_ui->gate01->value(), _ui->cutout->value());
}

/**
 * @brief MainWindow::on_doubleSpinBox_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    _ui->chart01->setZoom(arg1, 0);
}

/**
 * @brief MainWindow::on_doubleSpinBox_2_valueChanged
 * @param arg1
 */
void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    _ui->chart01->setZoom(0, arg1);
}
