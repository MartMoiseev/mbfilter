#include "chunk.h"

/**
 * @brief MyoChunk::set
 * @param value
 * @param number
 */
void Chunk::set(float value, int number)
{
    if(number >= 0 && number < Chunk::SIZE) {
        this->_data[number] = value;
    }
}

/**
 * @brief MyoChunk::get
 * @param number
 * @return
 */
float Chunk::get(int number) const
{
    if(number >= 0 && number < Chunk::SIZE) {
         return _data[number];
    } else {
        return 0;
    }
}
