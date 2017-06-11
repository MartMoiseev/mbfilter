#include "chart.h"
#include <chunk.h>

#include <QScrollBar>
#include <QDebug>

/**
 * @brief Chart::Chart
 * @param parent
 */
Chart::Chart(Canal* canal, QWidget *parent)
    // Вызываем конструктор родителя
    : QGraphicsView(parent)
{
    _path = new QGraphicsPathItem();
    QPen tiny(Qt::blue);
    _path->setPen(tiny);

    _scene = new QGraphicsScene();
    setScene(_scene);
    _scene->addItem(_path);
    _scene->addItem(&chart);
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

    QPainterPath pPath;

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

    //настройка сцены
    _scene->setSceneRect(0, height / 2.0, absLenth / zx, height * (countCanals + 1));

    this->ensureVisible(_scene->sceneRect());
    this->fitInView(_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->minimum());    
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
