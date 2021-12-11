#ifndef _CLI_H
#define _CLI_H

/**
 * @file    main.c
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Main function of text search
 */

#include "invidx.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAX_LINE_SIZE 200

int readlinefrom(FILE* fp, char* line);
int load(InvertedIndex_T oInvertedIndex, const char* path);

int isFile(const char* filename);

InvertedIndex_T bootstrap(char* prog, char* path);

void search(InvertedIndex_T oInvertedIndex, char* keyword);

void cli(InvertedIndex_T oInvertedIndex, char* prog);

#endif
