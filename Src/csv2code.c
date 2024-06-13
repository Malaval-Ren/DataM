/* SPDX - License - Identifier: GPL - 3.0 - or -later
 *
 * A tool to help cross dev for Apple II GS.
 *
 * Copyright(C) 2023 - 2024 Renaud Malaval <renaud.malaval@free.fr>.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <conio.h>
#include <malloc.h>
#include <direct.h>

#include <windows.h>

#include "conv.h"
#include "csv2code.h"

// _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/**
 * @fn char *convertCsvToCode( char *pInputFile, unsigned int uDataSize, unsigned int *puOutputDataSize)
 *
 * @brief
 *
 * @param[in]       pInputFile :
 * @param[in]       uDataSize :
 * @param[in]       puOutputDataSize :
 *
 * return size of pOutputData
 */
char *convertCsvToCode(char *pInputFile, unsigned int uDataSize, unsigned int *puOutputDataSize)
{
    char                   *pOutputFileData = NULL;
    char                   *pInputRunner = NULL;

    if ((pInputFile) && (uDataSize))
    {
        pOutputFileData = calloc((size_t )(65535), 1);
        if (pOutputFileData)
        {

        }
    }

    return pOutputFileData;
}
