#ifndef DATA_H
#define DATA_H

#include "canal.h"

class Data
{
public:
    void    load(QString fileName);
    bool    save(QString fileName);
    Canal*  get(int canal);
    Canal*  getNew(int canal);
    int     getCount() const;

private:
    void    loadMegawin(QFile &file);

private:
    QList<Canal*>   _canals;
    QList<Canal*>   _canalsNew;
    QString         _head;
};

#endif // DATA_H
