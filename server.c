/*****************************DONT FOS**********/
/*            - TCP server -           */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 9876                  // port number
#define error(a,b) fprintf(stderr, a, b)  // error 'function'

/**
 * mastermind game representation
 */
 char * mastermind(char * sec, char * guess, char * reply)
 {

 	int i = 0;
 	int slot_counter = 0;
 	int color_counter = 0;
 	int seen [4] = {-1,-1,-1,-1};

 	//check first color
 	if (((guess[0] == sec[1]) || (guess[0] == sec[2]) || (guess[0] == sec[3]))) {
 		for(i = 0; i < 4; i++) {
 			if ((guess[0] == sec[i]) && (seen[i] == -1)) {
 				seen[i] = 1;
 				break;
 			}
 		}
 	}
 	//check second color
 	if (((guess[1] == sec[0]) || (guess[1] == sec[2]) || (guess[1] == sec[3]))) {
 		if ((guess[1] == sec[0]) && (seen[0] == -1)) {
 			seen[0] = 1;
 		}
 		else if ((guess[1] == sec[1]) && (seen[0] == -1)) {
 			seen[1] = 1;
 		}
 		else {
 			for(i = 2; i < 4; i++) {
 				if ((guess[1] == sec[i]) && (seen[i] == -1)) {
 					seen[i] = 1;
 					break;
 				}
 			}
 		}
 	}
 	//check third color
 	if (((guess[2] == sec[0]) || (guess[2] == sec[1]) || (guess[2] == sec[3]))) {
 		if ((guess[2] == sec[0]) && (seen[0] == -1)) {
 			seen[0] = 1;
 		}
 		else if((guess[2] == sec[1]) && (seen[1] == -1)) {
 			seen[1] = 1;
 		}
 		else if((guess[2] == sec[2]) && (seen[2] == -1)) {
 			seen[2] = 1;
 		}
 		else if((guess[2] == sec[3]) && (seen[3] == -1)) {
 			seen[3] = 1;
 		}
 	}
 	//check fourth color
 	if (((guess[3] == sec[0]) || (guess[3] == sec[1]) || (guess[3] == sec[2]))) {
 		if ((guess[3] == sec[0]) && (seen[0] == -1)) {
 			seen[0] = 1;
 		}
 		else if((guess[3] == sec[1]) && (seen[1] == -1)) {
 			seen[1] = 1;
 		}
 		else if((guess[3] == sec[2]) && (seen[2] == -1)) {
 			seen[2] = 1;
 		}
 		else if((guess[3] == sec[3]) && (seen[3] == -1)) {
 			seen[3] = 1;
 		}
 	}
 	//check for correct color and slot
 	for (i= 0; i < 4; i++) {
 		if (sec[i] == guess[i]) {
 			slot_counter++;
 			seen[i] = -1;
 		}
 	}
 	//tally up number of correct colors
 	for (i = 0; i < 4; i++) {
 		if (seen[i] != -1) {
 			color_counter++;
 		}
}
	if (slot_counter == 4) {
 		reply = "You win";
	} else {
 	sprintf(reply, "%d correct color+slot, %d correct colors", slot_counter, color_counter);
}

 	return reply;
 }

int main(int argc, char *argv[] ){ // arg count, arg vector

   /************************** Declarations ***********************/
   int fd, fd2;	        	           // socket endpoint
   int fdc1;                        // socket endpoint
   int flags;                      // receive/send flags
   struct sockaddr_in server;      	// socket name (addr) of server
   struct sockaddr_in client;	     	// socket name of client
   struct sockaddr_in client2;
   int server_size;                	// length of the socket addr. server
   int client_size;                	// length of the socket addr. client
   int client2_size;
   int bytes;		           			// length of buffer
   int bytes2;
   int rcvsize;                    	// received bytes
   int rcvsize2;
   int trnmsize;                   	// transmitted bytes
   int trnmsize2;       // transmitted bytes
   int err;                        // error code
   char on;                        // sockopt option
   char buffer[100];
   char temporary[100];
   char temporary2[100];
   char buffer2[100];				       // datagram dat buffer area
   int fdc2;
   char s[INET6_ADDRSTRLEN];
   int endgame = 0;
	 char temp[100];
	 char * msg;
   char * msg2;
   int num_guess = 0;
   /********************** Initialization *************************/
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   server_size            = sizeof server;
   client_size            = sizeof client;
	 char choice[5];
	 char secret[5];
	 char reply[100];
	 int i;

	printf("Enter <random> or <four digit number>: ");
	scanf("%s", choice);

	if (strcmp(choice, "random") == 0) {
		srand(time(NULL));
		for (i=0; i <4; ++i)
			secret[i] = (char)(((int)'0')+rand() % 6);
		secret[4] = '\0';
		//printf("%s", secret);
	}
	else {
		for (i = 0; i < 4; i++)
			secret[i] = choice[i];
		secret[4] = '\0';
	}

   /************************ Creating socket **********************/
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   if ( fd < 0 ) {
      error("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /************************ Setting options **********************/
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /************************ Binding socket ***********************/
   err = bind( fd, (struct sockaddr *) &server, server_size);

   /************************ Listening ****************************/
   err = listen( fd, 2);

   printf("Waiting for connections..\n");

   fdc1 = accept(fd, (struct sockaddr *) &client, &client_size);
   fdc2 = accept(fd, (struct sockaddr *) &client, &client_size);
   if (fdc1 < 0) {
      printf("%s: Cannot accept on socket\n",argv[0]);
      exit(4);
    }

	strcpy(buffer, "");

    while(1){

          send(fdc1, buffer, 100, flags);
    		  recv(fdc1, buffer, 100, flags );

          buffer[rcvsize] = '\0';
          msg = mastermind(secret, buffer, temp);
          ++num_guess;
          sprintf(buffer, "%s%s",buffer, msg);

          if((strncmp(msg, "You win", 7) == 0)){
              send(fdc1, "You win", 100, flags);
              send(fdc2, "You Lose", 100, flags);
          	  break;
            }

          send(fdc2, buffer, 100, flags);
          send(fdc1, msg, 100, flags);
          rcvsize = recv(fdc2, buffer, 100, flags );

          buffer[rcvsize] = '\0';
          msg = mastermind(secret, buffer, temp);
          sprintf(buffer, "%s%s",buffer, msg);
          //send(fdc2, msg, 100, flags);

          if((strncmp(msg, "You win", 7) == 0)){
            sprintf(buffer2, "You Lose. The winner number is: %s\n", secret);
              send(fdc2, "You win", 100, flags);
              send(fdc1, buffer2, 100, flags);
          	  break;
            }

          if(num_guess == 2){
              sprintf(msg, "You Lose. Both of you. Winnumber: %s\n", secret);
              send(fdc1, msg, 100, flags);
              send(fdc2, msg, 100, flags);
              break;
            }
            send(fdc2, msg, 100, flags);
    }

close(fdc1);
close(fdc2);
exit(0);
   }
