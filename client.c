/***************************************/
/*            - TCP client -           */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define PORT_NO 9876
#define error(a,b) fprintf(stderr, a, b)

int main(int argc, char *argv[] ){ // arg count, arg vector

   /************************** Declarations ********************/
   int fd;	                       // socket endpoint
   int flags;                      // receive/send flags
   struct sockaddr_in server;      // address of server
   struct sockaddr_in client;      // address of client
   int server_size;                // length of the sockaddr_in server
   int client_size;                // length of the sockaddr_in client
   int bytes;                      // length of buffer
   int rcvsize;                    // received bytes
   int trnmsize;                   // transmitted bytes
   int err;                        // error code
   char on;                        // sockopt option
   char buffer[100];                  // datagram dat buffer area
   char server_addr[16];           // server address
   char reply[100];
   char s[INET6_ADDRSTRLEN];
   char buffer2[100];
   char buffer3[100];
   char TEMP[100];
	char * win = "You win";
	char * lose = "You Lose";
  int cica;


   /********************** Initialization **********************/
   on                     = 1;
   flags                  = 0;
   sprintf(server_addr, "%s", argv[1]);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr(server_addr);
   server.sin_port        = htons(PORT_NO);
   server_size            = sizeof server;
   client_size            = sizeof client;

   /********************** Creating socket **********************/
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   printf("Creating socket! \n");
   if ( fd < 0 ) {
      error("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /********************** Setting options **********************/
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /********************** connecting ***************************/
   err = connect( fd, (struct sockaddr *) &server, server_size);
   printf("Connecting.. \n");
   if ( err < 0 ) {

      error("%s: Cannot connect to server\n",argv[0]);
      }

  //recv( fd, buffer, 1024, flags );
  // printf("%s\n", buffer);
  //
	// printf("Koszonj a masiknak: ");
	// fgets(buffer,100,stdin);
	// send(fd, buffer, 100, flags);
  //
	// recv(fd, buffer, 100, flags );
	// printf("Az ellenfél üzenete: %s\n", buffer);

    while(1){

        recv(fd, TEMP, 100, flags );
        printf("Előző válasz: %s\n", buffer);
        printf("%s\n", TEMP);
        printf("\nEnter your guess: ");
				fgets(buffer,100,stdin);
				send(fd, buffer, 100, flags);

                /*cica++;
                printf("Enter your guess: ");
                fgets(buffer, 100-1, stdin);

                send(fd, buffer, 10, flags);

                rcvsize = recv( fd, buffer, 1024, flags );
                buffer[rcvsize] = '\0';

                if(strncmp(buffer, lose, 8) == 0){
                    printf("%s\n", buffer);
                    close(fd);
                    break;
                }

                if(strcmp(buffer, win) == 0){
                    recv( fd, buffer2, 1024, flags );
                    printf("Winner number: %s\n", buffer2);
                    close(fd);
                    break;
                } printf("Hint: %s\n", buffer);*/
  }

   exit(0);
   }
