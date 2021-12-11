/**
 * @file    linkedlist.h
 * @author  Haney Kang
 * @date    2021-09-15
 * @brief   Linked List APIs
 */

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include <stddef.h>

typedef struct LinkedList* LinkedList_T;

LinkedList_T LinkedList_new(
                    int (*equalFunc)(const void* elem, const void* argv),
                    void (*freeFunc)(void* elem));

void LinkedList_free(LinkedList_T oLinkedList);

int LinkedList_set(LinkedList_T oLinkedList, const void* content, size_t size);

void* LinkedList_get(LinkedList_T oLinkedList, const void* argv);

int LinkedList_del(LinkedList_T oLinkedList, const void* argv);

void* LinkedList_iter(LinkedList_T oLinkedList);

#endif
