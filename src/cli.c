/**
 * @file    main.c
 * @author  Haney Kang
 * @date    2021-09-16
 * @brief   Main function of text search
 */

#include "cli.h"

int readlinefrom(FILE* fp, char* line){
    int c;

    while(1){
        c = fgetc(fp);
        if((c == '\n') || (c == EOF)) break;
        *line = c;
        line += 1;
    }

    *line = '\0';

    return (c == '\n') ? 1 : 0;
}

int load(InvertedIndex_T oInvertedIndex, const char* path){
    FILE* fDocument;
    char line[MAX_LINE_SIZE];
    char* chunk;
    int lineNum;

    assert(oInvertedIndex);
    assert(path);

    if(!(fDocument = fopen(path, "r"))){
        perror("fopen error");
        return 0;
    }
    
    lineNum = 1;
    while(readlinefrom(fDocument, line)){
        chunk = my_strtok_word(line);
        while(chunk){
            if(!InvertedIndex_set(oInvertedIndex, chunk, path, lineNum))
                fprintf(stderr, "Error while Inserting %s:(%s, %d)\n", 
                    chunk, path, lineNum);
            chunk = my_strtok_word(NULL);
        }
        lineNum += 1;
    }

    fclose(fDocument);
    return 1;
}

int isFile(const char* filename){
    struct stat statbuf;

    if((stat(filename, &statbuf) < 0)){
        perror("stat error");
        return 0;
    }
    return S_ISREG(statbuf.st_mode);
}

InvertedIndex_T bootstrap(char* prog, char* path){
    InvertedIndex_T oInvertedIndex;
    char* filepath;
    DIR* dirpath;
    struct dirent* ep;
    int fileNum;

    oInvertedIndex = InvertedIndex_new();

    if(!(dirpath = opendir(path))){
        perror("opendir error");
        return NULL;
    }
    
    filepath = NULL;
    fileNum = 0;
    while((ep = readdir(dirpath))){
        filepath = (char*)malloc(my_strlen(path) + my_strlen(ep->d_name) + 2);
        my_strcpy(filepath, path);
        filepath = my_strcat(filepath, ep->d_name);
        if(!isFile(filepath) || !load(oInvertedIndex, filepath)) continue;
        free(filepath);
        fileNum += 1;
    }
    closedir(dirpath);
    printf("*** Bootstrapping Complete: %d file(s) loaded ***\n", fileNum);
    return oInvertedIndex;
}

void search(InvertedIndex_T oInvertedIndex, char* keyword){
    struct ValueNode* result;

    if(!keyword) fprintf(stderr, "need keyword\n");
    else{
        while((result = InvertedIndex_iterKey(oInvertedIndex, keyword)))
            printf("%s: line #%d\n", result->name, result->line);
    }
}

void cli(InvertedIndex_T oInvertedIndex, char* prog){
    char line[MAX_LINE_SIZE];
    char* cmd;
    char* arg1;


    printf("%s> ", prog);
    while(readlinefrom(stdin, line)){
        if((cmd = my_strtok_word(line))){
            arg1 = my_strtok_word(NULL);
            if(my_strcmp(cmd, "search") == 0) search(oInvertedIndex, arg1);
            else if(my_strcmp(cmd, "exit") == 0) break;
            else fprintf(stderr, "Command unknown: %s\n", cmd);
        }
        printf("%s> ", prog);
    }
}

// int main(int argc, char* argv[]){
//     InvertedIndex_T oInvertedIndex;

//     if(argc != 2){
//         fprintf(stderr, "usage: %s <target-directory>\n", argv[0]);
//         return 1;
//     }
//     oInvertedIndex = bootstrap(argv[0], argv[1]);
//     cli(oInvertedIndex, argv[0]);
//     InvertedIndex_free(oInvertedIndex);
// }
