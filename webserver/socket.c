#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#define BACKLOG 10

int creer_serveur(int port) {
  int socket_serveur;

  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

  if(socket_serveur == -1) {
    perror("socket_serveur");
  }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if(bind(socket_serveur, (struct sockaddr*) &saddr, sizeof(saddr)) == -1) {
    perror("bind serveur socket");
  }

  if(listen(socket_serveur, BACKLOG) == -1) {
    perror("listen serveur socket");
  }

  return socket_serveur;
  
}
