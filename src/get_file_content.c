#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char* getFileContent(const char* filename) {

    char* result = NULL;
    int nbRead = 0;
    int nbReadFromStart = 0;
    int fd = open(filename, O_RDONLY);

    if(fd == -1) {
        perror("getFileContent");
        return NULL;
    }

    result = (char*)malloc(sizeof(char) * 1024);
    if(result == NULL) {
        perror("getFileContent");
        close(fd);
        return NULL;
    }

    while((nbRead = read(fd, result + nbReadFromStart, 1024)) > 0) {
        char* tmp = NULL;
        nbReadFromStart += nbRead;
        tmp = (char*)realloc(result, sizeof(char) * (nbReadFromStart + 1024));
        if(tmp == NULL) {
            perror("getFileContent");
            free(result);
            close(fd);
            return NULL;
        }
        result = tmp;
    }

    if(nbRead == -1) {
        perror("getFileContent");
        free(result);
        close(fd);
        return NULL;
    } else {
        char* tmp = realloc(result, sizeof(char) * (nbReadFromStart + 1));
        if(tmp == NULL) {
            perror("getFileContent");
            free(result);
            close(fd);
            return NULL;
        }
        result = tmp;
        result[nbReadFromStart] = '\0';
    }

    close(fd);

    return result;

}
