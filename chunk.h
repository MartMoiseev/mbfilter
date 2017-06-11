#ifndef CHUNK_H
#define CHUNK_H

/**
 * @brief Класс чанка - содержит кусок данных
 */
class Chunk
{
public:
    /**
     * @brief SIZE Размер чанка 20000
     */
    static const int SIZE = 20000;

public:
    /**
     * @brief set
     * @param value
     * @param number
     */
    void    set(float value, int number);

    /**
     * @brief get
     * @param number
     * @return
     */
    float   get(int number) const;

public:
    /**
     * @brief data Данные
     */
    float   _data[SIZE];
};

#endif // CHUNK_H
