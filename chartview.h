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

    void    resizeEvent(QResizeEvent *event);

public slots:
    void    preview();
    void    filter();
    void    zoomx(int x);
    void    zoomy(int y);
    void    setGate(int gate);
    void    setCutout(int cutout);

private:
    Canal*              _canal = nullptr;
    QGraphicsScene*     _scene = nullptr;
    QGraphicsPathItem*  _chart = nullptr;
    QGraphicsPathItem*  _filter = nullptr;

    qreal               _xZoom = 1.0;
    qreal               _yZoom = 1.0;
    int                 _gate = 50;
    int                 _cutout = 10;
};

#endif // CHARTVIEW_H
