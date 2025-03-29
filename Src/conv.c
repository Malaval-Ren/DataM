/* SPDX - License - Identifier: GPL - 3.0 - or -later
 *
 * A tool to help cross dev for Apple II GS.
 *
 * Copyright(C) 2023 - 2025 Renaud Malaval <renaud.malaval@free.fr>.
 *
 * This program is free software : you can redistribute it and /or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 *  GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *  along with this program.If not, see < https://www.gnu.org/licenses/>.
 */

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"

#include "conv.h"

// _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/**
* @fn char *parseAntiSlashChar( char **pPathname)
* @brief Replace character anti slash '\\' by a slash '/'
*
* @param[in,out]   pPathname
*
* @@return A new pointer pResultFilename, this is pPathname content corrected and pPathname is free or NULL
*/
char *parseAntiSlashChar( char **pPathname)
{
    char    *pResultFilename = NULL;
    char    *pInRunner;
    char    *pOutRunner;
    //int      iModif = 0;

    pResultFilename = (char *)calloc( 1, _MAX_PATH + _MAX_PATH);
    if (pResultFilename)
    {
        pInRunner = *pPathname;
        pOutRunner = pResultFilename;
        while (*pInRunner != '\0')
        {
            if (*pInRunner == '\\')
            {
                *pOutRunner = '/';
                pOutRunner++;
                //iModif++;
                if (*pInRunner == '\\')
                {
                    pInRunner++;
                }
            }

            *pOutRunner = *pInRunner;
            pInRunner++;
            pOutRunner++;
        }
        *pOutRunner = '\0';

        //if (iModif != 0)
        //{
        //  // (void )printf( "Full in  : %s\n", *pPathname);
        //  (void )printf( "Working file : %s\n\n", pResultFilename);
        //  (void )printf( "Anti Slash corrected is = %d\n", iModif);
        //}

        free( *pPathname);
        *pPathname = NULL;
    }

    return pResultFilename;
}


/**
* @fn char *parseSpaceChar( char **pPathname)
* @brief parse the pPathname if a space if found add an anti slash before
*
* @param[in,out]   pPathname
*
* @return A new pointer pResultFilename, this is pPathname content corrected and pPathname is free or NULL
*/
char *parseSpaceChar( char **pPathname)
{
    char    *pResultFilename = NULL;
    char    *pInRunner;
    char    *pOutRunner;
    //int      iModif = 0;

    if ((pPathname) && (*pPathname))
    {
        pResultFilename = (char *)calloc(1, _MAX_PATH + _MAX_PATH);
        if (pResultFilename)
        {
            pInRunner = *pPathname;
            pOutRunner = pResultFilename;
            while (*pInRunner != '\0')
            {
                if (*pInRunner == ' ')
                {
                    *pOutRunner = '\\';
                    pOutRunner++;
                    //iModif++;
                }
                else if (*pInRunner == '\\')
                {
                    *pOutRunner = '\\';
                    pOutRunner++;
                }

                *pOutRunner = *pInRunner;
                pInRunner++;
                pOutRunner++;
            }
            *pOutRunner = '\0';

            //if (iModif != 0)
            //{
            //  //(void )printf( "Full in  : %s\n", *pPathname);
            //  (void )printf( "Working file : %s\n\n", pResultFilename);
            //  (void )printf( "Space char corrected is = %d\n", iModif);
            //}

            free( *pPathname);
            *pPathname = NULL;
        }
    }

    return pResultFilename;
}


/**
* @fn char *getBasePathname( const char *pPathname, unsigned int uExtension)
* @brief Parse the pPathname to get folder(s) only
*
* @param[in]    pPathname
* @param[in]    uExtension
*
* @return A new pointer pResultFilename to the base of pPathname (remove at the end the filename) or NULL
*/
char *getBasePathname( const char *pPathname, unsigned int uExtension)
{
    char       *pBackRunning;
    char       *pResultFilename = NULL;
    size_t      uLenResultFilename;
    //char        dirSeparator = '\\';      // set dirSeparator according to platform
    char        dirSeparator = '/';     // set dirSeparator according to platform

    if (pPathname)
    {
        uLenResultFilename = strlen( pPathname) + (size_t )uExtension;
        pResultFilename = (char *)calloc( 1,((size_t )( uLenResultFilename) + (size_t )( 1)));
        if (pResultFilename)
        {
            (void )strncpy_s( pResultFilename, uLenResultFilename, pPathname, (int )strlen( pPathname));
            //(void)strncpy( pResultFilename, pPathname, uLenResultFilename);

            pBackRunning = (char *)&(pResultFilename[(int )strlen( pResultFilename) - 1]);
            while (*pBackRunning != dirSeparator)
            {
                pBackRunning--;
            }

            pBackRunning++;
            *pBackRunning = '\0';
        }
    }

    return pResultFilename;
}


/**
* @fn char *getFileName( char *pPathname)
* @brief Parse the pPathname to get filename
*
* @param[in]    pPathname
*
* @return the filename from the pPathname
*/
char *getFileName( char *pPathname)
{
    char    *pReturnName = NULL;
    char    *pRunning;

    if (pPathname)
    {
        for (pRunning = pPathname; *pRunning; pRunning++)
        {
            if ( (*pRunning == '/') || (*pRunning == '\\') || (*pRunning == ':') )
            {
                pReturnName = pRunning;
            }
        }

        if (pReturnName)
        {
            if ( (*pReturnName == '/') || (*pReturnName == '\\') || (*pReturnName == ':') )
            {
                pReturnName++;
            }
        }
    }
    return pReturnName;
}


/**
* @fn unsigned int getMyFileSize( char *pPathname)
* @brief get the size of file pPathname
*
* @param[in]    pPathname
*
* @return Size of the file pPathname or 0
*/
unsigned int getMyFileSize( char *pPathname)
{
    errno_t         iError;
    unsigned int    uSize = 0;
    FILE           *pFile = NULL;

    iError = fopen_s( &pFile, pPathname, "rb");
    if (iError == 0)
    {
        if (pFile)
        {
            (void )fseek( pFile, 0L, SEEK_END);
            uSize = ftell( pFile);
            (void )fclose( pFile);
        }
    }

    return uSize;
}


/**
* @fn char *readFileToMemory( char *pPathname)
* @brief read the content of file pPathname in memory
*
* @param[in]   pPathname
*
* @return Pointer to the pPathname content or NULL
*/
char *readFileToMemory( char *pPathname)
{
    FILE       *pFile = NULL;
    char       *pCharData = NULL;
    size_t      uNumberOfElementRead = 0;
    int         iFileSize = 0;
    errno_t     iError = 0;

    iFileSize = getMyFileSize( pPathname);
    if (iFileSize)
    {
        pCharData = (char *)calloc( 1, iFileSize);
        if (pCharData)
        {
            iError = fopen_s( &pFile, pPathname, "rb, ccs=UTF-8");
            if ( (iError == 0) && (pFile != NULL) )
            {
                uNumberOfElementRead = fread( pCharData, iFileSize, 1, pFile);
                (void )fclose( pFile);
                if (uNumberOfElementRead != 1)
                {
                    iError = 1;
                }
            }

            if ( (iError != 0) && (pFile != NULL) )
            {
                free( pCharData);
                pCharData = NULL;
            }
        }
    }
    return pCharData;
}

/**
* @fn int writeFileFromMemory( char *pPathname, char *pCharData, unsigned int uDataSize)
* @brief Write to file pPathname data from pCharData with size uDataSize
*
* @param[in]   pPathname
* @param[in]   pCharData
* @param[in]   uDataSize
*
* @return 0 if no error
*/
int writeFileFromMemory( char *pPathname, char *pCharData, unsigned int uDataSize)
{
    errno_t     iError = 0;
    FILE       *pFile = NULL;
    size_t      numberOfElementWrite = 0;

    iError = fopen_s( &pFile, (const char *)pPathname, "wb");
    if ((iError == 0) && (pFile != NULL))
    {
        numberOfElementWrite = fwrite( pCharData, uDataSize, 1, pFile);

        (void )fclose( pFile);
        if (numberOfElementWrite != 1)
        {

            iError = 1;
        }
    }

    return iError;
}

/**
* @fn BOOL pathFileExists( const char *pPathname)
* @brief Check if pPathname exist
*
* @param[in]        pPathname
*
* @return TRUE if file exist and FALSE for other case
*/
BOOL pathFileExists( const char *pPathname)
{
    BOOL        bFileExists = FALSE;
    FILE       *pFile = NULL;

    if (fopen_s( &pFile, pPathname, "rb") )
    {
        bFileExists = FALSE;
    }
    else
    {
        bFileExists = TRUE;
        if ( pFile)
        {
            (void )fclose( pFile);
        }
    }

    return bFileExists;
}

/**
* @fn BOOL directoryExists( const char *pAbsolutePath)
* @brief Check if pAbsolutePath folder exist
*
* @param[in]        pAbsolutePath
*
* @return TRUE if folder exist and FALSE for other case
*/
BOOL directoryExists( const char *pAbsolutePath)
{
    BOOL bFolderExists = FALSE;

    if (_access( pAbsolutePath, 0) == 0)
    {
        struct stat status;

        (void )stat( pAbsolutePath, &status);
        if ((status.st_mode & S_IFDIR) != 0)
        {
            bFolderExists = TRUE;
        }
        else
        {
            bFolderExists = FALSE;
        }
    }

    return bFolderExists;
}

/**
* @fn void exitOnError( char *pExplain, char *pDetail, char *pInfo, unsigned int uError)
* @brief Center for exit when an error occur
*
* @param[in]        pExplain
* @param[in,out]    pDetail     // this pointer if free
* @param[in]        pInfo
* @param[in]        uError
*
*/
void exitOnError( char *pExplain, char *pDetail, char *pInfo, unsigned int uError)
{
    const char  *pEndString = NULL;
    const char  *pSpaceString = NULL;
    char        *pMessage = NULL;
    size_t       uLen;

    if (pExplain)
    {
        pEndString = "\"";
        pSpaceString = " ";

        uLen = strlen( (const char *)pExplain);
        if (pDetail)
        {
            uLen += strlen( (const char *)pDetail);
        }
        if (pInfo)
        {
            uLen += strlen( (const char *)pInfo);
        }
        uLen += (size_t )32;    // a marge for char ' '; '"'

        pMessage = (char *)calloc( 1, uLen);
        if (pMessage)
        {
            (void )strncpy_s( pMessage, uLen, (const char *)pExplain, strlen( (const char *)pExplain));

            if (pDetail)
            {
                pMessage = strcat( pMessage, (const char *)pSpaceString);
                pMessage = strcat( pMessage, (const char *)pSpaceString);
                pMessage = strcat( pMessage, (const char *)pEndString);
                pMessage = strcat( pMessage, (const char *)pDetail);
                pMessage = strcat( pMessage, (const char *)pEndString);
            }

            if (pInfo)
            {
                pMessage = strcat( pMessage, (const char *)pSpaceString);
                pMessage = strcat( pMessage, (const char *)pSpaceString);
                pMessage = strcat( pMessage, (const char *)pEndString);
                pMessage = strcat( pMessage, (const char *)pInfo);
                pMessage = strcat( pMessage, (const char *)pEndString);
            }

            (void )printf( "DATAM : %s\n", pMessage);

            free( pMessage);
        }
    }

    exit( uError);
}
