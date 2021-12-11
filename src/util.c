/**
 * @file    util.c
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Definition of custom string function APIs
 */

#include "util.h"

#include <ctype.h>
#include <stddef.h>

int my_strcmp(const char* str1, const char* str2){
    int compare;
    while((compare = (int)*str1 - (int)*str2) == 0){
        if ((*str1 == '\0') && (*str2 == '\0')) return 0;
        str1 += 1; str2 += 1;
    }
    return compare > 0 ? 1 : -1;
}

void my_strcpy(char* str1, const char* str2){
    int len = 0;
    while(*(str2 + len) != '\0'){
        *(str1 + len) = *(str2 + len);
        len += 1;
    }
    *(str1 + len) = *(str2 + len);
}

int my_strlen(const char* str){
    int len = 0;
    while(*(str + len) != '\0') len += 1;
    return len;
}

char* my_strtok_word (char* str){
    static char* olds;
    int len = 0;

    if(str == NULL) str = olds;

    /* Remove leading non-alphanumerics */
    while(!isalnum((int)*str) && (*str != '\0')) str += 1;

    /* Return NULL if end of string */
    if(*str == '\0'){
        olds = str;
        return NULL;
    }

    /* Find the length of the token.  */
    while(isalnum((int)*(str + len))) len += 1;

    if(*(str + len) == '\0') olds = str + len;
    else olds = str + len + 1;

    *(str + len) = '\0';
    return str;
}

char* my_strcat(char* dest, const char* origin){
    int start = my_strlen(dest);
    my_strcpy(dest + start, origin);
    return dest;
}
