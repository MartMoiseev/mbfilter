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
void ChartView::setCanal(Canal *canal, Canal *canalNew)
{
    // Устанавливаем ссылку на канал
    this->_original = canal;
    this->_canalNew = canalNew;

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
void ChartView::renderData()
{
    int scroll = this->horizontalScrollBar()->value();

    // Создаем график
    QPainterPath path;

    Canal* canal = nullptr;
    if (_preview) {
        canal = _canalNew;
    } else {
        canal = _original;
    }

    long length = canal->length();

    // Рисуем горизонтальную синюю линию
    _scene->addLine(0, 0, length / _xZoom, 0, QPen(Qt::blue));

    for(long i = 0; i < length; i++) {
        float value = canal->get(i);
        path.lineTo(i / _xZoom, -value / _yZoom);
    }

    this->_chart->setPath(path);

    //настройка сцены
    this->_scene->setSceneRect(0, -110.0, length / _xZoom, 0);

    //this->ensureVisible(_scene->sceneRect());
    //this->fitInView(_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
    this->horizontalScrollBar()->setValue(scroll);
}

/**
 * @brief ChartView::resizeEvent
 * @param event
 */
void ChartView::resizeEvent(QResizeEvent *event)
{
    if (this->_canalNew) {
        this->renderData();
    }
}

/**
 * @brief ChartView::preview
 */
void ChartView::preview()
{
    QPainterPath path;

    for(long i = 1; i < this->_canalNew->length(); i++) {
        float last = this->_canalNew->get(i - 1);
        float current = this->_canalNew->get(i);

        if (abs(current - last) > _gate) {
            path.moveTo(i / _xZoom, -300 / _yZoom);
            path.lineTo(i / _xZoom, 300 / _yZoom);
        }
    }

    this->_filter->setPath(path);
    this->_filter->setPen(QPen(QBrush(QColor(255, 0, 0, 16)), 5.0));
}

/**
 * @brief ChartView::filter
 */
void ChartView::filter()
{
    // Клонируем весь канал, для отмены
    this->_history.push_back(this->_canalNew->clone());

    // Выбираем последний канал как текущий
    this->_canalNew = this->_history.last();

    // Обрабатываем все данные
    for(long i = _cutout; i < this->_canalNew->length() - _cutout; i++) {

        // Вытаскиваем текущее и предыдущее значения
        float last = this->_canalNew->get(i - 1);
        float current = this->_canalNew->get(i);

        // Определяем пик
        if (abs(current - last) > _gate) {
            switch(this->_spacer) {

            default:
            case Spacer::CHAIN:
                this->filterChain(i);
                break;

            case Spacer::MIDDLE:
                this->filterMiddle(i);
                break;
            }

            // Пропускаем отфильтрованный участок
            i += _cutout;
        }
    }
    _preview = true;
    emit changeBypass(_preview);

    // Перерисовываем график
    this->renderData();
}

/**
 * @brief ChartView::filterMiddle
 * @param position
 */
void ChartView::filterMiddle(long position) {
    // Используем среднее
    qreal middle = 0.0;
    long start = position - _cutout;
    long end = position + _cutout;

    for (long i = start; i <= end; i++) {
        middle += this->_canalNew->get(i) / (_cutout * 2.0);
    }

    for (long pos = start; pos < end; pos++) {
        this->_canalNew->set(pos, middle);
    }
}

/**
 * @brief ChartView::filterChain
 * @param position
 */
void ChartView::filterChain(long position) {
    // Используем среднее
    qreal middle = 0.0;
    long start = position - _cutout;
    long end = position + _cutout;

    for (long i = start; i <= end; i++) {
        middle += this->_canalNew->get(i) / (_cutout * 2.0);
    }

    for (long pos = start; pos < end; pos++) {
        this->_canalNew->set(pos, middle);
    }
}

/**
 * @brief ChartView::filterZero
 * @param position
 */
void ChartView::filterZero(long position) {
    long start = position - _cutout;
    long end = position + _cutout;

    for (long pos = start; pos < end; pos++) {
        this->_canalNew->set(pos, 0.0);
    }
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

/**
 * @brief ChartView::setSpacer
 * @param spacer
 */
void ChartView::setSpacer(int spacer) {
    this->_spacer = spacer;
}

/**
 * @brief ChartView::setPreview
 * @param preview
 */
void ChartView::setPreview(bool preview)
{
    _preview = preview;
    renderData();
}

/**
 * @brief ChartView::undo
 */
void ChartView::undo()
{
    if (_history.length() > 1) {
        this->_history.pop_back();
        this->_canalNew = _history.last();
        this->renderData();
    } else {
        if (_history.length() == 1) {
            this->_canalNew = _original;
            this->renderData();
        }
    }
}

/**
 * @brief ChartView::mousePressEvent
 * @param event
 */
void ChartView::mousePressEvent(QMouseEvent *event)
{
    _origin = event->pos();
    if (_rubberBand == nullptr) {
        _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    }
    _rubberBand->setGeometry(QRect(_origin, QSize()));
    _rubberBand->show();
}

/**
 * @brief ChartView::mouseMoveEvent
 * @param event
 */
void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    _rubberBand->setGeometry(QRect(_origin, event->pos()).normalized());
}

/**
 * @brief ChartView::mouseReleaseEvent
 * @param event
 */
void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    _rubberBand->hide();
    QPoint pos = _rubberBand->pos();
    QRect rect = _rubberBand->rect();

    int scrollLeft = this->horizontalScrollBar()->value();

    QPainterPath path;
    path.moveTo(pos.x() * _xZoom + scrollLeft * _xZoom, -300 / _yZoom);
    path.lineTo(pos.x() * _xZoom + scrollLeft * _xZoom, 300 / _yZoom);

    path.moveTo(pos.x() * _xZoom + rect.right() * _xZoom + scrollLeft * _xZoom, -300 / _yZoom);
    path.lineTo(pos.x() * _xZoom + rect.right() * _xZoom + scrollLeft * _xZoom, 300 / _yZoom);

    this->_filter->setPath(path);
    this->_filter->setPen(QPen(QBrush(QColor(0, 255, 0, 192)), 10.0));
}
