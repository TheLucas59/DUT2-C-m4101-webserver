#include "socket.h"
#include "client.h"
#define PORT 8080
#define BUFF_LENGTH 50

void traitement_signal(int sig) {
	printf("Signal %d reçu\n", sig);

	int pid = waitpid(-1, NULL, WNOHANG);
	printf("Le processus %d a été arrêté\n", pid);
}

void initialiser_signaux(void){
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		perror("Signal SIGPIPE");
	}

	struct sigaction sa;

	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction(SIGCHLD)");
	}
			
}


int main(int argc, char** argv) {
	if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don't panic\n");
		return 42;
	}
	printf("Need an advice ?\n");

	initialiser_signaux();

	int socket_serveur = creer_serveur(PORT);
	if(socket_serveur == -1){
		perror("créer serveur");
		return -1;
	}
	int socket_client;
	while(1) {
		char buff[BUFF_LENGTH];
		
		socket_client = accept(socket_serveur, NULL, NULL);
		FILE* fichier = fdopen(socket_client, "a+");
		if(fork() == 0) {
			traitement_client(socket_client, buff, fichier);
			
			close(socket_client);
			exit(0);
		}
		close(socket_client);
	}
	close(socket_serveur);
	return 0;
}

