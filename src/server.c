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
    num_task--;
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
    /*
    * Execute server io function. 
    */
    run_cli(connfd);
}

void* listen_start(void* arg)
{
    int listenfd = *((int*)arg);
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set reads, cpy_reads;
    int fd_max, fd_num, buf_len;

    FD_ZERO(&reads);
    FD_SET(listenfd, &reads);
    fd_max = listenfd;

    while(1){
        cpy_reads = reads;

        fd_num = select(fd_max + 1, &cpy_reads, 0, 0, NULL);
        if (fd_num == -1) break;
        else if (fd_num == 0) continue;
        else{
            for (int i = 0; i < fd_max + 1; i++){
                if (FD_ISSET(i, &cpy_reads)){
                    if (i == listenfd){ // Accept the request from the client
                        clientlen = sizeof(clientaddr);
                        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
                        add_task_node(connfd);
                        // FD_SET(connfd, &reads);
                        // if(fd_max < connfd) fd_max = connfd;
                    }
                    // else{ // When client
                    //     buf_len = read(i, buf, BUF_SIZE);
                    //     if (buf_len == 0){
                    //         FD_CLR(i, &reads);
                    //         close(i);
                    //     }
                    //     else write(i, buf, buf_len);
                    // }
                }
            }
        }
    }
    close(listenfd);
    return NULL;
}

void run_cli(int connfd)
{
    char buf[MAXLINE];
    int total_length;
    int msg_type;
    char paylord[MAXLINE];
    char* cmd;
    char* word;

    rio_t rio;
    Rio_readinitb(&rio, connfd);
    Rio_readlineb(&rio, buf, MAXLINE);
    while(my_strlen(&buf[0])){
        sscanf(buf, "%d %x %s", total_length, msg_type, paylord);
        if ((cmd = my_strtok_word(line))){
            word = my_strtok_word(NULL);
            if (my_strcmp(cmd, "search") == 0) search(oInvertedIndex, word, connfd);
            else fprintf(Stderr, "Command unkown: %s\n", cmd);
        }
        Rio_readlineb(&rio, buf, MAXLINE);
    }
    Close(connfd);
}


int main(int argc, int *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage = %s <target directory> <port>\n", argv[0]);
        return 1;
    }
    oInvertedIndex = bootstrap(argv[0], argv[1]);
    pthread_mutex_init(&mutex_task);
    pthread_cond_init(&cond_task);


    // Listening Thread
    int* listenfd = (int *)malloc(sizeof(int));
    pthraed_t listen_tid;
    *listenfd = Open_listenfd(argv[2]);
    Pthread_create(&listen_tid, NULL, listen_start, (void*)listenfd);


    // Task Processing Threads
    for (int i = 0; i < num_thread; i++){
        if (pthread_create(&threads[i], NULL, &thread_Start, NULL) != 0){
            perror("Failed to create the thread\n");
        }
    }

    if (pthread_join(&listen_tid, NULL) != 0){
        perror("Failed to join listening thread");
    }

    for (int i = 0; i < num_thread; i++){
        if (pthread_join(&threads[i], NULL) != 0){
            perror("Failed to join the thread\n");
        }
    }

    pthread_mutex_destroy(&mutex_task);
    pthread_cond_destroy(&cond_task);
    return 0;
}
