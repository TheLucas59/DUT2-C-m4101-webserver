#include "socket.h"
#include "client.h"
#include "utils.h"
#include "stats.h"
#include "../parselib/http_parse.h"
#define BUFF_LENGTH 1024
#define CONTENT_PATH "./www"

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

void send_stats(FILE *client){
	send_status(client, 200, "OK");
	fprintf(client,"\n Served connections%d \n",get_stats()->served_connections);
	fprintf(client,"\n Served requests %d \n",get_stats()->served_requests);
	fprintf(client,"\n OK 200 %d \n",get_stats()->ok_200);
	fprintf(client,"\n KO 400 %d \n",get_stats()->ko_400);
	fprintf(client,"\n KO 403 %d \n",get_stats()->ko_403);
	fprintf(client,"\n KO 404 %d \n",get_stats()->ko_404);
}

void send_response(FILE* client, int code, const char* reason_phrase, const char* message_body, FILE* fichier) {
    send_status(client, code, reason_phrase);
    if(fichier != NULL) {
        fprintf(client, "Connection: close\r\nContent-Length: %d\n\r\n", get_file_size(fichier));
        fflush(client);

        if(copy(fichier, client) == -1) {
            perror("Erreur de transmission du fichier");
            exit(1);
        }
    }
    else {
        fprintf(client, "Connection: close\r\nContent-Length: %ld\n\r\n", strlen(message_body));
        fflush(client);
    }
}



void traitement_client(int socket_client, char* buff, FILE* client) {
    if(socket_client == -1) {
		perror("accept socket client");
    }

	printf("Connexion établie\n");
	get_stats()->served_connections++;
    http_request req;

    fgets_or_exit(buff, BUFF_LENGTH, client);
    int parse = parse_http_request(buff, &req);
    skip_headers(client);
    FILE* fichier = NULL;
    char* target;
    if(parse == -1) {
        if(req.method == HTTP_UNSUPPORTED) {
            send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n", NULL);
        }
        else {
            send_response(client, 400, "Bad Request", "Bad Request\r\n", NULL);
			get_stats()->ko_400++;
        }
    } 
    else {
        int status = 0;
        target = rewrite_target(req.target);
		if(strncmp(target, "/stats", 6)==0){
			get_stats()->served_requests++;
			send_stats(client);
		}else{
			fichier = check_and_open(target, CONTENT_PATH, &status);
			get_stats()->served_requests++;
			if (status == 404){
				send_response(client, 404, "Not Found", "Not Found\r\n", NULL);
				get_stats()->ko_404++;
				exit(1);
			}
			if(status == 403){
				send_response(client, 403, "Forbidden", "Forbidden\r\n", NULL);
				get_stats()->ko_403++;
				exit(1);
			}
			send_response(client, status, "OK", NULL, fichier);
			get_stats()->ok_200++;
		}
	}
    exit(0);
}
