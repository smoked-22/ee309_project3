/**
 * @file    invidx.c
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Implementation of Inverted Index
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "invidx.h"
#include "util.h"
#include "linkedlist.h"

struct KeyNode{
    char* key;
    LinkedList_T values;
};

int keyMatch(const void* elem, const void* argv){
    struct KeyNode* keyElem = (struct KeyNode*) elem;
    struct KeyNode* query = (struct KeyNode*) argv;
    return my_strcmp(keyElem->key, query->key) ? 0 : 1;
}

int valueMatch(const void* elem, const void* argv){
    struct ValueNode* valueElem = (struct ValueNode*) elem;
    struct ValueNode* query = (struct ValueNode*) argv;
    return my_strcmp(valueElem->name, query->name) &&
                (valueElem->line == query->line);
}

void keyFree(void* elem){
    struct KeyNode* keyElem = (struct KeyNode*) elem;
    free(keyElem->key);
    LinkedList_free(keyElem->values);
    memset(keyElem, 0, sizeof(struct KeyNode));
    free(keyElem);
}

void valueFree(void* elem){
    struct ValueNode* valueElem = (struct ValueNode*) elem;
    free(valueElem->name);
    memset(valueElem, 0, sizeof(struct ValueNode));
    free(valueElem);
}

InvertedIndex_T InvertedIndex_new(){
    return (InvertedIndex_T) LinkedList_new(keyMatch, keyFree);
}

int InvertedIndex_set(InvertedIndex_T oInvertedIndex, const char* key, 
                        const char* name, const int line){
    LinkedList_T oKeyList;
    struct KeyNode* keyNode;
    struct KeyNode queryKey;
    struct ValueNode queryValue;

    oKeyList = (LinkedList_T)oInvertedIndex;

    queryKey.key = (char*) calloc(my_strlen(key) + 1, sizeof(char));
    memcpy(queryKey.key, key, my_strlen(key) + 1);

    queryValue.name = (char*) calloc(my_strlen(name) + 1, sizeof(char));
    memcpy(queryValue.name, name, my_strlen(name) + 1);
    queryValue.line = line;

    keyNode = (struct KeyNode*) LinkedList_get(oKeyList, &queryKey);

    if(!keyNode){
        queryKey.values = LinkedList_new(valueMatch, valueFree);
        if(!LinkedList_set(oKeyList, &queryKey, sizeof(struct KeyNode))) 
            return 0;
        if(!LinkedList_set(queryKey.values, 
                &queryValue, sizeof(struct ValueNode)))
            return 0;
    }
    else{
        if(!LinkedList_set(keyNode->values, 
                &queryValue, sizeof(struct ValueNode)))
            return 0;
    }

    memset(&queryKey, 0, sizeof(struct KeyNode));
    memset(&queryValue, 0, sizeof(struct ValueNode));
    return 1;
}

void InvertedIndex_free(InvertedIndex_T oInvertedIndex){
    LinkedList_free((LinkedList_T) oInvertedIndex);
}

void* InvertedIndex_get(InvertedIndex_T oInvertedIndex, const char* key){
    LinkedList_T oKeyList;
    struct KeyNode queryKey;
    void* keyNode;

    oKeyList = (LinkedList_T)oInvertedIndex;

    queryKey.key = (char*) calloc(my_strlen(key) + 1, sizeof(char));
    memcpy(queryKey.key, key, my_strlen(key) + 1);

    keyNode = LinkedList_get(oKeyList, &queryKey);

    memset(queryKey.key, 0, my_strlen(key) + 1);
    free(queryKey.key);
    memset(&queryKey, 0, sizeof(struct KeyNode));

    return keyNode;
}

struct ValueNode* InvertedIndex_iterKey(InvertedIndex_T oInvertedIndex, 
                        const char* key){
    struct KeyNode* keyNode;

    if(!(keyNode = InvertedIndex_get(oInvertedIndex, key))) return NULL;
    return (struct ValueNode*) LinkedList_iter(keyNode->values);
}
