/**
 * @file    linkedlist.c
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Implementation of Linked List
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "linkedlist.h"

struct Node{
    struct Node* next;
    struct Node* prev;
    void* content;
};

struct LinkedList{
    struct Node* nodeFirst;
    struct Node* nodeLast;
    struct Node* iterPtr;
//    size_t len;
    int (*equalFunc)(const void* elem, const void* argv);
    void (*freeFunc)(void* elem);
};

int TrueFunc(const void* elem, const void* argv){ return 1; }

struct Node* _LinkedList_get(LinkedList_T oLinkedList, const void* argv){
    struct Node* currNode;
    
    assert(oLinkedList);

    currNode = oLinkedList->nodeFirst;
    while(currNode){
        if(oLinkedList->equalFunc(currNode->content, argv)) return currNode;
        currNode = currNode->next;
    }
    return NULL;
}

LinkedList_T LinkedList_new(
                int (*equalFunc)(const void* elem, const void* argv),
                void (*freeFunc)(void* elem)){
    LinkedList_T oLinkedList;

    assert(equalFunc && freeFunc);

    oLinkedList = (struct LinkedList*) calloc(1, sizeof(struct LinkedList));
    if(!oLinkedList) return NULL;
    oLinkedList->equalFunc = equalFunc;
    oLinkedList->freeFunc = freeFunc;

    return oLinkedList;
}

void LinkedList_free(LinkedList_T oLinkedList){
    assert(oLinkedList);

    oLinkedList->equalFunc = TrueFunc;
    while(LinkedList_del(oLinkedList, NULL));
    memset(oLinkedList, 0, sizeof(struct LinkedList));
    free(oLinkedList);
}

int LinkedList_set(LinkedList_T oLinkedList, 
                const void* content, size_t size){
    struct Node* targetNode;

    assert(oLinkedList);

    if((targetNode = _LinkedList_get(oLinkedList, content))){
        targetNode->content = realloc(targetNode->content, size);
        memcpy(targetNode->content, content, size);
    }
    else{
        targetNode = (struct Node*) calloc(1, sizeof(struct Node));
        if(!targetNode) return 0;
        targetNode->content = realloc(targetNode->content, size);
        memcpy(targetNode->content, content, size);
        targetNode->prev = oLinkedList->nodeLast;
        if((!oLinkedList->nodeFirst) || (!oLinkedList->nodeLast)){
            assert((!oLinkedList->nodeFirst) && (!oLinkedList->nodeLast));
            oLinkedList->nodeFirst = targetNode;
        }
        else oLinkedList->nodeLast->next = targetNode;
        oLinkedList->nodeLast = targetNode;
    }
    return 1;
}

void* LinkedList_get(LinkedList_T oLinkedList, const void* argv){
    struct Node* node;

    assert(oLinkedList);

    if(!(node = _LinkedList_get(oLinkedList, argv))) return NULL;
    return node->content;
}

int LinkedList_del(LinkedList_T oLinkedList, const void* argv){
    struct Node* currNode;

    assert(oLinkedList);

    if(!(currNode = _LinkedList_get(oLinkedList, argv))) return 0;
    if(!currNode->prev) oLinkedList->nodeFirst = currNode->next;
    else currNode->prev->next = currNode->next;
    if(!currNode->next) oLinkedList->nodeLast = currNode->prev;
    else currNode->next->prev = currNode->prev;
    
    oLinkedList->freeFunc(currNode->content);
    memset(currNode, 0, sizeof(struct Node));
    free(currNode);
    return 1;
}

void* LinkedList_iter(LinkedList_T oLinkedList){
    assert(oLinkedList);

    if(!oLinkedList->iterPtr){
        oLinkedList->iterPtr = oLinkedList->nodeFirst;
        return oLinkedList->nodeFirst->content;
    }
    else{
        oLinkedList->iterPtr = oLinkedList->iterPtr->next;
        if(!oLinkedList->iterPtr) return NULL;
        return oLinkedList->iterPtr->content;
    }
}
