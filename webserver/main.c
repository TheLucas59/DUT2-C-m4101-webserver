#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

int main(int argc, char** argv) {
  if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
    printf("Don't panic\n");
    return 42;
  }
  printf("Need an advice ?\n");

  int socket_serveur = creer_serveur(8080);
  int socket_client;
  while(1) {
    socket_client = accept(socket_serveur, NULL, NULL);
    if(socket_client == -1) {
      perror("accept socket client");
    }
    const char* message_bienvenue = "Hello World\n";
    write(socket_client, message_bienvenue, strlen(message_bienvenue));
    close(socket_client);
  }
  return 0;
}
