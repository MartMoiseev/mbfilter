#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <canal.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QRubberBand>
#include "spacer.h"

class ChartView : public QGraphicsView
{
    Q_OBJECT
public:
            ChartView(QWidget *parent = 0);
    void    setCanal(Canal* canal, Canal* canalNew);
    void    setZoom(qreal xZoom, qreal yZoom);
    void    renderData();

private:
    void    filterMiddle(long position);
    void    filterChain(long position);
    void    filterZero(long position);

public slots:
    void    preview();
    void    filter();
    void    zoomx(int x);
    void    zoomy(int y);
    void    setGate(int gate);
    void    setCutout(int cutout);
    void    setPreview(bool);
    void    setSpacer(int spacer);
    void    undo();

signals:
    void    changeBypass(bool);

private:
    Canal*              _original = nullptr;
    Canal*              _canalNew = nullptr;
    QList<Canal*>       _history;
    QGraphicsScene*     _scene = nullptr;
    QGraphicsPathItem*  _chart = nullptr;
    QGraphicsPathItem*  _filter = nullptr;
    QRubberBand*        _rubberBand = nullptr;

    qreal               _xZoom = 1.0;
    qreal               _yZoom = 1.0;
    QPoint              _origin;
    int                 _gate = 50;
    int                 _cutout = 10;
    bool                _preview = false;
    int                 _spacer = Spacer::MIDDLE;
};

#endif // CHARTVIEW_H
