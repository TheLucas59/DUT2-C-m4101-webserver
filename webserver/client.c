#include "socket.h"
#include "client.h"
#include "../parselib/http_parse.h"
#define BUFF_LENGTH 1024

char* fgets_or_exit(char* buffer, int size, FILE* stream) {
    if(fgets(buffer, size, stream) == NULL) {
        exit(1);
    }
    return buffer;
}

void skip_headers(FILE* client) {
    char buff[BUFF_LENGTH];
    fgets_or_exit(buff, BUFF_LENGTH, client);
    while(!(buff[0] == '\n' || (buff[0] == '\r' && buff[1] == '\n'))) {
        fgets_or_exit(buff, BUFF_LENGTH, client);
    }
}

void send_status(FILE* client, int code, const char* reason_phrase) {
    fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
}

void send_response(FILE* client, int code, const char* reason_phrase, const char* message_body) {
    send_status(client, code, reason_phrase);
    fprintf(client, "Connection: close\r\nContent-Length: %ld\n\r\n", strlen(message_body));
    fprintf(client, "%s", message_body);
}

char* rewrite_target(char* target) {
    int i = 0;
    char* res = (char*) malloc(strlen(target));
    while(target[i] != '\0' || target[i] != '?') {
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

void traitement_client(int socket_client, char* buff, FILE* client) {
    if(socket_client == -1) {
		perror("accept socket client");
    }

	printf("Connexion établie\n");
	
    const char* bvn = "Bienvenue !\n";

    http_request req;

    fgets_or_exit(buff, BUFF_LENGTH, client);
    int parse = parse_http_request(buff, &req);
    skip_headers(client);
    FILE* fichier = NULL;
    char* target;
    if(parse == -1) {
        target = rewrite_target(req.target);
        fichier = check_and_open(target, "../");
        //int size;
        //size = get_file_size(fichier);
        if(req.method == HTTP_UNSUPPORTED) {
            send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
        }
        else {
            send_response(client, 400, "Bad Request", "Bad Request\r\n");
        }
    } 
    else if (strcmp(req.target, "/") == 0) {
        send_response(client, 200, "OK", bvn);
        if(copy(fichier, client) == -1) {
            perror("Erreur de transmission du fichier");
            exit(1);
        }
    }
    else {
        send_response(client, 404, "Not Found", "Not Found\r\n");
    }
    exit(0);
}
