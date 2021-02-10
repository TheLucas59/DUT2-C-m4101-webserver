#include "socket.h"
#include "client.h"
#define BUFF_LENGTH 50

int analyser_ligne_1(char* buff) {
    return strncmp("GET / HTTP/1.1\r\n", buff, 16);
}

void traitement_client(int socket_client, char* buff, FILE* fichier) {
    int i = 1;
    if(socket_client == -1) {
		perror("accept socket client");
    }

	printf("Connexion établie\n");
			
	sleep(1);
	const char* message_bienvenue = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc neque nisl, tempus sed elementum dictum, fringilla in massa. Nam tempor massa lacus, non posuere mi mattis vitae. Fusce velit orci, efficitur at sodales et, luctus vel felis. Duis viverra, lorem at congue dignissim, elit elit vestibulum urna, fringilla euismod ante mi a leo. Praesent at nulla et nunc pulvinar congue in ut felis. Proin ac dui ut neque finibus vulputate non sed arcu. Aenean suscipit nisl nec tempor accumsan. Praesent at odio finibus, accumsan ipsum non, varius lacus. Nullam tristique mauris in sem imperdiet hendrerit. Proin pharetra accumsan urna iaculis malesuada. In dictum neque ac gravida convallis. Ut quis arcu eu velit tincidunt iaculis ac et sem. Vivamus et velit justo. Etiam at massa tristique, aliquet eros egestas, finibus nunc. Maecenas nulla augue, mattis at ornare in, aliquet eget massa. Suspendisse fermentum ex at libero molestie, quis lacinia nulla congue. Vivamus maximus orci nisl, at vestibulum risus molestie in. In hac habitasse platea dictumst. Maecenas justo sapien, aliquet sed nunc id, finibus maximus odio. Vestibulum non mauris ante. Duis bibendum fermentum lacus sit amet suscipit. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nunc quis velit venenatis, congue neque non, finibus tellus. Nulla nec ullamcorper nunc, id dapibus enim.\n";
		
	while(fgets(buff, BUFF_LENGTH, fichier) != NULL) {
        if(i == 1) {
            if(analyser_ligne_1(buff) != 0) {
                fprintf(fichier, "HTTP/1.1 400 Bad request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
                exit(0);
            }
        }
        else {
            if(strncmp("\r\n", buff)) {
                fprintf(fichier, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n%s\r\n", strlen(message_bienvenue), message_bienvenue);
            }
        }
	    
        i++;
	}
		
}
