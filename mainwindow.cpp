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
    array = new QList<Chunk>();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete array;
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
        /*
         * Выбираем файл
         */
        QFile file(fileName);

        /*
         * Пытаемся открыть файл
         */
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            /*
             * Читаем первую строчку
             */
            QString line = file.readLine();

            /*
             * Пролистываем до надписи [DATA]
             */
            while (line != "[DATA]\n") {
                line = file.readLine();
            }

            /*
             * Читаем остальные данные до конца файла
             */
            while(!line.isEmpty())
            {
                /*
                 * Создаем чанк
                 */
                Chunk chunk;

                /*
                 * Заполняем чанк значениями
                 */
                for(int number = 0; number < Chunk::SIZE; number++)
                {
                    /*
                     * Читаем новую строчку данных
                     */
                    line = file.readLine();

                    /*
                     * Если данные есть (чанк не прервался раньше)
                     */
                    if(!line.isEmpty())
                    {
                        /*
                         * Вытаскиваем данные в виде строк
                         */
                        QStringList list = line.split("\t");

                        /*
                         * Для каждого канала
                         */
                        for(int cannel = 0; cannel < list.size(); cannel++)
                        {
                            /*
                             * Вытаскиваем само число в виде строки
                             */
                            QString str = list.at(cannel);

                            /*
                             * Записываем в чанк число, заменив запятую на точку
                             */
                            chunk.data[number][cannel] = str.replace(",", ".").toFloat();
                        }
                    } else {
                        /*
                         * Чанк прервался раньше - выходим из цикла
                         */
                        break;
                    }
                }
                /*
                 * Добавляем чанк в массив
                 */
                array->push_back(chunk);

                /*
                 * Флушим файл
                 */
                file.flush();
            }

            /*
             * Закрываем файл
             */
            file.close();
        }
    }
}
