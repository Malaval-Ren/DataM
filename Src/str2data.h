
#ifndef MY_STR2DATA_H
#define MY_STR2DATA_H

#ifdef __cplusplus
extern "C" {
#endif

    void findNumLinesLst( char *pinputFile, unsigned short *puNumLines, unsigned short *puDataSize);

    char *mySentence( char *pOutputFileData, unsigned short uNumber);

    unsigned short str2data( char *pInputFileName, unsigned short uNumLines, char *pOutputFileData, unsigned short uDataSize);

    unsigned short myComputeIndexOf( char *pCheckEnumPathname, char *pEnumElementName);

#ifdef __cplusplus
}
#endif

#endif
