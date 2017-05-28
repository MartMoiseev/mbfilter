#ifndef CHART_H
#define CHART_H

#include <chunk.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>

class Chart : public QGraphicsView
{
    Q_OBJECT
public:
    Chart(QWidget *parent = 0);

public:
    void LoadFromFile(QString fileName);
    void Render();
    void Test();
    void setXZoom(qreal xZoom);
    void setYZoom(qreal yZoom);
    void setWidthZoom(qreal widthZoom);
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene      *_scene;
    QGraphicsPathItem   *_path;
    QList<Chunk>        array;

private:
    qreal               xZoom;
    qreal               yZoom;
    qreal               widthZoom;
};

#endif // CHART_H
