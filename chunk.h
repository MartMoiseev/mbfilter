#ifndef CHUNK_H
#define CHUNK_H

/**
 * @brief Класс чанка - содержит кусок данных
 */
class Chunk
{
public:
    Chunk();

public:
    /**
     * @brief SIZE Размер чанка 20000
     */
    static const int SIZE = 20000;

    /**
     * @brief CHANNELS Количество каналов
     */
    static const int CHANNELS = 16;

public:
    /**
     * @brief Set
     * @param _value
     * @param _number
     * @param _canal
     */
    void    Set(float _value, int _number, int _canal);

    /**
     * @brief Get
     * @param _number
     * @param _canal
     * @return
     */
    float   Get(int _number, int _canal) const;

public:
    /**
     * @brief data Данные
     */
    float   data[SIZE][CHANNELS];
};

#endif // CHUNK_H
