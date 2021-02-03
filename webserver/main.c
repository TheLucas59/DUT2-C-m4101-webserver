#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "socket.h"
#define PORT 8080
#define BUFF_LENGTH 50

void traitement_signal(int sig) {
  printf("Signal %d reçu\n", sig);

  int pid = waitpid(-1, NULL, 0);
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
  int socket_client;
  while(1) {
    int rad;
    char* buff[BUFF_LENGTH];
    
    socket_client = accept(socket_serveur, NULL, NULL);
    if(fork() == 0) {
      if(socket_client == -1) {
	perror("accept socket client");
      }

      printf("Connexion établie\n");
      
      sleep(1);
      const char* message_bienvenue = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc neque nisl, tempus sed elementum dictum, fringilla in massa. Nam tempor massa lacus, non posuere mi mattis vitae. Fusce velit orci, efficitur at sodales et, luctus vel felis. Duis viverra, lorem at congue dignissim, elit elit vestibulum urna, fringilla euismod ante mi a leo. Praesent at nulla et nunc pulvinar congue in ut felis. Proin ac dui ut neque finibus vulputate non sed arcu. Aenean suscipit nisl nec tempor accumsan. Praesent at odio finibus, accumsan ipsum non, varius lacus. Nullam tristique mauris in sem imperdiet hendrerit. Proin pharetra accumsan urna iaculis malesuada. In dictum neque ac gravida convallis. Ut quis arcu eu velit tincidunt iaculis ac et sem. Vivamus et velit justo. Etiam at massa tristique, aliquet eros egestas, finibus nunc. Maecenas nulla augue, mattis at ornare in, aliquet eget massa. Suspendisse fermentum ex at libero molestie, quis lacinia nulla congue. Vivamus maximus orci nisl, at vestibulum risus molestie in. In hac habitasse platea dictumst. Maecenas justo sapien, aliquet sed nunc id, finibus maximus odio. Vestibulum non mauris ante. Duis bibendum fermentum lacus sit amet suscipit. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nunc quis velit venenatis, congue neque non, finibus tellus. Nulla nec ullamcorper nunc, id dapibus enim.\n";
      write(socket_client, message_bienvenue, strlen(message_bienvenue));
    
      while((rad = read(socket_client, buff, BUFF_LENGTH)) != 0) {
	write(socket_client, buff, rad);
      }
      
      if(rad == 0) {
	printf("Connexion coupée\n");
      }
      else if(rad == -1) {
	perror("read");
      }
    
      close(socket_client);
      exit(0);
    }
    close(socket_client);
  }
  close(socket_serveur);
  return 0;
}

