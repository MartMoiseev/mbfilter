#include "chartview.h"

#include <QScrollBar>

/**
 * @brief ChartView::ChartView
 * @param canal
 * @param parent
 */
ChartView::ChartView(QWidget *parent)
    // Вызываем конструктор родителя
    : QGraphicsView(parent)
{
    // Создаем новый путь для графика
    this->_chart = new QGraphicsPathItem();

    this->_filter = new QGraphicsPathItem();
    this->_filter->setPen(QPen(QColor(128, 0, 0, 16)));

    //QPen tiny(Qt::blue);
    //_chart->setPen(tiny);

    // Создаем новую сцену
    _scene = new QGraphicsScene();

    // Устанавливаем сцену как текущую
    this->setScene(this->_scene);

    // Добавляем на сцену наш график
    this->_scene->addItem(this->_chart);
    this->_scene->addItem(this->_filter);
}

/**
 * @brief ChartView::setCanal
 * @param canal
 */
void ChartView::setCanal(Canal *canal)
{
    // Устанавливаем ссылку на канал
    this->_canal = canal;

    delete this->_filter;
    this->_filter = new QGraphicsPathItem();
    this->_filter->setPen(QPen(QColor(128, 0, 0, 16)));
    this->_scene->addItem(this->_filter);
}

/**
 * @brief ChartView::setZoom
 * @param xZoom
 * @param yZoom
 */
void ChartView::setZoom(qreal xZoom, qreal yZoom)
{
    if (xZoom) {
        _xZoom = xZoom;
    }

    if (yZoom) {
        _yZoom = yZoom;
    }

    renderData();
}

/**
 * @brief ChartView::renderData
 * @param data
 */
void ChartView::renderData(bool saveScroll)
{
    int scroll = this->horizontalScrollBar()->value();

    // Создаем график
    QPainterPath path;

    long length = this->_canal->length();

    // Рисуем горизонтальную синюю линию
    _scene->addLine(0, 0, length / _xZoom, 0, QPen(Qt::blue));

    for(long i = 0; i < length; i++) {
        float value = this->_canal->get(i);
        path.lineTo(i / _xZoom, -value / _yZoom);
    }

    this->_chart->setPath(path);

    //настройка сцены
    this->_scene->setSceneRect(0, -350, length / _xZoom, 700);

    this->ensureVisible(_scene->sceneRect());
    this->fitInView(_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
    if (!saveScroll) {
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->minimum());
    } else {
        this->horizontalScrollBar()->setValue(scroll);
    }
}

/**
 * @brief ChartView::resizeEvent
 * @param event
 */
void ChartView::resizeEvent(QResizeEvent *event)
{
    if (this->_canal) {
        this->renderData();
    }
}

/**
 * @brief ChartView::filter
 */
void ChartView::preview()
{
    QPainterPath path;

    for(long i = 1; i < this->_canal->length(); i++) {
        float last = this->_canal->get(i - 1);
        float current = this->_canal->get(i);

        if (abs(current - last) > _gate) {
            path.moveTo(i / _xZoom, -300 / _yZoom);
            path.lineTo(i / _xZoom, 300 / _yZoom);
        }
    }

    this->_filter->setPath(path);
    this->_filter->setPen(QPen(QBrush(QColor(255, 0, 0, 16)), 5.0));
}

/**
 * @brief ChartView::cut
 * @param gate
 * @param cutout
 */
void ChartView::filter()
{
    // Обрабатываем все данные
    for(long i = _cutout; i < this->_canal->length() - _cutout; i++) {

        // Вытаскиваем текущее и соседние значения
        float last = this->_canal->get(i - 1);
        float current = this->_canal->get(i);
        float next = this->_canal->get(i + 1);

        // Определяем пик
        if (abs(current - last) > _gate) {

            // Если пик найден - фильтруем данные в заданном диапазоне
            for (int t = i - _cutout; t < i + _cutout; t++) {

                // Используем скользящее среднее - ищем среднее за n предыдущих
                qreal middle = 0.0;
                for (long j = t - _cutout; j <= t; j++) {
                    middle += this->_canal->get(j) / _cutout;
                }

                // Присваиваем новое значение
                this->_canal->set(t, middle);
            }

            // Пропускаем отфильтрованный участок
            i += _cutout;
        }
    }
    // Перерисовываем график
    this->renderData(true);
}

/**
 * @brief ChartView::zoomx
 * @param x
 */
void ChartView::zoomx(int x)
{
    this->_xZoom = x;
    this->renderData();
}

/**
 * @brief ChartView::zoomy
 * @param y
 */
void ChartView::zoomy(int y)
{
    this->_yZoom = y / 10.0;
    this->renderData();
}

/**
 * @brief ChartView::setGate
 * @param gate
 */
void ChartView::setGate(int gate)
{
    this->_gate = gate;
}

/**
 * @brief ChartView::setCutout
 * @param cutout
 */
void ChartView::setCutout(int cutout)
{
    this->_cutout = cutout;
}
