#ifndef CHART_H
#define CHART_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPathItem>

#include <canal.h>

class Chart : public QGraphicsView
{
    Q_OBJECT
public:
            Chart(Canal* canal, QWidget *parent = 0);
    void    render();
    void    resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene*     _scene;
    QGraphicsPathItem*  _chart;
    Canal*              _canal;
};

#endif // CHART_H
