#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// the port client will be connecting to
#define PORT 3490
// max number of bytes we can get at once
#define MAXDATASIZE 300

#define LINE_MAX 200

struct hostent *he;

int sockfd;

char* ip_address;

void get_host_info() {
  if((he = gethostbyname(ip_address)) == NULL)
  {
    perror("gethostbyname()");
    exit(1);
  }
  else
    printf("Client-The remote host is: %s\n", ip_address);
}

void create_socket() {
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket()");
    exit(1);
  }
  else
    printf("Client-The socket() sockfd is OK...\n");
}

void connect_socket() {
  struct sockaddr_in their_addr;
  // host byte order
  their_addr.sin_family = AF_INET;
  // short, network byte order
  printf("Server-Using %s and port %d...\n", ip_address, PORT);
  their_addr.sin_port = htons(PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  // zero the rest of the struct
  memset(&(their_addr.sin_zero), '\0', 8);
  if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
  {
    perror("connect()");
    exit(1);
  }
  else
    printf("Client-The connect() is OK...\n");
}

void send_message(char const *message) {
  //printf("El mensaje a enviar es: %s", message);
  if (send(sockfd, message, strlen(message), 0) == -1)
    perror("Server-send() error");
}

void receive_message() {
  int numbytes;
  char buf[MAXDATASIZE];

  if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
  {
    perror("recv()");
    exit(1);
  }
  else
    printf("Client-The recv() is OK...\n");

  buf[numbytes] = '\0';
  printf("Client-Received: %s", buf);
}

int main(int argc, char *argv[])
{
  int salir = 1;
  char linea1[LINE_MAX];
  
  if(argc != 2)
  {
    fprintf(stderr, "Client-Usage: %s hostname_del_servidor\n", argv[0]);
    exit(1);
  }
  ip_address = argv[1];
  get_host_info();
  create_socket();
  connect_socket();

  while (salir)
  {
    printf("Escribe un mensaje a enviar\n");
    fgets(linea1,LINE_MAX,stdin);
    send_message(linea1);
    if (strcmp(linea1, "SALIR\n") == 0)
    {
      printf("Client-Closing sockfd\n");
      close(sockfd);
      return 0;
    } else {
      receive_message();
    }
  }
}
