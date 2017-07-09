#ifndef CANAL_H
#define CANAL_H

#include <QVector>
#include "chunk.h"

/**
 * Класс для хранения и работы с данными отдельного канала
 * А также вся история фильтрации этого канала
 *
 * @brief The Canal class
 */
class Canal
{
public:

    Canal();

    /**
     * Добавление данных в канал, автоматически разбивает на чанки
     *
     * @brief add
     * @param value
     */
    void    add(float value);



    /**
     * Устанавливает значение для конкретной записи
     *
     * @brief set
     * @param position
     * @param value
     */
    void    set(long position, float value);



    /**
     * Возвращает данные
     *
     * @brief get
     * @param position
     * @return
     */
    float   get(long position) const;


    /**
     * Возвращает количество записей в канале
     *
     * @brief length
     * @return
     */
    long    length() const;


    /**
     * Возвращает имя канала
     *
     * @brief getName
     * @return
     */
    QString getName() const;



    /**
     * Устанавливает имя канала
     *
     * @brief setName
     * @param name
     */
    void    setName(QString name);


    /**
     * Создает копию канала
     *
     * @brief clone
     * @return
     */
    Canal*  clone();

    /**
     * Возвращает массив данных
     *
     * @brief getData
     * @return
     */
    QVector<double>  getData();

    /**
     * Возвращает массив указателей на значения
     *
     * @brief getNumbers
     * @return
     */
    QVector<double>  getNumbers();

private:
    /**
     * Массив чанков данного канала
     *
     * @brief _chunks
     */
    QVector<Chunk*> _chunks;

    /**
     * Количество данных в канале
     *
     * @brief _length
     */
    long            _length = 0;

    /**
     * Название канала (для отображения в окне)
     *
     * @brief _name
     */
    QString         _name;
};

#endif // CANAL_H
