#ifndef __AU_READER_H__
#define __AU_READER_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define AU_MAGIC 0x2e736e64

#define AU_READ_SUCCESS 0
#define AU_READ_FAILURE_INVALID_MAGIC 1
#define AU_READ_FAILURE_OPEN_FILE_ERROR 2

#define AU_WRITE_SUCCESS 0
#define AU_WRITE_FAILURE_OPEN_FILE_ERROR 1

typedef uint32_t uint32;
typedef uint8_t  uint8;

typedef struct AuHeaderFields_st
{
    uint32 magic;
    uint32 data_offset;
    uint32 data_size;
    uint32 encoding;
    uint32 sample_rate;
    uint32 channels;
}
AuHeaderFields;

typedef struct AuFileData_st
{
    AuHeaderFields fields;
    uint8* header;
    uint8* data;
}
AuFileData;

typedef enum AuEncoding_en
{
    AU_8BIT_G_711_U_LAW = 1,
    AU_8BIT_LPCM = 2,
    AU_16BIT_LPCM = 3,
    AU_24BIT_LPCM = 4,
    AU_32BIT_LPCM = 5,
    AU_32BIT_FLOAT = 6,
    AU_64BIT_FLOAT = 7,
    AU_FRAGMENTED = 8,
    AU_DSP_PROGRAM = 9,
    AU_8BIT_FIXED = 10,
    AU_16BIT_FIXED = 11,
    AU_24BIT_FIXED = 12,
    AU_32BIT_FIXED = 13,
    AU_16BIT_LINEAR_EMPH = 18,
    AU_16BIT_LINEAR_COMP = 19,
    AU_16BIT_LINEAR_EMPH_COMP = 20,
    AU_MUSIC_KIT_DSP_COMMANDS = 21,
    AU_4BIT_G_721_ADPCM = 23,
    AU_SB_G_722_ADPCM = 24,
    AU_3BIT_G_723_ADPCM = 25,
    AU_5BIT_G_723_ADPCM = 26,
    AU_8BIT_G_711_A_LAW = 27
}
AuEncoding;

// reverse the bytes of a 32-bit integer
uint32 au_ReverseBytes(uint32 value);

// initialize a fields struct with null values
AuHeaderFields au_GetNullFields(void);

// read the header data and return them in a fields struct
AuHeaderFields au_GetHeaderFields(char* file_name);

// initialize an au data struct with null values
AuFileData au_GetNullData(void);

// destroy data inside an au data struct
void au_FreeData(AuFileData* au_data);

// read a file and load its data in a au data struct
uint8 au_ReadFile(char* file_name, AuFileData* au_data);

// write the data inside an au data struct to a file
uint8 au_WriteFile(char* file_name, AuFileData au_data);

#endif
