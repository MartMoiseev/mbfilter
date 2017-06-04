#include "chart.h"
#include <chunk.h>

#include <QScrollBar>
#include <QDebug>

/**
 * @brief Chart::Chart
 * @param parent
 */
Chart::Chart(QWidget *parent)
    // Вызываем конструктор родителя
    : QGraphicsView(parent)
{
    _path = new QGraphicsPathItem();
    QPen tiny(Qt::blue);
    _path->setPen(tiny);

    _scene = new QGraphicsScene();
    setScene(_scene);
    _scene->addItem(_path);

    for(int c = 0; c < Chunk::CHANNELS; c++)
    {
        text[c] = new QGraphicsTextItem();
        text[c]->setDefaultTextColor(Qt::blue);
        text[c]->setPlainText("#" + QString::number(c + 1));
        text[c]->setFont(QFont("Arial", 10));
        text[c]->setZValue(100);

        _scene->addItem(text[c]);

        _scene->addItem(&chart[c]);
        visible[c] = true;
    }
}

/**
 * @brief Chart::Test
 */
void Chart::Test()
{
    //qreal width = this->width() * 1.2;
    //zx = absLenth / width;

    qreal padding = 20.0;
    qreal height = 400.0 / zy + padding;

    int countCanals = 0;

    for (int i = 0; i < lines.length(); i++) {
        delete lines[i];
    }
    lines.clear();

    for(int c = 0; c < Chunk::CHANNELS; c++)
    {
        if (visible[c]) {
            countCanals++;

            qreal yZero = countCanals * height + padding;

            text[c]->setOpacity(1);
            text[c]->setPos(4, yZero - 12);

            for(long i = 0; i < absLenth - 1; i++) {
                qreal current = Get(c, i);
                qreal next = Get(c, i + 1);
                qreal absolute = abs(next - current);

                if (absolute > delta) {
                    QGraphicsRectItem *line = new QGraphicsRectItem(i / zx, yZero - 50, 1, 100);
                    line->setPen(QPen(QColor(0, 255, 0, 255)));
                    lines.push_back(line);
                    _scene->addItem(line);
                    i++;
                }
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
    this->zx = xZoom;
    this->Render();
}

/**
 * @brief Chart::setYZoom
 * @param yZoom
 */
void Chart::setYZoom(qreal yZoom)
{
    this->zy = yZoom;
    this->Render();
}

/**
 * @brief Chart::setWidthZoom
 * @param widthZoom
 */
void Chart::setWidthZoom(qreal widthZoom)
{
    this->zd = widthZoom;
    QPen tiny(Qt::blue);
    tiny.setWidthF(this->zd);
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

        // Проверяем что за файл
        if (line == "MegaWin ASCII file\n") {
            // Если файл экспорта MegaWin
            this->loadMegaWin(file);
        }

        /*
         * Закрываем файл
         */
        file.close();

        /*****DEBUG*****/ qDebug() << "Всего загружено " << array.length() << " чанков";
    }
}

/**
 * @brief Chart::loadMegaWin
 * @param file
 */
void Chart::loadMegaWin(QFile &file)
{
    QString line = file.readLine();

    /*
     * Пролистываем до надписи [DATA]
     */
    while (line != "[DATA]\n") {
        line = file.readLine();
    }

    line = file.readLine();

    data.clear();
    dataNative.clear();
    absLenth = 0;
    unsigned long lenth = 0;

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
            lenth++;
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
                absLenth = lenth;
                break;
            }
        }
        /*
         * Добавляем чанк в массив
         */
        array.push_back(chunk);
        data.push_back(chunk);
        dataNative.push_back(chunk);

        /*
         * Флушим файл
         */
        file.flush();
    }

    if(absLenth > this->size().width()) {
        zx = absLenth / this->size().width();
    } else {
        zx = 0.7;
    }
}

/**
 * @brief Chart::Render
 */
void Chart::Render()
{
    qreal width = this->width() * 1.2;
    zx = absLenth / width;

    qreal padding = 20.0;
    qreal height = 400.0 / zy + padding;

    int countCanals = 0;

    for(int c = 0; c < Chunk::CHANNELS; c++)
    {
        QPainterPath pPath;

        if (visible[c]) {
            countCanals++;

            qreal yZero = countCanals * height + padding;

            pPath.moveTo(0, yZero);
            _scene->addLine(0, yZero, absLenth / zx, yZero, QPen(Qt::blue));
            _scene->addRect(0, yZero - 10, 50, 20, QPen(Qt::blue), QBrush(Qt::white));

            text[c]->setOpacity(1);
            text[c]->setPos(4, yZero - 12);

            /*
            int STEP = absLenth / width;
            if(STEP % 2 != 0) {
                STEP++;
            }

            for(int i = 0; i < absLenth; i+=STEP) {
                // Локальные максимум и минимум
                double localMax = 0.0;
                double localMin = 0.0;

                // Координаты максимума и минимума
                long posMax = 0;
                long posMin = 0;

                // Двигаемся по участку
                for(int a = 0; a < STEP; a++) {
                    // Устанавливаем текущие значения как минимум и максимум
                    localMax = Get(c, i);
                    localMin = Get(c, i);

                    // Если следующее число больше максимума
                    if(localMax < Get(c, i + a)) {
                        localMax = Get(c, i + a);
                        posMax = a;
                    }

                    // Если следующее число меньше минимума
                    if(localMin > Get(c, i + a)) {
                        localMin = Get(c, i + a);
                        posMin = a;
                    }
                }

                // Определяем порядок отрисовки максимума и минимума
                if(posMax > posMin) {
                    // Если минимум встретился раньше
                    pPath.lineTo(i / zx, -localMin + yZero);
                    pPath.lineTo(i / zx, -localMax + yZero);
                } else {
                    // Если максимум встретился раньше
                    pPath.lineTo(i / zx, -localMax + yZero);
                    pPath.lineTo(i / zx, -localMin + yZero);
                }
            }*/

            for(int i = 0; i < absLenth; i++) {
                qreal value = Get(c, i);
                qreal GATE = 1000;
                if (abs(value) > GATE) {
                    value = value > 0 ? GATE : -GATE;
                }
                pPath.lineTo(i / zx, (-value / zy) + yZero);
            }

            chart[c].setPath(pPath);
        } else {
            text[c]->setOpacity(0);
        }
    }

    //настройка сцены
    _scene->setSceneRect(0, height / 2.0, absLenth / zx, height * (countCanals + 1));

    this->ensureVisible(_scene->sceneRect());
    this->fitInView(_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->minimum());

    /*
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
    */
}

void Chart::setCanal(int canal, bool enable)
{
    if (canal > 0 && canal <= Chunk::CHANNELS) {
        visible[canal - 1] = enable;
        Render();
    }
}

/**
 * @brief Chart::Fill
 */
void Chart::Fill()
{
    if(absLenth > this->size().width()) {
        zx = absLenth / this->size().width();
    } else {
        zx = 0.7;
    }
}

/**
 * @brief Chart::resizeEvent
 * @param event
 */
void Chart::resizeEvent(QResizeEvent *event)
{
    Render();
}

/**
 * @brief MyoChart::Get
 * @param _canal
 * @param _position
 * @return
 */
float Chart::Get(int _canal, long _position) const
{
    if(_canal >= 0 && _canal < Chunk::CHANNELS && _position >= 0 && _position < absLenth) {
        return data.at(int(_position / Chunk::SIZE)).Get(int(_position % Chunk::SIZE), _canal);
    } else {
        return -1;
    }
}

/**
 * @brief MyoChart::GetNative
 * @param _canal
 * @param _position
 * @return
 */
float Chart::GetNative(int _canal, long _position) const
{
    if(_canal >= 0 && _canal < Chunk::CHANNELS && _position >= 0 && _position < absLenth) {
        return dataNative.at(int(_position / Chunk::SIZE)).Get(int(_position % Chunk::SIZE), _canal);
    } else {
        return -1;
    }
}
