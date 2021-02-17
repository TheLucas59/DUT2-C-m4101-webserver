#include "socket.h"
#include "client.h"
#define BUFF_LENGTH 50

int analyser_ligne_1(char* buff) {
    return strncmp("GET / HTTP/1.1\r\n", buff, 16);
}

char* fgets_or_exit(char* buffer, int size, FILE* stream) {
    if(fgets(buffer, size, stream) == NULL) {
        exit(1);
    }
    return buffer;
 }


void traitement_client(int socket_client, char* buff, FILE* fichier) {
    int i = 1;
    if(socket_client == -1) {
		perror("accept socket client");
}

	printf("Connexion établie\n");
			
	while(fgets_or_exit(buff, BUFF_LENGTH, fichier)) {
        if(i == 1) {
            if(strstr(buff, "inexistant") != NULL) {
                fprintf(fichier, "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 15\r\n\r\n404 Not found\r\n");
                exit(1);
            }
            if(analyser_ligne_1(buff) != 0) {
                fprintf(fichier, "HTTP/1.1 400 Bad request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
                exit(1);
            }
        }
        else {
            if(strncmp("\r\n", buff, 2)) {
                fprintf(fichier, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n%s\r\n", strlen(message_bienvenue), message_bienvenue);
            }
        }
	    
        i++;
	}
		
}
