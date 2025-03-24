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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <conio.h>
#include <malloc.h>

#include "conv.h"
#include "str2data.h"

// _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/**
 * @fn void findNumLinesLst( char *pInputFile, unsigned short *pInputFile, unsigned short *puDataSize)
 *
 * @brief
 *
 * @param[in]       pInputFile :
 * @param[in,out]   pInputFile :
 * @param[in,out]   puDataSize :
 */
void findNumLinesLst( char *pInputFile, unsigned short *puNumLines, unsigned short *puDataSize)
{
    FILE                   *pFile = NULL;
    char                    line[512] = {0};
    unsigned short          uLineSize;

    if ( (pInputFile) && (puNumLines) && (puDataSize) )
    {
        *puNumLines = 0;
        *puDataSize = 0;

        pFile = fopen( pInputFile, "r");
        if (pFile != NULL)
        {
            while (fgets( line, sizeof(line), pFile) )
            {
                if ( (line[0] != '/') && (line[0] != '\n') )
                {
                    *puNumLines += 1;
                    uLineSize = (unsigned short int )strlen( line);
                    if (line[uLineSize - 1] == '\n')
                    {
                        // (void )printf( "#%02d : %d : %s", *puNumLines, *puDataSize, line);
                        *puDataSize += uLineSize;
                    }
                }
            }

            (void )fclose( pFile);
        }
    }
    return;
}

/**
 * @fn char *mySentence( pTheGameStruct pGameContext, unsigned short uNumber)
 * 
 * @brief debug function to test the conversion
 * 
 * @param[in,out]   pGameContext : game context values
 * @param[in]       iNumber : enum eSentence (the index in the array)
 *
 * @return char * pointer to the string
 */
char *mySentence( char *pOutputFileData, unsigned short uNumber)
{
    short int           *pDataPic;
    char                *pDataStr = NULL;
    unsigned short       uMaxPic;

    if (pOutputFileData)
    {
        uMaxPic = *(unsigned short int *)pOutputFileData;
        pDataPic = (short int *)pOutputFileData;  // handle lock after create

        if (uNumber <= uMaxPic)
        {
            pDataPic++;
            pDataStr = (char *)pDataPic;
            pDataStr += ((uMaxPic * sizeof( short int)) + (unsigned short int )pDataPic[uNumber]);
        }
    }
    // (void )printf( " result : %u : %s\n", uNumber, pDataStr);

    return pDataStr;
}

/**
 * @fn unsigned short str2data( char *pInputFileName, unsigned short uNumLines, char *pOutputFileData, unsigned short uDataSize)
 *
 * @brief 
 *
 * @param[in]       pInputFileName : 
 * @param[in]       uNumLines :
 * @param[in,out]   pOutputFileData :
 * @param[in]       uDataSize :
 *
 * @return char * pointer to the string
 */
unsigned short str2data( char *pInputFileName, unsigned short uNumLines, char *pOutputFileData, unsigned short uDataSize)
{
    FILE               *pFile = NULL;
    char               *pRunning = NULL;
    unsigned short     *puTable;
    char                line[512] = {0};
    unsigned short      uLineSize;
    unsigned short      uOffset = 0;
    unsigned short      uLoop;
    unsigned short      uNumberOfCharRemoved;
    unsigned short      uOutputDataSize = 0;

    if ( (pInputFileName) && (pOutputFileData) && (uNumLines) && (uDataSize) )
    {
        puTable = (unsigned short int *)pOutputFileData;
        *puTable = uNumLines;
        puTable++;
        pRunning = (char *)puTable + (unsigned short int )(uNumLines * (unsigned short int )sizeof( unsigned short int));

        pFile = fopen( pInputFileName, "rb, ccs=UTF-8");
        if (pFile != NULL)
        {
            while (fgets( line, sizeof( line), pFile))
            {
                if ( ((line[0] != '/') && (line[1] != '*')) && (line[0] != '\n') )      // not a comment line
                {
                    uLineSize = (unsigned short int )strlen( line);
                    if (uLineSize > 0)
                    {
                        if (uLineSize < 512)
                        {
                            if ( (uLineSize == 2) && (line[0] == '\r') && (line[1] == '\n') )
                            {
                                // Drop second empty line
                                line[uLineSize - 1] = '\0';
                            }

                            if ( (line[uLineSize - 1] == '\n') && (line[uLineSize - 2] == '\r') )
                            {
                                line[uLineSize - 1] = '\0';
                                line[uLineSize - 2] = '\0';
                                uLineSize--;
                                uNumberOfCharRemoved = 0;
                                for (uLoop = 0; uLoop < uLineSize; uLoop++)
                                {
                                    if ( (line[uLoop] == '\\') && (line[uLoop + 1] == 'n') )
                                    {
                                        *pRunning = 10;
                                        uLoop++;
                                        uNumberOfCharRemoved++;
                                    }
                                    else if ( (line[uLoop] == '\\') && (line[uLoop + 1] == 't') )
                                    {
                                        *pRunning = 9;
                                        uLoop++;
                                        uNumberOfCharRemoved++;
                                    }
                                    else if (line[uLoop] == '…')    // replace 3 points by slash to be compatible with picture letters.bmp 
                                    {
                                        *pRunning = '/';
                                    }
                                    else
                                    {
                                        *pRunning = line[uLoop];
                                    }
                                    pRunning++;
                                }

                                *puTable = uOffset;
                                puTable++;
                                uOffset += (uLineSize - uNumberOfCharRemoved);
                            }
                        }
                        else
                        {
                            (void )printf( "Out of memory : 'line' buffer is too small\n");
                            uOutputDataSize = 0;
                            break;
                        }
                    }
                }
            }

            (void )fclose( pFile);

            // validate len of data
            uOutputDataSize = 0;
            if (pRunning > pOutputFileData)
            {
                if ( (pRunning - pOutputFileData) <= 0x0000FFFF)
                {
                    uOutputDataSize = (unsigned short int )(pRunning - pOutputFileData);
                    // printf("\nTotal by pointer = %u  uOutputDataSize= %u\n", uTotal, uOutputDataSize);
                }
            }

            (void )printf( "\n");
        }
    }

    return uOutputDataSize;
}

/**
 * @fn static void findNumLinesHeader( char *pInputFile, unsigned short int *puNumLines, unsigned short int *puDataSize)
 * @brief
 *
 * @param[in]       pInputFile :
 * @param[in,out]   puNumLines :
 * @param[in,out]   puDataSize :
 */
static void findNumLinesHeader( char *pInputFile, unsigned short int *puNumLines, unsigned short int *puDataSize)
{
    FILE               *pFile = NULL;
    char                line[512] = {0};
    unsigned short int  uLineSize;

    *puNumLines = 0;
    *puDataSize = 0;

    if (pInputFile)
    {
        pFile = fopen( pInputFile, "r");
        if (pFile != NULL)
        {
            while (fgets( line, sizeof( line), pFile))
            {
                if (line[0] == 's')
                {
                    char *pPosition = strchr( line, '"');
                    char *pr_position = strrchr( line, '"');

                    if ( ( (pPosition) && (pr_position) ) && (pr_position > pPosition) )
                    {
                        // uLineSize = (unsigned short int )(pr_position - pPosition);

                        *puNumLines += 1;
                        uLineSize = (unsigned short )strlen( line);
                        if ( (line[uLineSize - 1] == '\n') && (line[uLineSize - 2] == ',') && (line[uLineSize - 3] == '"') )
                        {
                            // printf("#%02d : %d : %s", *puNumLines, *puDataSize, line);
                            uLineSize -= 3;
                            *puDataSize += uLineSize;
                        }
                    }
                }
            }

            (void )fclose( pFile);
        }
    }
}

/**
* @fn unsigned short myComputeIndexOf( char *pCheckEnumPathname, char *pEnumElementName)
* @brief parse a text file to found eSentence_t and index of element eObjects
* 
* @param[in]       pCheckEnumPathname :
* @param[in]       pEnumElementName :
* 
 * @return 0 is error or index of eObjects
*/
unsigned short myComputeIndexOf( char *pCheckEnumPathname, char *pEnumElementName)
{
    FILE            *pFichier;
    char            *pFound;
    char             buffer[256];
    unsigned int     uLigne = 0;
    unsigned int     uIndex;
    unsigned short   uCompteur = 0;
    unsigned short   uIndexObjects = 0;

    pFichier = fopen( pCheckEnumPathname, "r");
    if (pFichier == NULL)
    {
        (void )printf( "Erreur : Can't open file %s\n", pCheckEnumPathname);
        return uIndexObjects;
    }

    // find the begin of enum eSentence_t
    while (fgets( buffer, sizeof( buffer), pFichier) != NULL)
    {
        pFound = strstr( buffer, "eSentence_t");
        if (pFound != NULL)
        {
            //(void )printf( "The string has been found in line : %s", buffer);
            break;
        }
    }
    // jump to the declaration of the enum eSentence_t
    (void )fgets( buffer, sizeof( buffer), pFichier);
    (void )fgets( buffer, sizeof( buffer), pFichier);
    (void )fgets( buffer, sizeof( buffer), pFichier);
    pFound = strstr( buffer, "typedef enum {");
    if (pFound == NULL)
    {
        (void )printf( "Erreur : The begin of eSentence_t declaration not found");
        return uIndexObjects;
    }

    // parse the enum eSentence_t
    while (fgets( buffer, sizeof( buffer), pFichier) != NULL)
    {
        uLigne++;
        pFound = strstr(buffer, "} eSentence_t;");
        if (pFound != NULL)
        {
            break;
        }
        
        pFound = strstr( buffer, pEnumElementName);
        if (pFound != NULL)
        {
            uIndexObjects = uCompteur - 1;
            break;
        }

        for (uIndex = 0; uIndex < strlen( buffer); uIndex++)
        {
            if (buffer[uIndex] == ',')
            {
                uCompteur++;
            }
        }
    }

    (void )fclose( pFichier);
    //(void )printf( "The index of 'eObjects' is equal at %u. Parsed %u line.\n", uIndexObjects, uLigne);

    return uIndexObjects;
}
