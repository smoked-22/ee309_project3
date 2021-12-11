#ifndef _SERVER_H
#define _SERVER_H

#include "header.h"
#include "cli.h"

#define num_thread 300

struct TASK_NODE{
    int connfd;
    struct TASK_NODE *next;
}; typedef TASK_NODE *task_node;


int num_task = 0;
task_node head;
pthread_t threads[num_thread];
pthread_mutext_t mutex_task;
pthread_cond_t cond_task;
InvertedIndex_T oInvertedIndex;



task_node task_node_init(void);
void add_task_node(int connfd);
int pop_task_node(void);

void* thread_start(void* arg);
void* listen_start(void* arg);

#endif
