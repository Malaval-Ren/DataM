
#define TEMP_FILE_NAME  "tmp000"

#ifndef MY_CONV_H
#define MY_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

    extern char            *parseAntiSlashChar( char **pPathname);
    extern char            *parseSpaceChar( char **pPathname);
    extern char            *getBasePathname( const char* pPathname, unsigned int uExtension);
    extern char            *getFileName( char *pPathname);
    extern unsigned int     getMyFileSize( char *pPathname);
    extern char            *readFileToMemory( char *pPathname);
    extern int              writeFileFromMemory( char *pPathname, char *pCharData, unsigned int uDataSize);
    extern BOOL             pathFileExists( const char* pPathname);
    extern BOOL             directoryExists( const char *pAbsolutePath);
    extern void             exitOnError(char* pExplain, char* pDetail, char* pInfo, unsigned int uError);

#ifdef __cplusplus
}
#endif

#endif