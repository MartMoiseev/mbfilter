#ifndef CANAL_H
#define CANAL_H

#include <QList>

#include "chunk.h"

class Canal
{
public:
    void    add(float value);
    void    set(long position, float value);
    float   get(long position) const;
    long    length() const;
    QString getName() const;
    void    setName(QString name);
    Canal*  clone();
    QVector<double>  getData();
    QVector<double>  getNumbers();

private:
    QList<Chunk*>   _chunks;
    long            _length = 0;
    int             _current = -1;
    QString         _name;
};

#endif // CANAL_H
