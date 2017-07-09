#include "chartview.h"

/**
 * @brief ChartView::setCanal
 * @param canal
 */
void ChartView::setCanal(Canal* original)
{
    // Устанавливаем ссылку на канал
    this->_original = original;
    this->_canal = original->clone();

    // Перерисовываем графики
    this->rerender();
}



/**
 * Перерисовываем графики
 *
 * @brief ChartView::rerender
 */
void ChartView::rerender()
{
    Canal* canal = this->_canal;
    if (this->_bypass) {
        canal = this->_original;
    }

    this->clearGraphs();
    this->_graph = this->addGraph();
    this->_graph->setData(canal->getNumbers(), canal->getData());
    this->replot();
}



/**
 * @brief ChartView::preview
 */
void ChartView::preview()
{
    // Очищаем все графики (если есть)
    for (int g = 1; g < this->graphCount(); g++) {
        this->removeGraph(this->graph(g));
    }

    // Ищем новые места для фильтрации
    for(long i = 1; i < this->_canal->length() - this->_cutout; i++) {
        float last = this->_canal->get(i - 1);
        float current = this->_canal->get(i);

        if (abs(current - last) > _gate) {
            QVector<double> values;
            QVector<double> positions;
            for (long pos = i; pos < i + this->_cutout; pos++) {
                values.push_back(this->_canal->get(pos));
                positions.push_back(pos - 1);
            }
            QCPGraph* graph = this->addGraph();
            graph->setData(positions, values);
            graph->setPen(QPen(Qt::red));
            i += this->_cutout;
        }
    }

    // Перерисовываем графики
    this->replot();
}



/**
 * @brief ChartView::filter
 */
void ChartView::filter()
{
    // Клонируем весь канал, для отмены
    this->_history.push_back(this->_canal->clone());

    // Включаем кнопку отмены
    emit undoDisabled(false);

    // Выбираем последний канал как текущий
    this->_canal = this->_history.last();

    // Обрабатываем все данные
    for(long i = 1; i < this->_canal->length() - _cutout; i++) {

        // Вытаскиваем текущее и предыдущее значения
        float last = this->_canal->get(i - 1);
        float current = this->_canal->get(i);

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
    _bypass = false;
    emit changeBypass(_bypass);

    // Перерисовываем график
    this->rerender();
}



/**
 * @brief ChartView::filterMiddle
 * @param position
 */
void ChartView::filterMiddle(long position) {
    // Используем среднее
    qreal middle = 0.0;
    long start = position;
    long end = position + _cutout;

    for (long i = start; i <= end; i++) {
        middle += this->_canal->get(i) / (_cutout * 2.0);
    }

    for (long pos = start; pos < end; pos++) {
        this->_canal->set(pos, middle);
    }
}



/**
 * @brief ChartView::filterChain
 * @param position
 */
void ChartView::filterChain(long position) {
    // Используем среднее
    qreal middle = 0.0;
    long start = position;
    long end = position + _cutout;

    for (long i = start; i <= end; i++) {
        middle += this->_canal->get(i) / (_cutout * 2.0);
    }

    for (long pos = start; pos < end; pos++) {
        this->_canal->set(pos, middle);
    }
}



/**
 * @brief ChartView::filterZero
 * @param position
 */
void ChartView::filterZero(long position) {
    long start = position;
    long end = position + _cutout;

    for (long pos = start; pos < end; pos++) {
        this->_canal->set(pos, 0.0);
    }
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
void ChartView::setBypass(bool bypass)
{
    _bypass = bypass;
    this->rerender();
}



/**
 * @brief ChartView::undo
 */
void ChartView::undo()
{
    if (_history.length() > 1) {
        this->_history.pop_back();
        this->_canal = _history.last();
        this->rerender();
    } else {
        if (_history.length() == 1) {
            this->_canal = _original;
            this->rerender();
        }
    }

    // Выключаем кнопку, если отменять нечего
    if (this->_canal == _original) {
        emit undoDisabled(true);
    } else {
        emit undoDisabled(false);
    }
}
