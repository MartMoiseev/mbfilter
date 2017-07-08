#include "canal.h"

/**
 * Добавление первого чанка в массив
 *
 * @brief Canal
 */
Canal::Canal()
{
    Chunk* newChunk = new Chunk();
    this->_chunks.push_back(newChunk);
}

/**
 * Добавление данных в канал, автоматически разбивает на чанки
 *
 * @brief add
 * @param value
 */
void Canal::add(float value)
{
    // Увеличиваем счетчик
    this->_length++;

    // Вычисляем позицию в чанке
    int position = this->_length % Chunk::SIZE;

    // Если позиция больше размера чанка
    if (position == 0) {

        // Добавляем еще один чанк в массив
        Chunk* newChunk = new Chunk();
        this->_chunks.push_back(newChunk);
    }

    // Вычисляем чанк для работы
    int chunk = (int)this->_length / Chunk::SIZE;

    // Добавляем в чанк значение
    this->_chunks[chunk];//->set(value, position);
    this->_chunks[chunk]->set(value, position);
}



/**
 * Устанавливает значение для конкретной записи
 *
 * @brief set
 * @param position
 * @param value
 */
void Canal::set(long position, float value)
{
    // Если есть запись с таким номером
    if (position < this->_length) {

        int c = position / Chunk::SIZE;
        int p = position % Chunk::SIZE;

        // Устанавливаем значение
        this->_chunks[c]->set(value, p);
    }
}



/**
 * Возвращает данные
 *
 * @brief get
 * @param position
 * @return
 */
float Canal::get(long position) const
{
    // Если есть запись с таким номером
    if (position < this->_length) {

        int c = position / Chunk::SIZE;
        int p = position % Chunk::SIZE;

        // Возвращаем значение
        return this->_chunks.at(c)->get(p);
    } else {

        // В противном случае возвращаем 0
        return 0.0;
    }
}



/**
 * Возвращает количество записей в канале
 *
 * @brief length
 * @return
 */
long Canal::length() const
{    
    // Возвращаем количество элементов
    return this->_length;
}



/**
 * Возвращает имя канала
 *
 * @brief getName
 * @return
 */
QString Canal::getName() const
{
    return this->_name;
}



/**
 * Устанавливает имя канала
 *
 * @brief setName
 * @param name
 */
void Canal::setName(QString name)
{
    this->_name = name;
}



/**
 * Создает копию канала
 *
 * @brief clone
 * @return
 */
Canal* Canal::clone()
{
    Canal* canal = new Canal();
    canal->setName(this->getName());
    for (long i = 0; i < this->_length; i++) {
        canal->add(this->get(i));
    }
    return canal;
}



/**
 * @brief Canal::getData
 * @return
 */
QVector<double>  Canal::getData() {
    QVector<double> vector;
    for (long i = 0; i < this->length(); i++) {
        vector.push_back(this->get(i));
    }
    return vector;
}



/**
 * @brief Canal::getNumbers
 * @return
 */
QVector<double>   Canal::getNumbers() {
    QVector<double> vector;
    for (long i = 0; i < this->length(); i++) {
        vector.push_back(i);
    }
    return vector;
}
