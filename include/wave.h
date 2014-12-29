#include <iostream>
#include <cstdint>
#include <cmath>

typedef uint16_t sample_t;

#define SAMPLE_MAX 32767
#define SAMPLE_MIN -32767

class Wave
{
    public:
    
    char ChunkID[4];
    uint32_t ChunkSize;
    char Format[4];
    char Subchunk1ID[4];
    uint32_t Subchunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    char Subchunk2ID[4];
    uint32_t Subchunk2Size;
    sample_t *Data;
    uint32_t NumSamples;
    
    Wave(uint16_t, uint32_t, uint16_t, uint32_t);
    Wave(char *path);
    int save(char *path);
    ~Wave();
    
    double time();
};