/**
 * @file    util.h
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Custom string function APIs
 */

#ifndef _MYSTRING_H
#define _MYSTRING_H

int my_strcmp(const char* str1, const char* str2);

void my_strcpy(char* str1, const char* str2);

int my_strlen(const char* str);

char* my_strtok_word(char* str);

char* my_strcat(char* dest, const char* origin);

#endif
