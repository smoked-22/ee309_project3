/**
 * @file    invidx.h
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Linked List based inverted index APIs
 */

#ifndef _INVIDX_H
#define _INVIDX_H

#include "linkedlist.h"

typedef LinkedList_T InvertedIndex_T;

struct ValueNode{
    char* name;
    int line;
};

InvertedIndex_T InvertedIndex_new();

int InvertedIndex_set(InvertedIndex_T oInvertedIndex, const char* key,
                    const char* name, const int line);

void InvertedIndex_free(InvertedIndex_T oInvertedIndex);

void* InvertedIndex_get(InvertedIndex_T oInvertedIndex, const char* key);

struct ValueNode* InvertedIndex_iterKey(InvertedIndex_T oInvertedIndex, 
                    const char* key);

#endif
