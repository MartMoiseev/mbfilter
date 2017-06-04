#include "chunk.h"

/**
 * @brief Chunk::Chunk
 */
Chunk::Chunk()
{

}

/**
 * @brief MyoChunk::Set
 * @param _value
 * @param _number
 * @param _canal
 */
void Chunk::Set(float _value, int _number, int _canal)
{
    if(_number >= 0 && _number < Chunk::SIZE && _canal >= 0 && _canal < Chunk::CHANNELS)
        data[_number][_canal] = _value;
}

/**
 * @brief MyoChunk::Get
 * @param _number
 * @param _canal
 * @return
 */
float Chunk::Get(int _number, int _canal) const
{
    if(_number >= 0 && _number < Chunk::SIZE && _canal >= 0 && _canal < Chunk::CHANNELS)
         return data[_number][_canal];
    else return -1;
}
