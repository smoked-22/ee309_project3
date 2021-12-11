#include "server.h"

task_node task_node_init(void)
{
    head = (task_node)malloc(sizeof(struct TASK_NODE));
    head->connfd = 0;
    head->next = NULL;
}

void add_task_node(int connfd)
{
    // Allocate memory for new task. 
    task_node temp head->next;
    task_node new_node = (task_node)malloc(sizeof(struct TASK_NODE));

    // Link with head and increase num_task. 
    new_node->connfd = connfd;
    head->next = new_node;
    new_node->next = temp;
    num_task++;
    pthread_cond_signal(&cond_task);
}

int pop_task_node(void)
{
    // Pops lastly inputted task number and returns connfd mentioned in there. 
    int connfd;
    if (head->next == NULL) {
        // There is no task to pop. 
        return NULL;
    }
    task_node target_node = head->next;
    head->next = target_node->next;
    connfd = target_node->connfd;
    free(target_node);
    return connfd;
}

void* thread_start(void* arg)
{
    int connfd;
    pthread_mutex_lock(&mutex_task);
    while(num_task == 0){
        pthread_cond_wait(&cond_task, &mutex_task);
    }
    connfd = pop_task_node();
    pthread_mutex_unlock(&mutex_task);
}


int main(){

    return 0;
}
