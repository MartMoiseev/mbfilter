#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <canal.h>

#include "qcustomplot.h"
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QRubberBand>
#include "spacer.h"

class ChartView : public QCustomPlot
{
    Q_OBJECT
public:
            ChartView(int);
    void    setCanal(Canal* original);
    void    rerender();

private:
    void    filterMiddle(long position);
    void    filterChain(long position);
    void    filterZero(long position);

public slots:
    void    preview();
    void    filter();

    void    setGate(int gate);
    void    setCutout(int cutout);
    void    setBypass(bool);
    void    setSpacer(int spacer);

    void    undo();

signals:
    void    changeBypass(bool);
    void    undoDisabled(bool);
    void    changeChanal(int, Canal*);

private:
    Canal*              _original = nullptr;
    Canal*              _canal = nullptr;
    QList<Canal*>       _history;
    QCPGraph*           _graph;

    int                 _id;
    int                 _gate = 50;
    int                 _cutout = 10;
    bool                _bypass = true;
    int                 _spacer = Spacer::MIDDLE;
};

#endif // CHARTVIEW_H
