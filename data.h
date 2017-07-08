#ifndef DATA_H
#define DATA_H

#include <QFile>
#include "canal.h"

/**
 * Класс для хранения данных из файла
 *
 * @brief The Data class
 */
class Data
{
public:
    /**
     * Загрузка данных с файла
     *
     * @brief loadFromFile
     * @param fileName
     */
    void    loadFromFile(QString fileName);



    /**
     * Экспорт данных в файл
     *
     * @brief saveToFile
     * @param fileName
     * @return
     */
    bool    saveToFile(QString fileName);



    /**
     * Получение нужного канала
     *
     * @brief getCanal
     * @param canal
     * @return
     */
    Canal*  getCanal(int canal);



    /**
     * Количество загруженных каналов
     *
     * @brief countCanal
     * @return
     */
    int     countCanal() const;



private:
    /**
     * Загрузка данных из экспортного файла мегавин
     *
     * @brief loadMegawin
     * @param file
     */
    void    loadMegawin(QFile &file);



private:
    /**
     * Список каналов
     *
     * @brief _canals
     */
    QVector<Canal*> _canals;



    /**
     * Заголовок файла для экспорта
     *
     * @brief _head
     */
    QString         _head;
};

#endif // DATA_H
