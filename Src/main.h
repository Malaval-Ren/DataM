
#ifndef MY_GLOBAL_H
#define MY_GLOBAL_H

#define NONE        0
#define DUMP        1
#define DATALST     2
#define CSVTOC      3

#pragma pack(1)     // structure without padding....

typedef struct
{
    signed short int    NumberOfBytes;
    unsigned short int  mode;
} DirEntry;

#pragma pack()

typedef struct
{
    char           *pFullFilename;
    char           *pOutputPathname;
    int             iTabColumns;
} ConvmArguments;

#endif
