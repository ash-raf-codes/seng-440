/* 
    All Wave File Notation was referenced here: https://web.archive.org/web/20130510081513/https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "comp_decomp.h"
#include "read_wave_file.h"

//Convers decimal to bin

// function to convert decimal to binary
void decToBinary(int n)
{
    // array to store binary number
    int binaryNum[32];

    // counter for binary array
    int i = 0;
    while (n > 0)
    {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
        printf("%d", binaryNum[j]);
}

int codeword_compression(int sample_magnitude, int sign)
{
    if(sample_magnitude >= 8192)
    {
        sample_magnitude = (sample_magnitude - 8192);
    }

    int chord, step;
    int codeword_tmp;

    //debug_print("\n<============== Checking Compression operation ==============>");

    //debug_print("\nSign before flip: %d", sign);
    sign = !sign; //Flip Sign

    //debug_print("\nSample Megnatude: %d | ", sample_magnitude);
    if (DEBUG)
        decToBinary(sample_magnitude);
    //debug_print("\nSample Sign: %d | ", sign);
    if (DEBUG)
        decToBinary(sign);
    //debug_print("\n");

    if (sample_magnitude > 16383) //check if input is within upper bound
    {
        printf("\n!!!!! INPUT IS TOO LARGE !!!!!\n");
        return 0;
    }

    if (sample_magnitude >= 4096)
    {
        chord = 0x7;
        step = (sample_magnitude >> 8) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 2048)
    {
        chord = 0x6;
        step = (sample_magnitude >> 7) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 1024)
    {
        chord = 0x5;
        step = (sample_magnitude >> 6) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 512)
    {
        chord = 0x4;
        step = (sample_magnitude >> 5) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("\nchord: %d, step: %d, codeword_tmp: %d |", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 256)
    {
        chord = 0x3;
        step = (sample_magnitude >> 4) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("\nchord: %d, step: %d, codeword_tmp: %d |", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 128)
    {
        chord = 0x2;
        step = (sample_magnitude >> 3) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 64)
    {
        chord = 0x1;
        step = (sample_magnitude >> 2) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }
    if (sample_magnitude >= 32)
    {
        chord = 0x0;
        step = (sample_magnitude >> 1) & 0xF;
        codeword_tmp = (sign << 7) ^ (chord << 4) ^ step;
        //debug_print("chord: %d, step: %d, codeword_tmp: %d | ", chord, step, codeword_tmp);
        return (codeword_tmp);
    }

    //debug_print("COULD NOT MEET IF STATMENT CONDITION");
    return 0; //Error
}

int codeword_decompression(int codeWord)
{
    //debug_print("\n<============== Checking Decompression operation ==============>");

    int sign = ((codeWord << 6) & 0x2000);
    sign = !sign;

    int sign_Most_Sig_Bit = ((sign << 13) & 0x2000); //shift sign to correct position and mask
    int step = (codeWord & 0x0F);                    //Step
    int chord = ((codeWord >> 4) & 0x07);            //chord
    int step_shifted_by_chord = step << (chord + 1);
    int add_ones_on_either_side = (0x21 << chord); // add the 1 A B C D 1 33 dec, 100001 bin, 21 hex
    int magnitude = add_ones_on_either_side | step_shifted_by_chord;
    int finalVal = sign_Most_Sig_Bit | magnitude;

    // if (DEBUG)
    // {

    //     //debug_print("\n Sign before flip: %d | ", !sign);
    //     decToBinary(!sign);

    //     //debug_print("\n Sign after flip: %d | ", sign);
    //     decToBinary(sign);

    //     //int sign_Most_Sig_Bit = ((sign << 13) & 0x2000);        //shift sign to correct position and mask

    //     //debug_print("\n Sign in 14th place: %d | ", sign_Most_Sig_Bit);
    //     decToBinary(sign_Most_Sig_Bit);

    //     //int step = (codeWord & 0x0F);   //Step

    //     //debug_print("\n Step: %d | ", step);
    //     decToBinary(step);

    //     // int chord = ((codeWord >> 4) & 0x07);  //chord
    //     //debug_print("\n Chord: %d | ", chord);
    //     decToBinary(chord);

    //     //int step_shifted_by_chord = step << (chord+1);
    //     //debug_print("\n step_shifted_by_chord: %d | ", step_shifted_by_chord);
    //     decToBinary(step_shifted_by_chord);

    //     //int add_ones_on_either_side = (0x21 << chord);        // add the 1 A B C D 1 33 dec, 100001 bin, 21 hex
    //     //debug_print("\n add_ones_on_either_side: %d | ", add_ones_on_either_side);
    //     decToBinary(add_ones_on_either_side);

    //     //int megnatude = add_ones_on_either_side | step_shifted_by_chord;
    //     //debug_print("\n megnatude: %d | ", megnatude);
    //     decToBinary(megnatude);

    //     //int finalVal = sign_Most_Sig_Bit | megnatude;
    //     //debug_print("\n Decompressed Work: %d | ", finalVal);
    //     decToBinary(finalVal);
    // }
    return finalVal;
}

int signum(int sample)
{
    if (sample > 8192)
    {
        //debug_print("\nNegative\n");
        return (1); /* sign is ’0’ for negative samples */
    }
    else
    {
        //debug_print("\nPositive\n");
        return (0); /* sign is ’1’ for positive samples */
    }
}

int magnitude(int sample)
{
    if (sample < 0)
    {
        sample = -sample;
    }
    //debug_print("Magnatude: %d", sample);
    return (sample);
}

int Test(int sample)
{
    int Compressed_Word = codeword_compression(magnitude(sample), signum(sample));
    int Decompressed_Word = codeword_decompression(Compressed_Word);

    printf("\n<Testing..");
    printf("\nInput: %d | ", sample);
    decToBinary(sample);
    printf("\nCompressed: %d | ", Compressed_Word);
    decToBinary(Compressed_Word);
    printf("\nDecompressed: %d | ", Decompressed_Word);
    decToBinary(Decompressed_Word);

    return 0;
}

// int encode_wave_file(char *input_file_name, char *output_file_name, bool encodeType)
// {

//     FILE *input_file, *output_file;
//     wave_header_t wav_header;

//     unsigned char byte_buffer_2[2], byte_buffer_4[4];
//     unsigned char *inputfile_data_buffer, *output_file_data_buffer;

//     int16_t input_data1, input_data2, input_data3, input_data4;
//     int8_t codeword1, codeword2, codeword3, codeword4;

//     input_file = fopen(input_file_name, "rb+");
//     output_file = fopen(output_file_name, "wb");

//     // Riff - does not change
//     fread(wav_header.riff, sizeof(wav_header.riff), 1, input_file);
//     fwrite(wav_header.riff, sizeof(wav_header.riff), 1, output_file);

//     // Overall Size (ChunkSize) - the new size must be calculated and written
//     fread(wav_header.overall_size, sizeof(wav_header.overall_size), 1, input_file);
//     unsigned int overall_size = wav_header.overall_size[0] | (wav_header.overall_size[1] << 8) | (wav_header.overall_size[2] << 16) | (wav_header.overall_size[3] << 24);
//     unsigned int new_overall_size = overall_size + 8 - 44;

//     new_overall_size = new_overall_size / 2;
//     new_overall_size = new_overall_size + 44 - 8;
//     byte_buffer_4[0] = (new_overall_size >> 24) & 0xFF;
//     byte_buffer_4[1] = (new_overall_size >> 16) & 0xFF;
//     byte_buffer_4[2] = (new_overall_size >> 8) & 0xFF;
//     byte_buffer_4[3] = new_overall_size & 0xFF;
//     fwrite(&byte_buffer_4[3], 1, 1, output_file);
//     fwrite(&byte_buffer_4[2], 1, 1, output_file);
//     fwrite(&byte_buffer_4[1], 1, 1, output_file);
//     fwrite(&byte_buffer_4[0], 1, 1, output_file);

//     // Wave(Format) - does not change between I/O
//     fread(wav_header.format, sizeof(wav_header.format), 1, input_file);
//     fwrite(wav_header.format, sizeof(wav_header.format), 1, output_file);

//     // Wave (Subchunk1ID) - does not change between I/O
//     fread(wav_header.subChunk1ID, sizeof(wav_header.subChunk1ID), 1, input_file);
//     fwrite(wav_header.subChunk1ID, sizeof(wav_header.subChunk1ID), 1, output_file);

//     // Wave (Subchunk1Size) - does not change between I/O
//     fread(wav_header.subChunk1Size, sizeof(wav_header.subChunk1Size), 1, input_file);
//     fwrite(wav_header.subChunk1Size, sizeof(wav_header.subChunk1Size), 1, output_file);

//     // Wave (Audio Format) :  1=PCM; 6=ALAW; 7=MuLaw
//     fread(wav_header.audioFormat, sizeof(wav_header.audioFormat), 1, input_file);
//     byte_buffer_2[0] = 7;
//     byte_buffer_2[1] = '\0';
//     fwrite(&byte_buffer_2[0], 1, 1, output_file);
//     fwrite(&byte_buffer_2[1], 1, 1, output_file);

//     // Number of Channels - record the value but do not change it
//     fread(wav_header.numChannels, sizeof(wav_header.numChannels), 1, input_file);
//     unsigned int numChannels = wav_header.numChannels[0] | (wav_header.numChannels[1] << 8);
//     fwrite(wav_header.numChannels, sizeof(wav_header.numChannels), 1, output_file);

//     // Sample Rate - Record the value but do not change it
//     fread(wav_header.sampleRate, sizeof(wav_header.sampleRate), 1, input_file);
//     unsigned int sampleRate = wav_header.sampleRate[0] | (wav_header.sampleRate[1] << 8) | (wav_header.sampleRate[2] << 16) | (wav_header.sampleRate[3] << 24);
//     fwrite(wav_header.sampleRate, sizeof(wav_header.sampleRate), 1, output_file);

//     // Byte Rate = (SampleRate * NumChannels * BitsPerSample) / 8
//     fread(wav_header.byteRate, sizeof(wav_header.byteRate), 1, input_file);
//     unsigned int byteRate = (sampleRate * numChannels * BITS_PER_SAMPLE) / 8;
//     byte_buffer_4[0] = (byteRate >> 24) & 0xFF;
//     byte_buffer_4[1] = (byteRate >> 16) & 0xFF;
//     byte_buffer_4[2] = (byteRate >> 8) & 0xFF;
//     byte_buffer_4[3] = byteRate & 0xFF;
//     fwrite(&byte_buffer_4[3], 1, 1, output_file);
//     fwrite(&byte_buffer_4[2], 1, 1, output_file);
//     fwrite(&byte_buffer_4[1], 1, 1, output_file);
//     fwrite(&byte_buffer_4[0], 1, 1, output_file);

//     // Wave (Block Align)  = (NumChannels * BitsPerSample) / 8
//     fread(wav_header.blockAlign, sizeof(wav_header.blockAlign), 1, input_file);
//     unsigned int blockAlign = (numChannels * BITS_PER_SAMPLE) / 8;
//     byte_buffer_2[0] = (blockAlign >> 8) & 0xFF;
//     byte_buffer_2[1] = blockAlign & 0xFF;
//     fwrite(&byte_buffer_2[1], 1, 1, output_file);
//     fwrite(&byte_buffer_2[0], 1, 1, output_file);

//     // Bits per sample = 8
//     fread(wav_header.bitsPerSample, sizeof(wav_header.bitsPerSample), 1, input_file);
//     byte_buffer_2[0] = BITS_PER_SAMPLE;
//     byte_buffer_2[1] = '\0';
//     fwrite(&byte_buffer_2[0], 1, 1, output_file);
//     fwrite(&byte_buffer_2[1], 1, 1, output_file);

//     // Wave (subChunk2ID) - does not change between I/O
//     fread(wav_header.subChunk2ID, sizeof(wav_header.subChunk2ID), 1, input_file);
//     fwrite(wav_header.subChunk2ID, sizeof(wav_header.subChunk2ID), 1, output_file);

//     // Wave (Data size) - needs to be calculated
//     fread(wav_header.subChunk2Size, sizeof(wav_header.subChunk2Size), 1, input_file);
//     int chunk_header = new_overall_size + 8 - 44;
//     byte_buffer_4[0] = (chunk_header >> 24) & 0xFF;
//     byte_buffer_4[1] = (chunk_header >> 16) & 0xFF;
//     byte_buffer_4[2] = (chunk_header >> 8) & 0xFF;
//     byte_buffer_4[3] = chunk_header & 0xFF;
//     fwrite(&byte_buffer_4[3], 1, 1, output_file);
//     fwrite(&byte_buffer_4[2], 1, 1, output_file);
//     fwrite(&byte_buffer_4[1], 1, 1, output_file);
//     fwrite(&byte_buffer_4[0], 1, 1, output_file);

//     overall_size -= 36;

//     inputfile_data_buffer = malloc(overall_size * sizeof(char));

//     fread(inputfile_data_buffer, overall_size, 1, input_file);

//     register int i;

//     if (encodeType == COMPRESS)
//     {

//         output_file_data_buffer = malloc((overall_size / 2) * sizeof(char));

//         for (i = 0; i < overall_size; i = i + 8)
//         {

//             input_data1 = bytes_to_int16(inputfile_data_buffer[i], inputfile_data_buffer[i + 1]);
//             input_data2 = bytes_to_int16(inputfile_data_buffer[i + 2], inputfile_data_buffer[i + 3]);
//             input_data3 = bytes_to_int16(inputfile_data_buffer[i + 4], inputfile_data_buffer[i + 5]);
//             input_data4 = bytes_to_int16(inputfile_data_buffer[i + 6], inputfile_data_buffer[i + 7]);

//             codeword1 = codeword_compression(input_data1, signum(input_data1));
//             codeword2 = codeword_compression(input_data2, signum(input_data2));
//             codeword3 = codeword_compression(input_data3, signum(input_data3));
//             codeword4 = codeword_compression(input_data4, signum(input_data4));

//             output_file_data_buffer[i / 2] = codeword1;
//             output_file_data_buffer[(i / 2) + 1] = codeword2;
//             output_file_data_buffer[(i / 2) + 2] = codeword3;
//             output_file_data_buffer[(i / 2) + 3] = codeword4;
//         }

//         fwrite(output_file_data_buffer, (overall_size / 2), 1, output_file);
//     }

//     if (encodeType == DECOMPRESS)
//     {

//         output_file_data_buffer = malloc((overall_size * 2) * sizeof(char));

//         for (i = 0; i < overall_size; i = i + 8)
//         {

//             input_data1 = bytes_to_int16(inputfile_data_buffer[i], inputfile_data_buffer[i + 1]);
//             input_data2 = bytes_to_int16(inputfile_data_buffer[i + 2], inputfile_data_buffer[i + 3]);
//             input_data3 = bytes_to_int16(inputfile_data_buffer[i + 4], inputfile_data_buffer[i + 5]);
//             input_data4 = bytes_to_int16(inputfile_data_buffer[i + 6], inputfile_data_buffer[i + 7]);

//             codeword1 = codeword_decompression(input_data1);
//             codeword2 = codeword_decompression(input_data2);
//             codeword3 = codeword_decompression(input_data3);
//             codeword4 = codeword_decompression(input_data4);

//             output_file_data_buffer[i / 2] = codeword1;
//             output_file_data_buffer[(i / 2) + 1] = codeword2;
//             output_file_data_buffer[(i / 2) + 2] = codeword3;
//             output_file_data_buffer[(i / 2) + 3] = codeword4;
//         }
//         fwrite(output_file_data_buffer, (overall_size * 2), 1, output_file);
//     }

//     fclose(input_file);
//     fclose(output_file);

//     return 0;
// }

int encode_wave_file(char *input_file_name, char *output_file_name, bool encodeType)
{
    FILE *input_file, *output_file;
    wave_header_t wav_header;

    unsigned char byte_buffer_2[2], byte_buffer_4[4];
    unsigned char *inputfile_data_buffer, *output_file_data_buffer;

    int16_t input_data1, input_data2, input_data3, input_data4;
    int8_t codeword1, codeword2, codeword3, codeword4;

    input_file = fopen(input_file_name, "rb+");
    output_file = fopen(output_file_name, "wb");

    // Riff - does not change
    fread(wav_header.riff, sizeof(wav_header.riff), 1, input_file);
    fwrite(wav_header.riff, sizeof(wav_header.riff), 1, output_file);

    // Overall Size (ChunkSize) - the new size must be calculated and written
    fread(wav_header.overall_size, sizeof(wav_header.overall_size), 1, input_file);
    unsigned int overall_size = wav_header.overall_size[0] | (wav_header.overall_size[1] << 8) | (wav_header.overall_size[2] << 16) | (wav_header.overall_size[3] << 24);
    unsigned int new_overall_size = overall_size + 8 - 44;

    new_overall_size = new_overall_size / 2;
    new_overall_size = new_overall_size + 44 - 8;
    byte_buffer_4[0] = (new_overall_size >> 24) & 0xFF;
    byte_buffer_4[1] = (new_overall_size >> 16) & 0xFF;
    byte_buffer_4[2] = (new_overall_size >> 8) & 0xFF;
    byte_buffer_4[3] = new_overall_size & 0xFF;
    fwrite(&byte_buffer_4[3], 1, 1, output_file);
    fwrite(&byte_buffer_4[2], 1, 1, output_file);
    fwrite(&byte_buffer_4[1], 1, 1, output_file);
    fwrite(&byte_buffer_4[0], 1, 1, output_file);

    // Wave(Format) - does not change between I/O
    fread(wav_header.format, sizeof(wav_header.format), 1, input_file);
    fwrite(wav_header.format, sizeof(wav_header.format), 1, output_file);

    // Wave (Subchunk1ID) - does not change between I/O
    fread(wav_header.subChunk1ID, sizeof(wav_header.subChunk1ID), 1, input_file);
    fwrite(wav_header.subChunk1ID, sizeof(wav_header.subChunk1ID), 1, output_file);

    // Wave (Subchunk1Size) - does not change between I/O
    fread(wav_header.subChunk1Size, sizeof(wav_header.subChunk1Size), 1, input_file);
    fwrite(wav_header.subChunk1Size, sizeof(wav_header.subChunk1Size), 1, output_file);

    // Wave (Audio Format) :  1=PCM; 6=ALAW; 7=MuLaw
    fread(wav_header.audioFormat, sizeof(wav_header.audioFormat), 1, input_file);
    byte_buffer_2[0] = 7;
    byte_buffer_2[1] = '\0';
    fwrite(&byte_buffer_2[0], 1, 1, output_file);
    fwrite(&byte_buffer_2[1], 1, 1, output_file);

    // Number of Channels - record the value but do not change it
    fread(wav_header.numChannels, sizeof(wav_header.numChannels), 1, input_file);
    unsigned int numChannels = wav_header.numChannels[0] | (wav_header.numChannels[1] << 8);
    fwrite(wav_header.numChannels, sizeof(wav_header.numChannels), 1, output_file);

    // Sample Rate - Record the value but do not change it
    fread(wav_header.sampleRate, sizeof(wav_header.sampleRate), 1, input_file);
    unsigned int sampleRate = wav_header.sampleRate[0] | (wav_header.sampleRate[1] << 8) | (wav_header.sampleRate[2] << 16) | (wav_header.sampleRate[3] << 24);
    fwrite(wav_header.sampleRate, sizeof(wav_header.sampleRate), 1, output_file);

    // Byte Rate = (SampleRate * NumChannels * BitsPerSample) / 8
    fread(wav_header.byteRate, sizeof(wav_header.byteRate), 1, input_file);
    unsigned int byteRate = (sampleRate * numChannels * BITS_PER_SAMPLE) / 8;
    byte_buffer_4[0] = (byteRate >> 24) & 0xFF;
    byte_buffer_4[1] = (byteRate >> 16) & 0xFF;
    byte_buffer_4[2] = (byteRate >> 8) & 0xFF;
    byte_buffer_4[3] = byteRate & 0xFF;
    fwrite(&byte_buffer_4[3], 1, 1, output_file);
    fwrite(&byte_buffer_4[2], 1, 1, output_file);
    fwrite(&byte_buffer_4[1], 1, 1, output_file);
    fwrite(&byte_buffer_4[0], 1, 1, output_file);

    // Wave (Block Align)  = (NumChannels * BitsPerSample) / 8
    fread(wav_header.blockAlign, sizeof(wav_header.blockAlign), 1, input_file);
    unsigned int blockAlign = (numChannels * BITS_PER_SAMPLE) / 8;
    byte_buffer_2[0] = (blockAlign >> 8) & 0xFF;
    byte_buffer_2[1] = blockAlign & 0xFF;
    fwrite(&byte_buffer_2[1], 1, 1, output_file);
    fwrite(&byte_buffer_2[0], 1, 1, output_file);

    // Bits per sample = 8
    fread(wav_header.bitsPerSample, sizeof(wav_header.bitsPerSample), 1, input_file);
    byte_buffer_2[0] = BITS_PER_SAMPLE;
    byte_buffer_2[1] = '\0';
    fwrite(&byte_buffer_2[0], 1, 1, output_file);
    fwrite(&byte_buffer_2[1], 1, 1, output_file);

    // Wave (subChunk2ID) - does not change between I/O
    fread(wav_header.subChunk2ID, sizeof(wav_header.subChunk2ID), 1, input_file);
    fwrite(wav_header.subChunk2ID, sizeof(wav_header.subChunk2ID), 1, output_file);

    // Wave (Data size) - needs to be calculated
    fread(wav_header.subChunk2Size, sizeof(wav_header.subChunk2Size), 1, input_file);
    int chunk_header = new_overall_size + 8 - 44;
    byte_buffer_4[0] = (chunk_header >> 24) & 0xFF;
    byte_buffer_4[1] = (chunk_header >> 16) & 0xFF;
    byte_buffer_4[2] = (chunk_header >> 8) & 0xFF;
    byte_buffer_4[3] = chunk_header & 0xFF;
    fwrite(&byte_buffer_4[3], 1, 1, output_file);
    fwrite(&byte_buffer_4[2], 1, 1, output_file);
    fwrite(&byte_buffer_4[1], 1, 1, output_file);
    fwrite(&byte_buffer_4[0], 1, 1, output_file);

    overall_size -= 36;

    inputfile_data_buffer = malloc(overall_size * sizeof(char));

    fread(inputfile_data_buffer, overall_size, 1, input_file);

    if (encodeType == COMPRESS) {
        output_file_data_buffer = malloc((overall_size / 2) * sizeof(char));
        compress_wave_data(inputfile_data_buffer, output_file_data_buffer, overall_size);
        fwrite(output_file_data_buffer, (overall_size / 2), 1, output_file);
    } else if (encodeType == DECOMPRESS) {
        output_file_data_buffer = malloc((overall_size * 2) * sizeof(char));
        decompress_wave_data(inputfile_data_buffer, output_file_data_buffer, overall_size);
        fwrite(output_file_data_buffer, (overall_size * 2), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}

void compress_wave_data(unsigned char *inputfile_data_buffer, unsigned char *output_file_data_buffer, int overall_size)
{
    int16_t input_data1, input_data2, input_data3, input_data4;
    int8_t codeword1, codeword2, codeword3, codeword4;

    for (int i = 0; i < overall_size; i += 8) {
        input_data1 = bytes_to_int16(inputfile_data_buffer[i], inputfile_data_buffer[i + 1]);
        input_data2 = bytes_to_int16(inputfile_data_buffer[i + 2], inputfile_data_buffer[i + 3]);
        input_data3 = bytes_to_int16(inputfile_data_buffer[i + 4], inputfile_data_buffer[i + 5]);
        input_data4 = bytes_to_int16(inputfile_data_buffer[i + 6], inputfile_data_buffer[i + 7]);

        codeword1 = codeword_compression(input_data1, signum(input_data1));
        codeword2 = codeword_compression(input_data2, signum(input_data2));
        codeword3 = codeword_compression(input_data3, signum(input_data3));
        codeword4 = codeword_compression(input_data4, signum(input_data4));

        output_file_data_buffer[(i / 2) + 0] = codeword1;
        output_file_data_buffer[(i / 2) + 1] = codeword2;
        output_file_data_buffer[(i / 2) + 2] = codeword3;
        output_file_data_buffer[(i / 2) + 3] = codeword4;
    }
}

void decompress_wave_data(unsigned char *inputfile_data_buffer, unsigned char *output_file_data_buffer, int overall_size)
{
    int16_t input_data1, input_data2, input_data3, input_data4;
    int8_t codeword1, codeword2, codeword3, codeword4;

    for (int i = 0; i < overall_size; i += 4) {
        input_data1 = codeword_decompression(inputfile_data_buffer[i]);
        input_data2 = codeword_decompression(inputfile_data_buffer[i + 1]);
        input_data3 = codeword_decompression(inputfile_data_buffer[i + 2]);
        input_data4 = codeword_decompression(inputfile_data_buffer[i + 3]);

        codeword1 = input_data1 << 6;
        codeword2 = input_data2 << 6;
        codeword3 = input_data3 << 6;
        codeword4 = input_data4 << 6;

        output_file_data_buffer[(i * 2) + 0] = (codeword1 >> 8) & 0xFF;
        output_file_data_buffer[(i * 2) + 1] = codeword1 & 0xFF;
        output_file_data_buffer[(i * 2) + 2] = (codeword2 >> 8) & 0xFF;
        output_file_data_buffer[(i * 2) + 3] = codeword2 & 0xFF;
        output_file_data_buffer[(i * 2) + 4] = (codeword3 >> 8) & 0xFF;
        output_file_data_buffer[(i * 2) + 5] = codeword3 & 0xFF;
        output_file_data_buffer[(i * 2) + 6] = (codeword4 >> 8) & 0xFF;
        output_file_data_buffer[(i * 2) + 7] = codeword4 & 0xFF;
    }
}
