#ifndef DATA_H
#define DATA_H

#include "canal.h"

class Data
{
public:
    void    load(QString fileName);
    Canal*  get(int canal);

private:
    void    loadMegawin(QFile &file);

private:
    QList<Canal*>   _canals;
};

#endif // DATA_H
