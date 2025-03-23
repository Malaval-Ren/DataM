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

// datam.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <windows.h>

//#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <direct.h>

#include "main.h"
#include "conv.h"

#include "str2data.h"

// _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/*
 * where to found the wnidows error code :
 * https://docs.microsoft.com/fr-fr/windows/win32/debug/system-error-codes--0-499-
 */

/*
 * coding rules:
 *
 * BOOL             b<Name>
 * int              i<Name>
 * unsigned int     u<Name>
 * unsigned char    u<Name>
 * unsigned short   u<Name>
 * unsigned #       u<Name>
 * pointer         *p<Name>
 */

/*
 * version management is :
 *  4 numbers separate by a '.'
 *  major.minor.maintenance.build (example: 1.1.3.99)
 */

/*
 * complement ligne de iCommand pour debug :
 * -dblf ../../textes.cc
 *
 * -rled ../../AUBERGIS.shr
 */


/*
 * @fn static void usage( char *pAboutString)
 * @brief Show how to use soft
 *
 * @param[in]        pAboutString
 */
static void usage( char *pAboutString)
{
    if (pAboutString)
    {
        printf("\n%s\n\n", pAboutString);
    }
    printf( "Usage: datam <convmspec> <option> \"<filespec>\" \"<output folder>\"\n");
    printf( "\n  <convmspec> is one of the following:\n");
    printf( "   -dump        - dump content of a supported file format\n");
    printf( "   -sltb        - file with strings list to binary file\n");
    printf( "   -ctbl        - create table from .cvs to .c\n");

    printf( "\n  <option> is one of the following:\n");
    printf( "   +lower       - the output file name is in lower case\n");
    printf( "   +chkenu      - the path to file with enum eSentence_t\n");

    printf( "\n  <filespec> file extension could be:\n");
    printf( "   -dump           : any\n");
}

/**
* @fn static int checkFileExtension( char *pPathFilename, int iCommand)
* @brief Check the 4 last char of file name to be .scr, .shr, .pnt, .pic
*
* @param[in]        pPathFilename
* @param[in]        iCommand
*
* @return 0 if ok, or 3 on error but never return the software exit
*/
static int checkFileExtension( char *pPathFilename, int iCommand)
{
    const char  *pEndString = NULL;
    char        *pRunning = NULL;
    char        *pLastPointChar = NULL;
    int          iError = 0;
    BOOL         bErrorNoExt = FALSE;
    BOOL         bError = FALSE;
    BOOL         bErrorCmd = FALSE;
    BOOL         bErrorExt = FALSE;
    const char  *pCmdtext[] = { "none", "-dump", "-sltb", "-ctbl" };

    if (iCommand == DUMP)
    {
        iError = 0;
    }
    else
    {
        pEndString = getFileName( pPathFilename);
        if (pEndString)
        {
            pRunning = (char*)pEndString;

            while (*pRunning != '\0')
            {
                if (*pRunning == '.')
                {
                    pLastPointChar = pRunning;
                }
                pRunning++;
            }

            if (pLastPointChar)
            {
                pLastPointChar++;
                if (*pLastPointChar == '\0')
                {
                    bErrorNoExt = TRUE;
                }
                else if (strlen( pLastPointChar) == 3)
                {
                    if (iCommand == DATALST)
                    {
                        if (strcmp( (const char *)pLastPointChar, "txt"))
                        {
                            bError = TRUE;
                        }
                    }
                    else if (iCommand == CSVTOC)
                    {
                        if (strcmp( (const char *)pLastPointChar, "csv"))
                        {
                            bError = TRUE;
                        }
                    }
                }
                else
                {
                    bError = TRUE;
                }

                if (bError)
                {
                    exitOnError( (char *)"file extension not supported", (char *)pCmdtext[iCommand], (char *)pEndString, 3);
                    iError = 1;
                }

                if (bErrorCmd)
                {
                    exitOnError( (char *)"command already done on this file", (char *)pCmdtext[iCommand], (char *)pEndString, 3);
                    iError = 1;
                }

                if (bErrorExt)
                {
                    exitOnError( (char *)"file not compatible with command", (char *)pCmdtext[iCommand], (char *)pEndString, 5);
                    iError = 1;
                }
            }
            else
            {
                bErrorNoExt = TRUE;
            }

            if (bErrorNoExt)
            {
                exitOnError( (char *)"file without extension", NULL, pPathFilename, 3);
                iError = 1;
            }
        }
    }

    return iError;
}

/**
* @fn static void pathanmeToLowerCase( char **pPathname)
* @brief parse the pathname and set only the file name in lower case
*
* @param[in]        pPathname
*/
static void pathanmeToLowerCase( char **pPathname)
{
    char        *pRunning = NULL;
    const char  *pEndString = NULL;

    // printf("before input path is : %s\n", context.pFullFilename);
    pEndString = getFileName( *pPathname);
    if (pEndString)
    {
        pRunning = (char *)pEndString;

        while (*pRunning != '\0')
        {
            if (isupper( *pRunning))
            {
                *pRunning = (char )tolower( *pRunning);
            }
            pRunning++;
        }
        *pRunning = '\0';
    }
}

/**
* @fn static void getPathnameFromCmd( char *pInputPathname, char **pOutputPathname, BOOL bLowerCase, int iCommand, BOOL bIsAFile)
* @brief get pathname clean it and store it
*
* @param[in]        pInputPathname      pathname of enum, input pathname in utf8, output pathname in binary
* @param[in,out]    pOutputPathname     handle to cleaned pInputPathname
* @param[in]        bLowerCase          convert pathname in lower case
* @param[in]        iCommand            0 for enum file, 1 for input file, 2 for output file
* @param[in]        bIsAFile            True for a file, False for a folder
*/
static void getPathnameFromCmd( char *pInputPathname, char **pOutputPathname, BOOL bLowerCase, int iCommand, BOOL bIsAFile)
{
    char    *pDuplicateString = NULL;

    if ((pInputPathname) && (pOutputPathname))
    {
        if (!*pOutputPathname)
        {
            *pOutputPathname = (char *)calloc( 1, _MAX_PATH + _MAX_PATH);
            if (!*pOutputPathname)
            {
                exitOnError((char *)"Out of memory", NULL, NULL, 1);
            }
            else
            {
                if (((pInputPathname[0] == '.') && (pInputPathname[1] == '.')) ||
                    ((pInputPathname[2] != ':') && ((pInputPathname[3] != '/') && (pInputPathname[3] != '\\'))) ||
                    (((pInputPathname[0] != '/') && (pInputPathname[2] != '/')) || ((pInputPathname[0] != '\\') && (pInputPathname[2] != '\\'))))
                {
                    if (_fullpath( *pOutputPathname, pInputPathname, _MAX_PATH) != NULL)
                    {
                        if (bIsAFile)
                        {
                            if ( ! pathFileExists((const char *)*pOutputPathname))
                            {
                                exitOnError((char *)"File does not exist", NULL, pInputPathname, 2);
                            }
                        }
                        else
                        {
                            if ( ! directoryExists((const char*)*pOutputPathname))
                            {
                                if (pathFileExists((const char*)*pOutputPathname))
                                {
                                    exitOnError((char*)"File parameter must be a folder", NULL, *pOutputPathname, 2);
                                }
                                else
                                {
                                    exitOnError((char*)"File folder does not exist", NULL, *pOutputPathname, 2);
                                }
                            }
                        }
                    }
                    else
                    {
                        exitOnError((char *)"Invalid input parameter", NULL, pInputPathname, 2);
                    }
                }
                else
                {
                    (void )strncpy( *pOutputPathname, (const char *)pInputPathname, strlen( (const char *)pInputPathname) + (size_t)1);
                }
            }

            *pOutputPathname = parseAntiSlashChar(pOutputPathname);
            if (bIsAFile)
            {
                if (pathFileExists( (const char*)*pOutputPathname))
                {
                    if (getMyFileSize( *pOutputPathname) == 0)
                    {
                        exitOnError( (char *)"Size equal to 0 for file", NULL, *pOutputPathname, 3);
                    }
                    else
                    {
                        if (bLowerCase)
                        {
                            pathanmeToLowerCase( pOutputPathname);
                        }
                        else
                        {
                            pDuplicateString = strdup( *pOutputPathname);
                            if (pDuplicateString)
                            {
                                pathanmeToLowerCase( pOutputPathname);
                            }
                        }

                        if (iCommand)
                        {
                            if (!checkFileExtension( *pOutputPathname, iCommand))
                            {
                                if (pDuplicateString)
                                {
                                    free( *pOutputPathname);
                                    *pOutputPathname = pDuplicateString;
                                }
                            }
                            else
                            {
                                if (pDuplicateString)
                                {
                                    free( *pOutputPathname);
                                    *pOutputPathname = pDuplicateString;
                                }
                                exitOnError((char*)"File does not exist", *pOutputPathname, NULL, 2);
                            }
                        }
                    }
                }
                else
                {
                    exitOnError((char*)"File does not exist", *pOutputPathname, NULL, 2);
                }
            }
            else
            {
                if (*pOutputPathname)
                {
                    if ( ! directoryExists( (const char *)*pOutputPathname))
                    {
                        // Try to create it
                        (void )CreateDirectoryA( (const char *)*pOutputPathname, NULL);
                    }

                    if (directoryExists( (const char *)*pOutputPathname))
                    {
                        (void )printf( "\n");
                        (void )printf( "param output path is : %s\n", *pOutputPathname);
                    }
                    else
                    {
                        exitOnError( (char *)"output folder does not exist", *pOutputPathname, NULL, 2);
                        // TODO: try create the output folder
                    }
                }
                else
                {
                    exitOnError( (char *)"out of memory", NULL, NULL, 1);
                }
            }
        }
    }
    else
    {
        exitOnError( (char *)"Bad parameters", NULL, NULL, 1);
    }
}

/**
* @fn static int parseArguments( int argc, char *argv[], ConvmArguments *pContext)
* @brief parse the arguments and check it
*
* @param[in]        argc
* @param[in]        argv[]
* @param[in,out]    pContext
*
* @return the convmspec value and filled pContext structure, on error the software exit
*/
static int parseArguments( int argc, char *argv[], ConvmArguments *pContext)
{
    char           *pDuplicateString = NULL;
    const char     *pEndString = NULL;
    unsigned int    uIndex = 0;
    unsigned int    uLoop = 0;
    int             iCommand = 0;
    BOOL            bLowerCase = FALSE;

    for (uLoop = 1; uLoop < (unsigned int)argc; uLoop++)
    {
        if ( (argv[uLoop][0] == '-') || (argv[uLoop][0] == '+') )   // lower case the convmspec arguments
        {
            uIndex = 0;
            while (argv[uLoop][uIndex])
            {
                argv[uLoop][uIndex] = (char )tolower( argv[uLoop][uIndex]);
                uIndex++;
            }
        }
    }

    /*
     * Read through command-line arguments for convmspecs and options.
     */
    for (uIndex = 1; uIndex < (unsigned int )argc; uIndex++)
    {
        if (argv[uIndex][0] == '-')     // convmspec
        {
            if (!strcmp( (const char *)argv[uIndex], "-dump"))
            {
                iCommand = DUMP;
            }
            else if (!strcmp( (const char *)argv[uIndex], "-sltb"))    /* strings list to binary file */
            {
                iCommand = DATALST;
            }
            else if (!strcmp( (const char *)argv[uIndex], "-ctbl"))    /* strings list to binary file */
            {
                iCommand = CSVTOC;
            }
            else
            {
                exitOnError( (char *)"Illegal convmspec parameter", argv[uIndex], NULL, 2);
            }
        }
        else if (argv[uIndex][0] == '+')    // option, check
        {
            if ( !strcmp( (const char *)argv[uIndex], "+lower"))
            {
                bLowerCase = TRUE;
            }
            else if ( !strcmp( (const char *)argv[uIndex], "+chkenu"))
            {
                if (!pContext->pCheckEnumPathname)
                {
                    uIndex++;
                    getPathnameFromCmd( argv[uIndex], &pContext->pCheckEnumPathname, bLowerCase, 0, TRUE);
                }
            }
            else
            {
                exitOnError( (char *)"Illegal option parameter", argv[uIndex], NULL, 2);
            }
        }
        else
        {
            if (!pContext->pFullFilename)
            {
                getPathnameFromCmd( argv[uIndex], &pContext->pFullFilename, bLowerCase, iCommand, TRUE);
            }
            else if (!pContext->pOutputPathname)
            {
                getPathnameFromCmd( argv[uIndex], &pContext->pOutputPathname, bLowerCase, iCommand, FALSE);
            }
            else
            {
                exitOnError( (char *)"too many parameters", NULL, argv[uIndex], 1);
            }
        }
    }

    if ((iCommand == NONE) || ((pContext->pFullFilename == NULL) || (pContext->pOutputPathname == NULL)) )
    {
        if (pContext->pFullFilename)
        {
            free( pContext->pFullFilename);
            pContext->pFullFilename = NULL;
        }
        if (pContext->pOutputPathname)
        {
            free( pContext->pOutputPathname);
            pContext->pOutputPathname = NULL;
        }
        if (pContext->pCheckEnumPathname)
        {
            free(pContext->pCheckEnumPathname);
            pContext->pCheckEnumPathname = NULL;
        }
        if (iCommand == NONE)
        {
            exitOnError((char*)"Parameters not understood", NULL, NULL, 1);
        }
        else
        {
            exitOnError((char*)"Missing parameters", NULL, NULL, 1);
        }
    }

    return iCommand;
}

/**
* @fn static int mySystemCmd( const char *pCommand)
* @brief parse the arguments and check it
*
* @param[in]        pCommand
*
* @return 0 is not error, other values in case of error
*/
static int mySystemCmd( const char *pCommand)
{
    int                 iError = 6;
    char               *pTmpCommand = NULL;
    size_t              tmpCommandLen;
    char               *pCcmdExePath = NULL;
    size_t              cmd_exe_path_len;

    if (pCommand)
    {
        pCcmdExePath = getenv( "COMSPEC");
        if (pCcmdExePath)
        {
            cmd_exe_path_len = strlen(pCcmdExePath);

            tmpCommandLen = strlen(pCommand);
            pTmpCommand = (char *)calloc(1, cmd_exe_path_len + tmpCommandLen + 5);
            if (pTmpCommand)
            {
                (void )memcpy( pTmpCommand, pCcmdExePath, cmd_exe_path_len + 1);
                (void )strcat( pTmpCommand + cmd_exe_path_len, " /c ");   // cmd
                (void )memcpy( pTmpCommand + cmd_exe_path_len + 4, pCommand, tmpCommandLen + 1);

                (void )fflush( NULL);

                iError = system( pTmpCommand);
                if (iError)
                {
                    (void )printf( "DATAM: \"%s\") failed error = %d\n", pTmpCommand, iError);
                }

                free( pTmpCommand);
            }
        }
        else
        {
            iError = 1;
        }
    }

    return iError;
}


/**
* @fn static void updateFileType( ConvmArguments *pContext, char *pfullOutputFilename)
* @brief if iix emulator is present set a filetype and aux filetype to tthe output file
*
* @param[in]        pContext
* @param[in]        pfullOutputFilename
*
*/
static void updateFileType( ConvmArguments *pContext, char *pfullOutputFilename)
{
    const char  *pCheckEmulatorPresent = "where iix > nul\0";
    const char  *pCmdEmulatorPnt = "iix chtyp -t PNT -a 2 ";
    const char  *pCmdEmulatorPic = "iix chtyp -t PIC -a 0 ";
    const char  *pEndString = "\"";
    char        *pDuplicateString = NULL;
    char        *pShortPathname = NULL;
    char        *ptempFilename = NULL;
    int          iError;

    if ( (pContext) && (pfullOutputFilename) )
    {
        pShortPathname = (char *)calloc( 1, strlen(pfullOutputFilename) + 1);
        pDuplicateString = (char *)calloc( 1, _MAX_PATH + _MAX_PATH);
        if ((!pDuplicateString) || (!pShortPathname))
        {
            exitOnError( (char *)"out of memory", NULL, NULL, 1);
        }
        else
        {
            (void )printf( "DATAM: from = %s\n", pfullOutputFilename);
            ptempFilename = getBasePathname( pfullOutputFilename, 16);
            if (ptempFilename)
            {
                /* Change Windows pathname to Dos pathname (8.3) */
                iError = GetShortPathNameA( (LPCSTR )ptempFilename, (LPSTR )pShortPathname, (DWORD )strlen(ptempFilename) + 1);
                if ( ! iError)
                {
                    (void )printf( "DATAM: GetShortPathName() failed error = %d\n", (int )GetLastError());
                    pShortPathname = strdup(pfullOutputFilename);
                }
                else
                {
                    if ( ! strlen( pShortPathname))
                    {
                        pShortPathname = strdup( pfullOutputFilename);
                    }
                    else
                    {
                        pShortPathname = strcat( pShortPathname, getFileName(pfullOutputFilename));
                    }
                }
                (void )printf( "DATAM: to   = %s\n", pShortPathname);
                free( ptempFilename);
            }
            else
            {
                pShortPathname = strdup( pfullOutputFilename);
            }

            /* little cyber security protection */
            if ( (*(unsigned long *)pCheckEmulatorPresent != (unsigned long )1919248503) ||
                ( (*(unsigned long *)pCmdEmulatorPnt != (unsigned long )544762217) ||
                    (*(unsigned long *)pCmdEmulatorPic != ((unsigned long )544762217) ) ) )
            {
                free( pDuplicateString);
                free( pShortPathname);
            }
            else
            {
                iError = mySystemCmd( (char *)pCheckEmulatorPresent);
                if ( ! iError)    // Check if iix is installed to use it
                {
                    ;
                }
                else        // CiderPress File Attribute Preservation Tags
                {
                    (void )strncpy_s( pDuplicateString, _MAX_PATH + _MAX_PATH, pShortPathname, (int )strlen( pShortPathname));
                    pEndString = "#060000\0";
                    pDuplicateString = strcat( pDuplicateString, pEndString);
                    (void )rename( (const char *)pShortPathname, (const char *)pDuplicateString);
                }

                free( pDuplicateString);
                free( pShortPathname);
            }
        }
    }
}

/**
* @fn static char *createOutputPathname( char *pFullFilename, char *pOutPathname, int iCommand)
* @brief alloc and change the file extension
*
* @param[in]        pFullFilename
* @param[in]        pOutPathname
* @param[in]        iCommand
*
* @return a new pointer to the new pathname
*/
static char *createOutputPathname( char *pFullFilename, char *pOutPathname, int iCommand)
{
    char            *pOutputPathname = NULL;
    const char      *pEndString = NULL;
    size_t           uOutputPathnameLen = 0;

    if ( (pFullFilename) && (iCommand != NONE) )
    {
        if (pOutPathname)
        {
            pEndString = getFileName( pFullFilename);
            if (pEndString)
                uOutputPathnameLen = strlen( (const char *)pOutPathname) + strlen( (const char *)pEndString) + (size_t )16;
            else
                uOutputPathnameLen = 0;
        }
        else
        {
            uOutputPathnameLen = strlen( (const char *)pFullFilename) + (size_t )16;
        }

        if (uOutputPathnameLen)
        {
            uOutputPathnameLen = ( (size_t )(uOutputPathnameLen / 4) * 4) + 4;

            pOutputPathname = (char *)calloc( 1, uOutputPathnameLen);
            if (pOutputPathname)
            {
                if ( (pOutPathname) && (pEndString) )
                {
                    (void )strncpy_s( pOutputPathname, uOutputPathnameLen, pOutPathname, strlen( (const char *)pOutPathname));
                    pOutputPathname = strcat( pOutputPathname, pEndString);
                    pEndString = NULL;
                }
                else
                {
                    (void )strncpy_s( pOutputPathname, uOutputPathnameLen, pFullFilename, strlen( (const char *)pFullFilename));
                    //(void )strncpy( pOutputPathname, pfullFilename, uOutputPathnameLen);
                }

                // Change the extention of the output file
                if (pOutputPathname[strlen( pOutputPathname) - 4] == '.')
                {
                    pEndString = &pOutputPathname[strlen( pOutputPathname) - 3];
                    if (pEndString)
                    {
                        pOutputPathname[strlen( pOutputPathname) - 3] = 'b';
                        pOutputPathname[strlen( pOutputPathname) - 2] = 'i';
                        pOutputPathname[strlen( pOutputPathname) - 1] = 'n';
                    }
                    else
                    {
                        pOutputPathname = strcat( pOutputPathname, ".bin");
                    }
                }
            }
        }
    }

    return pOutputPathname;
}

/**
* @fn static char *myGetFileName( char *pFilePath)
* @brief extrat the file name in pFilePath
*
* @param[in]        pFilePath
* 
* @return file name if success, or pFilePath in case of error
*/
static char *myGetFileName( char *pFilePath)
{
    char *pFileName = NULL;

    if (pFilePath)
    {
        pFileName = (char *)strrchr( pFilePath, '\\');
        if (!pFileName)
        {
            pFileName = (char *)strrchr( pFilePath, '/');
        }
    }
    return pFileName ? pFileName + 1 : pFilePath;
}

/**
* @fn static char *myBuidAboutString( void)
* @brief built the about string
*
* @return the about string if success, or NULL in case of error
*/
static char *myBuidAboutString( void)
{
    char                filePath[MAX_PATH];
    unsigned long       ulSize;
    unsigned long       ulHandle;
    VS_FIXEDFILEINFO   *pfileInfo = NULL;
    UINT                uLen;
    char               *pResourceBuffer = NULL;
    char               *pApplicationName;
    char               *pLegalCopyright = NULL;
    char               *pVersion = NULL;
    char               *pfileVersionInformation = NULL;

    ulSize = GetModuleFileNameA( NULL, filePath, MAX_PATH);
    if (ulSize == 0 || ulSize == MAX_PATH)
    {
        printf("Error lors de la récupération du chemin du fichier.\n");
        return pfileVersionInformation;
    }

    // ulHandle is set to zero
    ulSize = GetFileVersionInfoSizeA( filePath, &ulHandle);
    if (ulSize == 0)
    {
        printf("Error lors de la récupération de la taille des informations de version.\n");
        return pfileVersionInformation;
    }

    pResourceBuffer = (char *)malloc( ulSize);
    if (!pResourceBuffer)
    {
        printf( "Error d'allocation de mémoire.\n");
        return pfileVersionInformation;
    }

    ulHandle = 666666666;     // random value to avoid stupîd warning on ulHandle if equal to zero
    if (!GetFileVersionInfoA( filePath, ulHandle, ulSize, pResourceBuffer))
    {
        printf("Error lors de la récupération des informations de version.\n");
        free (pResourceBuffer);
        return pfileVersionInformation;
    }

    pApplicationName = myGetFileName( filePath);
    if (pApplicationName)
    {
        ulSize = (unsigned long)strlen(pApplicationName) + 16;
    }
    else
    {
        ulSize = 16;
    }
    // Retrieve the LegalCopyright field
    if (!VerQueryValueA( pResourceBuffer, "\\StringFileInfo\\040904b0\\LegalCopyright", (LPVOID *)&pLegalCopyright, &uLen))
    {
        printf("Erreur lors de la récupération du champ LegalCopyright.\n");
    }
    ulSize += uLen;
    // Retrieve the ProductVersion field
    if (!VerQueryValueA( pResourceBuffer, "\\StringFileInfo\\040904b0\\ProductVersion", (LPVOID *)&pVersion, &uLen))
    {
        printf("Erreur lors de la récupération du champ LegalCopyright.\n");
    }

    ulSize += uLen;
    pfileVersionInformation = (char *)malloc( ulSize);
    if (!pfileVersionInformation)
    {
        printf("Erreur d'allocation de mémoire.\n");
        return NULL;
    }

    sprintf( pfileVersionInformation, "%s v%s, %s", pApplicationName ? pApplicationName : "", pVersion, pLegalCopyright);

    free( pResourceBuffer);

    return pfileVersionInformation;
}

/**
* @fn int main( int argc, char *argv[])
* @brief The entry point of the software
*
* @param[in]        argc
* @param[in]        argv[]
*
* @return 0 if success, other values in case of error
*/
int main(int argc, char* argv[])
{
    ConvmArguments      context = { NULL, NULL, 0 };
    char               *pfullOutputFilename = NULL;
    char               *pInputFileData = NULL;
    char               *pOutputFileData = NULL;
    const char         *pEndString = NULL;
    char               *pAboutString = NULL;
    int                 iCommand = NONE;
    unsigned int        uInputFileSize = 0;
    unsigned short int  uDataSize = 0;
    unsigned char       uFullFilename;
    unsigned short int  uNumLines = 0;
    unsigned short int  uResultDataSize;

    // Message Information Get the verion and copyright from the file datam.rc
    pAboutString = myBuidAboutString();

    if (argc < 3)
    {
        if ((argc == 2) && (argv[1]))
        {
            if ((!strcmp((const char*)argv[1], "-help")) || (!strcmp((const char*)argv[1], "-h")) || (!strcmp((const char*)argv[1], "-?")))
            {
                usage( pAboutString);
            }
            else if ((!strcmp((const char*)argv[1], "-version")) || (!strcmp((const char*)argv[1], "-v")))
            {
                (void )printf( "\n%s\n\n", pAboutString);
            }

            if (pAboutString)
            {
                free(pAboutString);
            }
            return 0;
        }
        else
        {
            usage( pAboutString);
            (void )printf( "\n");
            if ( pAboutString)
            {
                free( pAboutString);
            }
            exitOnError( (char *)"not enough parameters", NULL, NULL, 1);
        }
    }

    (void )printf( "\n%s\n\n", pAboutString);
    if (pAboutString)
    {
        free( pAboutString);
        pAboutString = NULL;
    }

    iCommand = parseArguments( argc, argv, &context);

    /*
    (void )printf("\n");
#ifdef _WIN32
    (void )printf("x86 mode 32 bits\n\n");
#else
    (void )printf("x64 mode 64 bits\n\n");
#endif

    (void )printf("\n");
    */
    //pfullFilename = parseSpaceChar( &pfullFilename);
    //printf("");

    if (!context.pFullFilename)
    {
        exitOnError( (char *)"bad parameters", NULL, NULL, 1);
    }

    uInputFileSize = getMyFileSize( context.pFullFilename);
    if (uInputFileSize < 4)
    {
        exitOnError( (char *)"imput file is empty", NULL, NULL, 1);
    }

    (void )printf( "\n");
    pInputFileData = readFileToMemory( context.pFullFilename);   // input file is in memory
    if ( (pInputFileData) && (context.pFullFilename) )
    {
        if (iCommand == DATALST)
        {
            uFullFilename = (unsigned char )strlen( context.pFullFilename) - 4;
            if ( (uFullFilename > 4) && (context.pFullFilename[uFullFilename] == '.') )
            {
                pEndString = &context.pFullFilename[strlen( context.pFullFilename) - 3];
                if (pEndString)
                {
                    if ( ! strcmp( pEndString, "txt"))
                    {
                        findNumLinesLst( context.pFullFilename, &uNumLines, &uDataSize);
                    }
                }
                else
                {
                    exitOnError( (char *)"imput file extension is not supported", NULL, NULL, 1);
                }
            }
            else
            {
                exitOnError( (char *)"imput file extension is not supported", NULL, NULL, 1);
            }

            if (uNumLines > 0)
            {
                // printf( "\nsizeof( unsigned short int)=%d\n", (unsigned int )sizeof( unsigned short int));
                // printf( "lines=%d : header=%d : data=%d\n", uNumLines, (unsigned short int )(uNumLines * (unsigned short int )sizeof( unsigned short int)), uDataSize);
                uDataSize += (unsigned short int )sizeof( unsigned short int) + (unsigned short int )(uNumLines * (unsigned short int )sizeof( unsigned short int));
                uDataSize += 64;   // 256 is an extension for memory security

                pOutputFileData = (char *)calloc(1, uDataSize + 64);
                if (pOutputFileData)
                {
                    uResultDataSize = str2data( context.pFullFilename, uNumLines, pOutputFileData, uDataSize);

                    if (uResultDataSize <= uDataSize)
                    {
                        // display a string from the file for debug
                        // (void )mySentence( pOutputFileData, 19);
                        // (void )mySentence( pOutputFileData, 39);
                        // (void )mySentence( pOutputFileData, 49);

                        pfullOutputFilename = createOutputPathname( context.pFullFilename, context.pOutputPathname, iCommand);
                        if (pfullOutputFilename)
                        {
                            if (context.pCheckEnumPathname)
                            {
                                unsigned int uOjectIndex = myComputeIndexOf( context.pCheckEnumPathname, (char *)"eObjects,");
                                if (uOjectIndex == 0)
                                {
                                    exitOnError( (char *)"enum file is empty", NULL, context.pCheckEnumPathname, 1);
                                }
                                else
                                {
                                    if (strcmp( mySentence( pOutputFileData, (unsigned short)uOjectIndex), "Objets") != 0)
                                    {
                                        exitOnError( (char *)"Synchro between enum and string table is BAD", NULL, pfullOutputFilename, 5);
                                    }
                                }
                            }

                            if (writeFileFromMemory( pfullOutputFilename, pOutputFileData, uResultDataSize))
                            {
                                exitOnError( (char *)"failed to write output file", NULL, pfullOutputFilename, 4);
                            }
                            free( pfullOutputFilename);
                        }
                        free( pOutputFileData);
                    }
                    else
                    {
                        exitOnError( (char *)"Out of memory", NULL, NULL, 1);
                    }
                }
            }
        }
        else if (iCommand == CSVTOC)
        {
            uInputFileSize = getMyFileSize( context.pFullFilename);

            (void )printf("\n");
            pInputFileData = readFileToMemory( context.pFullFilename);   // input file is in memory
            if ( (pInputFileData) && (uInputFileSize > 100) )
            {
                exitOnError( (char *)"Not implemented", NULL, NULL, 6);
            }
        }
        else if (iCommand == DUMP)
        {
            (void )printf( "\n");
            exitOnError( (char *)"Not implemented", NULL, NULL, 6);
        }
        else
        {
            // For next command...
            ;
        }
    }
    (void )printf("\n");

    if (context.pFullFilename)
    {
        free( context.pFullFilename);
    }

    (void )printf(" Work is done with success\n");
    return 0;
}
