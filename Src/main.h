
#ifndef MY_GLOBAL_H
#define MY_GLOBAL_H

//#define EMPTY       0
//#define DUMP        1
//#define DATALST     2
//#define CSVTOC      3
//#define GETSTR      4

/**
 * @enum eCommand_t
 * @brief  the eCommand_t parameter
 */
typedef enum {
    eEMPTY = 0,
    eDUMP,
    eDATALST,
    eCSVTOC,
    eGETSTR
} eCommand_t;


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
    char           *pCheckEnumPathname;
    int             iTabColumns;
} ConvmArguments;

#endif
