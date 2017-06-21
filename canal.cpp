#include "canal.h"

/**
 * @brief Canal::add
 * @param value
 */
void Canal::add(float value)
{
    if (this->_length == Chunk::SIZE) {
        // Сбрасываем счетчик до нуля
        this->_length = 0;
    }

    if (this->_length == 0) {
        // Увеличиваем текущий счетчик
        this->_current++;

        // Добавляем еще один чанк в массив
        Chunk* newChunk = new Chunk();
        this->_chunks.push_back(newChunk);
    }

    // Добавляем в чанк значение
    this->_chunks[this->_current]->set(value, this->_length);

    // Увеличиваем количество элементов
    this->_length++;
}

void Canal::set(long position, float value)
{
    // Если есть запись с таким номером
    if (position < this->length()) {
        // Возвращаем значение
        int c = position / Chunk::SIZE;
        int p = position % Chunk::SIZE;
        this->_chunks[c]->set(value, p);
    }
}

/**
 * @brief Canal::get
 * @param position
 * @return
 */
float Canal::get(long position) const
{
    // Если есть запись с таким номером
    if (position < this->length()) {
        // Возвращаем значение
        int c = position / Chunk::SIZE;
        int p = position % Chunk::SIZE;
        return this->_chunks.at(c)->get(p);
    } else {
        // В противном случае возвращаем 0
        return 0.0;
    }
}

/**
 * @brief Canal::length
 * @return
 */
long Canal::length() const
{    
    // Возвращаем количество элементов
    return Chunk::SIZE * this->_current + this->_length;
}

/**
 * @brief Canal::getName
 * @return
 */
QString Canal::getName() const
{
    return this->_name;
}

/**
 * @brief Canal::setName
 * @param name
 */
void Canal::setName(QString name)
{
    this->_name = name;
}
