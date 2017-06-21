#ifndef DATA_H
#define DATA_H

#include "canal.h"

class Data
{
public:
    void    load(QString fileName);
    void    save(QString fileName);
    Canal*  get(int canal);
    int     getCount() const;

private:
    void    loadMegawin(QFile &file);

private:
    QList<Canal*>   _canals;
    QString         _head;
};

#endif // DATA_H
