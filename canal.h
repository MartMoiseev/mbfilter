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

private:
    QList<Chunk*>   _chunks;
    long            _length = 0;
    int             _current = -1;
};

#endif // CANAL_H
