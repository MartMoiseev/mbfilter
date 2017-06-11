#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <canal.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

class ChartView : public QGraphicsView
{
    Q_OBJECT
public:
            ChartView(QWidget *parent = 0);
    void    setCanal(Canal* canal);
    void    setZoom(qreal xZoom, qreal yZoom);
    void    renderData(bool saveScroll = false);
    void    filter(int gate);
    void    cut(int gate, int cutout = 10);

    void    resizeEvent(QResizeEvent *event);

private:
    Canal*              _canal;
    QGraphicsScene*     _scene;
    QGraphicsPathItem*  _chart;
    QGraphicsPathItem*  _filter;

    qreal               _xZoom = 1.0;
    qreal               _yZoom = 1.0;
};

#endif // CHARTVIEW_H
