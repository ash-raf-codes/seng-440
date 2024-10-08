#ifndef READ_WAV_H
#define READ_WAV_H


#include <stdint.h> 
#include <stdbool.h> 
#include <stdio.h>
#include <string.h>
#include <time.h>

// Macro to convert bytes to uint16 


#define BITS_PER_SAMPLE 8

#define bytes_to_int16(byte_one, byte_two) byte_one | byte_two << 8



// #define BITS_PER_SAMPLE (8)

#define COMPRESS (1)
#define DECOMPRESS (0)


// wave file header struct

typedef struct wav_header {
  // riff header -------
    unsigned char riff[4];                        // 'R I F F'
    unsigned char overall_size[4];            
    unsigned char format[4];                        // ' W A V E'

    // ------- subchunk1 ---------
    unsigned char subChunk1ID[4];            //  'F M T'
    unsigned char subChunk1Size[4];               // length of the format data
    unsigned char audioFormat[2];                 // PCM = 1, 7 mu law, 6 = a law  
    unsigned char numChannels[2];                // 1 = audio, 2 = stereo 
    unsigned char sampleRate[4];                 // sampling rate (blocks per second)
    unsigned char byteRate[4];                   // SampleRate * NumChannels * BitsPerSample/8
    unsigned char blockAlign[2];                 // NumChannels * BitsPerSample/8 
    unsigned char bitsPerSample[2];             // bits per sample, 8- 8bits, 16- 16 bits etc

    // --- sub chink 2 (data) ------ 
    unsigned char subChunk2ID[4];           // 'D A T A'
    unsigned char subChunk2Size[4];                   // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} wave_header_t;



// // wave file pointer 
// typedef struct wave_file
// {
//     wave_header_t header;
//     uint32_t *data;
// }wave_file_t; 

// functions
int encode_wave_file(char* inFileName, char* outFileName, bool encodeType);



#endif 