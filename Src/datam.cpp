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
 * where to found the windows error code :
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
 * -sltb
 * +LOwer +chkEnu ..\..\..\Iron_Lord\main\pathname.h -SLTB ..\..\..\Iron_Lord\main\datatext.fr.txt ..\..\..\Iron_Lord\dessin\
 * +chkenu ..\..\..\Iron_Lord\main\pathname.h -sltb ..\..\..\Iron_Lord\main\datatext.fr.txt ..\..\..\Iron_Lord\dessin\
 * +chkenu ..\..\..\Iron_Lord\main\pathname.h -sltb ..\..\..\Iron_Lord\main\datatext.fr.txt ..\..\..\Iron_Lord\dessin\
 * 
 * -getstr
 * -getstr 400 ..\..\..\Iron_Lord\dessin\datatext.fr.bin
 * -getstr 400 ..\..\..\Iron_Lord\dessin\datatext.en.bin
 */


/*
 * @fn static void datam_usage( char *pAboutString)
 * @brief Show how to use soft
 *
 * @param[in]        pAboutString
 */
static void datam_usage( char *pAboutString)
{
    if (pAboutString)
    {
        (void )printf( "\n%s\n\n", pAboutString);
    }
    (void )printf( "Usage: datam <option> \"<filespec>\" <convmspec> \"<number>\" \"<filespec>\" \"<output folder>\"\n");

    (void )printf( "\n  <option> is one of the following\n");
    (void )printf( "   +lower       - the output file name is in lower case\n");
    (void )printf( "   +chkenu      - Check synchro enum eSentence_t and list of string\n");
    (void )printf( "                  <filespec> the path to file with enum eSentence_t definition\n");

    (void )printf( "\n  <convmspec> is one of the following\n");
    (void )printf( "   -dump        - dump content of a supported file format\n");
    (void )printf( "   -sltb        - file with strings list to binary file\n");
    (void )printf( "   -ctbl        - create table from .cvs to .c\n");
    (void )printf( "   -getstr      - get string at index in binary file\n");

    (void )printf( "\n  <filespec> file extension could be :\n");
    (void )printf( "   -dump          'any'\n");

    (void )printf( "\n  <filespec> :\n");
    (void )printf( "   -sltb          'pathname to txt file'\n");
    (void )printf( "  <output folder> :\n");
    (void )printf( "   -sltb          'pathname to folder destination'\n");

    (void )printf( "\n  <number> could be:\n");
    (void )printf( "   -getstr        '0 .. 999'\n");
    (void )printf( "  <filespec> :\n");
    (void )printf( "   -getstr        'pathname to bin file'\n");
}

/**
* @fn static int checkFileExtension( char *pPathFilename, eCommand_t eCommand)
* @brief Check the 4 last char of file name to be .scr, .shr, .pnt, .pic
*
* @param[in]        pPathFilename
* @param[in]        eCommand
*
* @return 0 if ok, or 3 on error but never return the software exit
*/
static int checkFileExtension( char *pPathFilename, eCommand_t eCommand)
{
    const char  *pEndString = NULL;
    char        *pRunning = NULL;
    char        *pLastPointChar = NULL;
    int          iError = 0;
    BOOL         bErrorNoExt = FALSE;
    BOOL         bError = FALSE;
    BOOL         bErrorCmd = FALSE;
    BOOL         bErrorExt = FALSE;
    const char  *pCmdtext[] = { "none", "-dump", "-sltb", "-ctbl", "-getstr"};

    if (eCommand == eDUMP)
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
                    if (eCommand == eDATALST)
                    {
                        if (strcmp( (const char *)pLastPointChar, "txt"))
                        {
                            bError = TRUE;
                        }
                    }
                    else if (eCommand == eCSVTOC)
                    {
                        if (strcmp( (const char *)pLastPointChar, "csv"))
                        {
                            bError = TRUE;
                        }
                    }
                    else if (eCommand == eGETSTR)
                    {
                        if (strcmp( (const char*)pLastPointChar, "bin"))
                        {
                            bError = TRUE;
                        }
                    }
                    else
                    {
                        bError = TRUE;
                    }
                }
                else
                {
                    bError = TRUE;
                }

                if (bError)
                {
                    exitOnError( (char *)"file extension not supported", (char *)pCmdtext[eCommand], (char *)pEndString, 3);
                    iError = 1;
                }

                if (bErrorCmd)
                {
                    exitOnError( (char *)"command already done on this file", (char *)pCmdtext[eCommand], (char *)pEndString, 3);
                    iError = 1;
                }

                if (bErrorExt)
                {
                    exitOnError( (char *)"file not compatible with command", (char *)pCmdtext[eCommand], (char *)pEndString, 5);
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

    // (void )printf( "before input path is : %s\n", context.pFullFilename);
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
* @fn static void getPathnameFromCmd( char *pInputPathname, char **pOutputPathname, BOOL bLowerCase, eCommand_t eCommand, BOOL bIsAFile)
* @brief get pathname clean it and store it
*
* @param[in]        pInputPathname      pathname of enum, input pathname in utf8, output pathname in binary
* @param[in,out]    pOutputPathname     handle to cleaned pInputPathname
* @param[in]        bLowerCase          convert pathname in lower case
* @param[in]        eCommand            num of command
* @param[in]        bIsAFile            True for a file, False for a folder
*/
static void getPathnameFromCmd( char *pInputPathname, char **pOutputPathname, BOOL bLowerCase, eCommand_t eCommand, BOOL bIsAFile)
{
    char    *pDuplicateString = NULL;

    if ((pInputPathname) && (pOutputPathname))
    {
        if ( ! *pOutputPathname)
        {
            *pOutputPathname = (char *)calloc( 1, _MAX_PATH + _MAX_PATH);
            if ( ! *pOutputPathname)
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
                if (pathFileExists( (const char *)*pOutputPathname))
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

                        if (eCommand)
                        {
                            if ( ! checkFileExtension( *pOutputPathname, eCommand))
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
                                exitOnError( (char *)"File does not exist", *pOutputPathname, NULL, 2);
                            }
                        }
                    }
                }
                else
                {
                    exitOnError( (char *)"File does not exist", *pOutputPathname, NULL, 2);
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
* @fn static eCommand_t parseArguments( int argc, char *argv[], ConvmArguments *pContext)
* @brief parse the arguments and check it
*
* @param[in]        argc
* @param[in]        argv[]
* @param[in,out]    pContext
*
* @return the convmspec value and filled pContext structure, on error the software exit
*/
static eCommand_t parseArguments( int argc, char *argv[], ConvmArguments *pContext)
{
    unsigned int    uIndex = 0;
    unsigned int    uLoop = 0;
    eCommand_t      eCommand = eEMPTY;
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
            if ( ! strcmp( (const char *)argv[uIndex], "-dump"))
            {
                eCommand = eDUMP;
            }
            else if ( ! strcmp( (const char *)argv[uIndex], "-sltb"))    /* strings list to binary file */
            {
                eCommand = eDATALST;
            }
            else if ( ! strcmp( (const char *)argv[uIndex], "-ctbl"))    /* strings list to binary file */
            {
                eCommand = eCSVTOC;
            }
            else if ( ! strcmp((const char *)argv[uIndex], "-getstr"))    /* strings list to binary file */
            {
                eCommand = eGETSTR;
                uIndex++;
                pContext->iTabColumns = atoi(argv[uIndex]);
            }
            else
            {
                exitOnError( (char *)"Illegal convmspec parameter", argv[uIndex], NULL, 2);
            }
        }
        else if (argv[uIndex][0] == '+')    // option, check
        {
            if ( ! strcmp( (const char *)argv[uIndex], "+lower"))
            {
                bLowerCase = TRUE;
            }
            else if ( ! strcmp( (const char *)argv[uIndex], "+chkenu"))
            {
                if ( ! pContext->pCheckEnumPathname)
                {
                    uIndex++;
                    getPathnameFromCmd( argv[uIndex], &pContext->pCheckEnumPathname, bLowerCase, eEMPTY, TRUE);
                }
            }
            else
            {
                exitOnError( (char *)"Illegal option parameter", argv[uIndex], NULL, 2);
            }
        }
        else
        {
            if ( ! pContext->pFullFilename)
            {
                getPathnameFromCmd( argv[uIndex], &pContext->pFullFilename, bLowerCase, eCommand, TRUE);
            }
            else if ( ( ! pContext->pOutputPathname) && (eCommand != eGETSTR) )
            {
                getPathnameFromCmd( argv[uIndex], &pContext->pOutputPathname, bLowerCase, eCommand, FALSE);
            }
            else
            {
                exitOnError( (char *)"too many parameters", NULL, argv[uIndex], 1);
            }
        }
    }

    if ( (eCommand == eEMPTY) ||
         (pContext->pFullFilename == NULL) ||
         ( (eCommand != eGETSTR) && (pContext->pOutputPathname == NULL)) )
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
        if (eCommand == eEMPTY)
        {
            exitOnError((char*)"Parameters not understood", NULL, NULL, 1);
        }
        else
        {
            exitOnError((char*)"Missing parameters", NULL, NULL, 1);
        }
    }

    return eCommand;
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
            cmd_exe_path_len = strlen( pCcmdExePath);

            tmpCommandLen = strlen( pCommand);
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
    const char  *pEndString;
    char        *pDuplicateString = NULL;
    char        *pShortPathname = NULL;
    char        *ptempFilename = NULL;
    int          iError;

    if ( (pContext) && (pfullOutputFilename) )
    {
        pShortPathname = (char *)calloc( 1, strlen(pfullOutputFilename) + 1);
        pDuplicateString = (char *)calloc( 1, _MAX_PATH + _MAX_PATH);
        if (( ! pDuplicateString) || ( ! pShortPathname))
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
                    free( pShortPathname);
                    pShortPathname = strdup(pfullOutputFilename);
                }
                else
                {
                    if ( ! strlen( pShortPathname))
                    {
                        free( pShortPathname);
                        pShortPathname = strdup( pfullOutputFilename);
                    }
                    else
                    {
                        pShortPathname = strcat( pShortPathname, getFileName(pfullOutputFilename));
                    }
                }
                if (pShortPathname)
                {
                    (void )printf( "DATAM: to   = %s\n", pShortPathname);
                }
                free( ptempFilename);
            }
            else
            {
                free( pShortPathname);
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
* @fn static char *createOutputPathname( char *pFullFilename, char *pOutPathname, eCommand_t eCommand)
* @brief alloc and change the file extension
*
* @param[in]        pFullFilename
* @param[in]        pOutPathname
* @param[in]        iCommand
*
* @return a new pointer to the new pathname
*/
static char *createOutputPathname( char *pFullFilename, char *pOutPathname, eCommand_t eCommand)
{
    char            *pOutputPathname = NULL;
    const char      *pEndString = NULL;
    size_t           uOutputPathnameLen;

    if ( (pFullFilename) && (eCommand != eEMPTY) )
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
                }
                else
                {
                    (void )strncpy_s( pOutputPathname, uOutputPathnameLen, pFullFilename, strlen( (const char *)pFullFilename));
                    //(void )strncpy( pOutputPathname, pfullFilename, uOutputPathnameLen);
                }

                // Change the extention of the output file
                if (pOutputPathname[strlen( pOutputPathname) - 4] == '.')
                {
                    pOutputPathname[strlen( pOutputPathname) - 3] = 'b';
                    pOutputPathname[strlen( pOutputPathname) - 2] = 'i';
                    pOutputPathname[strlen( pOutputPathname) - 1] = 'n';
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
        if ( ! pFileName)
        {
            pFileName = (char *)strrchr( pFilePath, '/');
        }
    }
    return pFileName ? pFileName + 1 : pFilePath;
}

/**
* @fn static char *myBuidAboutString( void)
* @brief built the about string using resources fields LegalCopyright and ProductVersion
*
* @return the about string if success, or NULL in case of error
*/
static char *myBuidAboutString( void)
{
    char                filePath[MAX_PATH];
    unsigned long       ulSize;
    unsigned long       ulHandle;
    UINT                uLen;
    char               *pResourceBuffer = NULL;
    char               *pApplicationName;
    char               *pLegalCopyright = NULL;
    char               *pVersion = NULL;
    char               *pfileVersionInformation = NULL;

    ulSize = GetModuleFileNameA( NULL, filePath, MAX_PATH);
    if (ulSize == 0 || ulSize == MAX_PATH)
    {
        (void )printf( "Error lors de la récupération du chemin du fichier.\n");
    }
    else
    {
        // ulHandle is set to zero
        ulSize = GetFileVersionInfoSizeA( filePath, &ulHandle);
        if (ulSize == 0)
        {
            (void )printf( "Error lors de la récupération de la taille des informations de version.\n");
        }
        else
        {
            pResourceBuffer = (char *)malloc( ulSize);
            if ( ! pResourceBuffer)
            {
                (void )printf( "Error d'allocation de mémoire.\n");
            }
            else
            {
                ulHandle = 0;
                if ( ! GetFileVersionInfoA( (const char *)filePath, ulHandle, ulSize, pResourceBuffer))
                {
                    (void )printf( "Error lors de la récupération des informations de version.\n");
                }
                else
                {
                    pApplicationName = myGetFileName( filePath);
                    if (pApplicationName)
                    {
                        ulSize = (unsigned long)strlen( pApplicationName) + 16;
                    }
                    else
                    {
                        ulSize = 16;
                    }
                    // Retrieve the LegalCopyright field
                    if ( ! VerQueryValueA( pResourceBuffer, "\\StringFileInfo\\040904b0\\LegalCopyright", (LPVOID *)&pLegalCopyright, &uLen))
                    {
                        (void )printf( "Erreur lors de la récupération du champ LegalCopyright.\n");
                    }
                    else
                    {
                        ulSize += uLen;
                        // Retrieve the ProductVersion field
                        if ( ! VerQueryValueA( pResourceBuffer, "\\StringFileInfo\\040904b0\\ProductVersion", (LPVOID *)&pVersion, &uLen))
                        {
                            (void )printf( "Erreur lors de la récupération du champ LegalCopyright.\n");
                        }
                        else
                        {
                            ulSize += uLen;
                            pfileVersionInformation = (char *)malloc( ulSize);
                            if ( ! pfileVersionInformation)
                            {
                                (void )printf( "Erreur d'allocation de mémoire.\n");
                            }
                            else
                            { 
                                (void )sprintf( pfileVersionInformation, "%s v%s, %s", pApplicationName ? pApplicationName : "", pVersion, pLegalCopyright);
                            }
                        }
                    }
                }
                free( pResourceBuffer);
            }
        }
    }

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
int main( int argc, char* argv[])
{
    ConvmArguments      context = { NULL, NULL, 0 };
    char               *pfullOutputFilename = NULL;
    char               *pInputFileData = NULL;
    char               *pOutputFileData = NULL;
    const char         *pEndString = NULL;
    char               *pAboutString = NULL;
    eCommand_t          eCommand = eEMPTY;
    unsigned int        uInputFileSize = 0;
    unsigned short      uDataSize = 0;
    unsigned char       uFullFilename;
    unsigned short      uNumLines = 0;
    unsigned short      uResultDataSize;
    unsigned int        uOjectIndex;

    // Message Information Get the verion and copyright from the file datam.rc
    pAboutString = myBuidAboutString();

    if (argc < 3)
    {
        if ((argc == 2) && (argv[1]))
        {
            if (( ! strcmp( (const char *)argv[1], "-help")) || ( ! strcmp( (const char *)argv[1], "-h")) || ( ! strcmp( (const char *)argv[1], "-?")))
            {
                datam_usage( pAboutString);
            }
            else if (( ! strcmp((const char*)argv[1], "-version")) || ( ! strcmp((const char*)argv[1], "-v")))
            {
                (void )printf( "\n%s\n\n", pAboutString);
            }

            if (pAboutString)
            {
                free( pAboutString);
                pAboutString = NULL;
            }
            return 0;
        }
        else
        {
            datam_usage( pAboutString);
            (void )printf( "\n");
            if ( pAboutString)
            {
                free( pAboutString);
                pAboutString = NULL;
            }
            exitOnError( (char *)"not enough parameters", NULL, NULL, 1);
        }
    }

    if (pAboutString)
    {
        (void )printf( "\n%s\n\n", pAboutString);

        free( pAboutString);
        pAboutString = NULL;
    }

    eCommand = parseArguments( argc, argv, &context);

    /*
    (void )printf( "\n");
#ifdef _WIN32
    (void )printf( "x86 mode 32 bits\n\n");
#else
    (void )printf( "x64 mode 64 bits\n\n");
#endif

    (void )printf( "\n");
    */
    //pfullFilename = parseSpaceChar( &pfullFilename);
    //(void )printf( "");

    if ( ! context.pFullFilename)
    {
        exitOnError( (char *)"bad parameters", NULL, NULL, 1);
    }

    uInputFileSize = getMyFileSize( context.pFullFilename);
    if (uInputFileSize < 4)
    {
        exitOnError( (char *)"imput file is empty", NULL, NULL, 1);
    }

    pInputFileData = readFileToMemory( context.pFullFilename);   // input file is in memory
    if ( (pInputFileData) && (context.pFullFilename) )
    {
        if (eCommand == eDATALST)
        {
            uFullFilename = (unsigned char )strlen( context.pFullFilename) - 4;
            if ( (uFullFilename > 4) && (context.pFullFilename[uFullFilename] == '.') )
            {
                pEndString = &context.pFullFilename[strlen( context.pFullFilename) - 3];
                if ( ! strcmp( pEndString, "txt"))
                {
                    findNumLinesLst( context.pFullFilename, &uNumLines, &uDataSize);
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

                pOutputFileData = (char *)calloc( 1, uDataSize + 64);
                if (pOutputFileData)
                {
                    uResultDataSize = str2data( context.pFullFilename, uNumLines, pOutputFileData, uDataSize);

                    if (uResultDataSize <= uDataSize)
                    {
                        // display a string from the file for debug
                        // (void )mySentence( pOutputFileData, 19);
                        // (void )mySentence( pOutputFileData, 39);
                        // (void )mySentence( pOutputFileData, 49);

                        pfullOutputFilename = createOutputPathname( context.pFullFilename, context.pOutputPathname, eCommand);
                        if (pfullOutputFilename)
                        {
                            if (context.pCheckEnumPathname)
                            {
                                uOjectIndex = myComputeIndexOf( context.pCheckEnumPathname, (char *)"eObjects,");
                                if (uOjectIndex == 0)
                                {
                                    exitOnError( (char *)"enum file is empty", NULL, context.pCheckEnumPathname, 1);
                                }
                                else
                                {
                                    if (strcmp( mySentence( pOutputFileData, (unsigned short )uOjectIndex), "Objets") != 0)
                                    {
                                        exitOnError( (char *)"Synchro between enum and string table is BAD", NULL, pfullOutputFilename, 5);
                                    }
                                }
                            }

                            if (writeFileFromMemory( pfullOutputFilename, pOutputFileData, (unsigned int)uResultDataSize))
                            {
                                exitOnError( (char *)"failed to write output file", NULL, pfullOutputFilename, 4);
                            }
                            free( pfullOutputFilename);
                        }
                    }
                    else
                    {
                        exitOnError( (char *)"Out of memory", NULL, NULL, 1);
                    }
                    free( pOutputFileData);
                }
            }
        }
        else if (eCommand == eCSVTOC)
        {
            if ( (pInputFileData) && (uInputFileSize > 100) )
            {
                exitOnError( (char *)"Not implemented", NULL, NULL, 6);
            }
        }
        else if (eCommand == eDUMP)
        {
            exitOnError( (char *)"Not implemented", NULL, NULL, 6);
        }
        else if (eCommand == eGETSTR)
        {
            uFullFilename = (unsigned char )strlen( context.pFullFilename) - 4;
            if ( (uFullFilename > 4) && (context.pFullFilename[uFullFilename] == '.') )
            {
                pEndString = &context.pFullFilename[strlen( context.pFullFilename) - 3];
                if ( ! strcmp( pEndString, "bin"))
                {
                    if ( (context.iTabColumns >= 0) && (context.iTabColumns < 1000) )
                    {
                        (void )printf( "\nString at index %d is :\n%s\n", context.iTabColumns, mySentence( pInputFileData, (unsigned short )context.iTabColumns));
                    }
                    else
                    {
                        exitOnError( (char *)"Bad index", NULL, NULL, 1);
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
        }
        else
        {
            // For next command...
            ;
        }
    }
    //(void )printf("\n");

    if (context.pFullFilename)
    {
        free( context.pFullFilename);
    }

    if (pInputFileData)
    {
        free( pInputFileData);
    }

    //(void )printf(" Work is done with success\n");
    return 0;
}
