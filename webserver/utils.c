#include "socket.h"

char* rewrite_target(char* target) {
    if(strcmp(target, "/") == 0){
        return "/index.html";
    }
    int i = 0;
    char* res = (char*) malloc(strlen(target)*sizeof(char));
    while(target[i] != '\0' && target[i] != '?') {
        res[i] = target[i];
        i++;
    }
    if(target[i] == '?') {
        res[i] = '\0';
    }
    return res;
}

FILE* check_and_open(const char *target, const char *document_root) {
    int targetLength = strlen(target);
    int document_rootLength = strlen(document_root);
    char path[targetLength + document_rootLength + 1];
    strcpy(path, document_root);
    strcat(path, target);

    struct stat stats;
    stat(path, &stats);

    if((stats.st_mode & S_IFMT) != S_IFREG) {
        perror("Ce n'est pas un fichier régulier.");
        return NULL;
    }

    FILE* file = fopen(path, "r");
    return file;
}

int get_file_size(FILE* file) {
    struct stat stats;
    int fd = fileno(file);
    fstat(fd, &stats);
    return stats.st_size;
}

int copy(FILE *in, FILE *out) {
    char buff[get_file_size(in)];
    if(read(fileno(in), buff, get_file_size(in)) == -1) {
        perror("Erreur de lecture du fichier");
        return -1;
    }
    if(write(fileno(out), buff, get_file_size(in)) == -1) {
        perror("Erreur de copie du fichier");
        return -1;
    }

    return 0;
}