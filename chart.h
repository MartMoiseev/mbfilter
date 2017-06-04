#ifndef CHART_H
#define CHART_H

#include <chunk.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QFile>

class Chart : public QGraphicsView
{
    Q_OBJECT
public:
    Chart(QWidget *parent = 0);

public:
    void LoadFromFile(QString fileName);
    void Render();
    void setCanal(int canal, bool enable);
    void Fill();
    void Test();
    void setXZoom(qreal xZoom);
    void setYZoom(qreal yZoom);
    void setWidthZoom(qreal widthZoom);
    void resizeEvent(QResizeEvent *event);

public:
    int                 delta = 40;
    int                 count = 2;

private:
    void    loadMegaWin(QFile &file);
    float   Get(int _canal, long _position) const;
    float   GetNative(int _canal, long _position) const;

private:
    QGraphicsScene      *_scene;
    QGraphicsPathItem   *_path;
    QGraphicsTextItem   *text[Chunk::CHANNELS];
    QList<Chunk>        array;
    QList<QGraphicsItem*>    lines;

    QGraphicsPathItem   chart[Chunk::CHANNELS];
    QList<Chunk>        data;
    QList<Chunk>        dataNative;

private:
    long                absLenth = 0;
    float               zx = 5;
    float               zy = 2;
    float               zd = 90;
    bool                visible[Chunk::CHANNELS];
};

#endif // CHART_H
