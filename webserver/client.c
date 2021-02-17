#include "socket.h"
#include "client.h"
#include "../parselib/http_parse.h"
#define BUFF_LENGTH 50

char* fgets_or_exit(char* buffer, int size, FILE* stream) {
    if(fgets(buffer, size, stream) == NULL) {
        exit(1);
    }
    return buffer;
}

void traitement_client(int socket_client, char* buff, FILE* fichier) {
    int exitFlag = 0;
    if(socket_client == -1) {
		perror("accept socket client");
    }

	printf("Connexion établie\n");
	
    http_request* req = (http_request*) malloc(sizeof(http_request));

    fgets_or_exit(buff, BUFF_LENGTH, fichier);
    if(parse_http_request(buff, req) == !0){
        // bad req
        fprintf(fichier, "HTTP/1.1 400 Bad request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
        exitFlag = 1;
    } else {
        // good req
        fprintf(fichier, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n%s\r\n", strlen("iiiii"), "iiiii\r\n");
    }
    free(req);
    exit(exitFlag);
}