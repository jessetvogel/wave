#include "wave.h"

// Constructor for new wave file
Wave::Wave(uint16_t _NumChannels, uint32_t _SampleRate, uint16_t _BitsPerSample, uint32_t _NumSamples)
{
    // Constant values
    strcpy(ChunkID, "RIFF");
    strcpy(Format, "WAVE");
    strcpy(Subchunk1ID, "fmt ");
    strcpy(Subchunk2ID, "data");
    AudioFormat = 1; // PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.
    
    // Variable values given by parameters
    NumChannels = _NumChannels;
    SampleRate = _SampleRate;
    BitsPerSample = _BitsPerSample;
    NumSamples = _NumSamples;
    
    // Values that are dependend on other values
    ByteRate = SampleRate * NumChannels * BitsPerSample / 8;
    BlockAlign = NumChannels * BitsPerSample / 8;
    
    // Chunk sizes
    Subchunk1Size = 16; // PCM
    Subchunk2Size = NumSamples * NumChannels * BitsPerSample / 8;
    ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size);
    
    // Allocate space for data
    Data = (sample_t*) malloc(Subchunk2Size * sizeof(sample_t));
}

// Constructor for loading wave file
Wave::Wave(char *path)
{
    // Open file for binary reading
    FILE *file;
    file = fopen(path, "rb");
    
    // Read header
    fread(ChunkID, sizeof(char), sizeof(ChunkID), file);
    fread((char*) &ChunkSize, sizeof(char), sizeof(ChunkSize), file);
    fread(Format, sizeof(char), sizeof(Format), file);
    fread(Subchunk1ID, sizeof(char), sizeof(Subchunk1ID), file);
    fread((char*) &Subchunk1Size, sizeof(char), sizeof(Subchunk1Size), file);
    fread((char*) &AudioFormat, sizeof(char), sizeof(AudioFormat), file);
    fread((char*) &NumChannels, sizeof(char), sizeof(NumChannels), file);
    fread((char*) &SampleRate, sizeof(char), sizeof(SampleRate), file);
    fread((char*) &ByteRate, sizeof(char), sizeof(ByteRate), file);
    fread((char*) &BlockAlign, sizeof(char), sizeof(BlockAlign), file);
    fread((char*) &BitsPerSample, sizeof(char), sizeof(BitsPerSample), file);
    fread(Subchunk2ID, sizeof(char), sizeof(Subchunk2ID), file);
    fread((char*) &Subchunk2Size, sizeof(char), sizeof(Subchunk2Size), file);
    
    // Allocate space for data and read samples
    Data = (sample_t*) malloc(Subchunk2Size * sizeof(sample_t));
    fread((char*) Data, sizeof(char), Subchunk2Size, file);
    
    // Calculate NumSamples
    NumSamples = Subchunk2Size / (NumChannels * BitsPerSample / 8);
    
    // Close file
    fclose(file);
}

int Wave::save(char *path)
{
    // Open file for binary writing
    FILE *file;
    file = fopen(path, "wb");
    
    // Write header
    fwrite(ChunkID, sizeof(char), sizeof(ChunkID), file);
    fwrite((char*) &ChunkSize, sizeof(char), sizeof(ChunkSize), file);
    fwrite(Format, sizeof(char), sizeof(Format), file);
    fwrite(Subchunk1ID, sizeof(char), sizeof(Subchunk1ID), file);
    fwrite((char*) &Subchunk1Size, sizeof(char), sizeof(Subchunk1Size), file);
    fwrite((char*) &AudioFormat, sizeof(char), sizeof(AudioFormat), file);
    fwrite((char*) &NumChannels, sizeof(char), sizeof(NumChannels), file);
    fwrite((char*) &SampleRate, sizeof(char), sizeof(SampleRate), file);
    fwrite((char*) &ByteRate, sizeof(char), sizeof(ByteRate), file);
    fwrite((char*) &BlockAlign, sizeof(char), sizeof(BlockAlign), file);
    fwrite((char*) &BitsPerSample, sizeof(char), sizeof(BitsPerSample), file);
    fwrite(Subchunk2ID, sizeof(char), sizeof(Subchunk2ID), file);
    fwrite((char*) &Subchunk2Size, sizeof(char), sizeof(Subchunk2Size), file);
    
    // Write samples
    fwrite((char*) Data, sizeof(char), Subchunk2Size, file);
    
    // Close file
    fclose(file);
    
    return 1;
}

// Destructor
Wave::~Wave()
{
    // Free the space used for the data
    free(Data);
}

double Wave::time() { return (double) NumSamples / SampleRate; }