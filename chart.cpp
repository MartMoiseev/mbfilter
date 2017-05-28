#include "chart.h"
#include <QFile>
#include <QDebug>
#include <chunk.h>

/**
 * @brief Chart::Chart
 * @param parent
 */
Chart::Chart(QWidget *parent)
    // Вызываем конструктор родителя
    : QGraphicsView(parent)
{
    xZoom = 1.0;
    yZoom = 0.5;
    widthZoom = 1.0;


    _path = new QGraphicsPathItem();
    QPen tiny(Qt::blue);
    tiny.setWidthF(widthZoom);
    _path->setPen(tiny);


    _scene = new QGraphicsScene();
    setScene(_scene);
    _scene->addItem(_path);
}

/**
 * @brief Chart::Test
 */
void Chart::Test()
{
    /*****DEBUG*****/ qDebug() << "тест";

    const int DELTA = 900;

    for(int chank = 0; chank < array.length(); chank++) {
        for (int position = 0; position < Chunk::SIZE - 1; position++) {
            qreal current = array.at(chank).data[0][position];
            qreal next = array.at(chank).data[0][position + 1];

            qreal x = ((chank * Chunk::SIZE) + position - DELTA) * xZoom;
            qreal y = -1000.0 * yZoom;

            if (abs(current - next) > DELTA) {
                _scene->addRect(x, y, DELTA * xZoom, 2000.0 * yZoom, QPen(Qt::green), QBrush(Qt::red));
                position += DELTA;
            }
        }
    }
}

/**
 * @brief Chart::setXZoom
 * @param xZoom
 */
void Chart::setXZoom(qreal xZoom)
{
    this->xZoom = xZoom;
    this->Render();
}

/**
 * @brief Chart::setYZoom
 * @param yZoom
 */
void Chart::setYZoom(qreal yZoom)
{
    this->yZoom = yZoom;
    this->Render();
}

/**
 * @brief Chart::setWidthZoom
 * @param widthZoom
 */
void Chart::setWidthZoom(qreal widthZoom)
{
    this->widthZoom = widthZoom;
    QPen tiny(Qt::blue);
    tiny.setWidthF(this->widthZoom);
    _path->setPen(tiny);
    this->Render();
}

/**
 * @brief Chart::LoadFromFile
 * @param fileName
 */
void Chart::LoadFromFile(QString fileName)
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
        while (true) {

            if (line.isEmpty()) return;

            bool isData = line == "[DATA]\n";
            bool isValues = line.left(6) == "Values";

            if (isData || isValues) {
                break;
            }

            line = file.readLine();
        }

        line = file.readLine();

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
                    if (list.length() == 1) {
                        list = line.split(" ");
                    }

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
                        chunk.data[number][cannel] = str.replace(",", ".").toFloat() * 1000000.0;
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
            array.push_back(chunk);

            /*
             * Флушим файл
             */
            file.flush();
        }

        /*
         * Закрываем файл
         */
        file.close();

        /*****DEBUG*****/ qDebug() << "Всего загружено " << array.length() << " чанков";
    }
}

/**
 * @brief Chart::Render
 */
void Chart::Render()
{
    xZoom = (size().width() * 5.0) / qreal((array.length() * Chunk::SIZE));
    yZoom = size().height() / 20000.0;

    // Создаем путь для отрисовки - график
    QPainterPath path;

    // Прогоняем по всем чанкам в массиве
    for(int chank = 0; chank < array.length(); chank++) {

        // Прогоняем по всем данным внутри чанка
        for (int position = 0; position < Chunk::SIZE; position++) {

            qreal x = ((chank * Chunk::SIZE) + position) * xZoom;
            qreal y = (array.at(chank).data[0][position]) * yZoom;

            // Отрисовываем линию по координате и данным
            path.lineTo(x, y);
        }
    }

    // Добавляем путь на QGraphicsView
    _path->setPath(path);

    this->ensureVisible(_scene->sceneRect());
    this->fitInView(_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

/**
 * @brief Chart::resizeEvent
 * @param event
 */
void Chart::resizeEvent(QResizeEvent *event)
{
    Render();
}
