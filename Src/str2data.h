
#ifndef MY_STR2DATA_H
#define MY_STR2DATA_H

#ifdef __cplusplus
extern "C" {
#endif

    void findNumLinesLst( char *pinputFile, unsigned short int *puNumLines, unsigned short int *puDataSize);

    char *mySentence( char *pOutputFileData, unsigned short int uNumber);

    unsigned short int str2data( char *pInputFileName, unsigned short int uNumLines, char *pOutputFileData, unsigned short int uDataSize);

    unsigned int myComputeIndexOf( char *pCheckEnumPathname, char *pEnumElementName);

#ifdef __cplusplus
}
#endif

#endif
