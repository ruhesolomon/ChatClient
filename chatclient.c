#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

//int sockfd;
int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];
int handle_stdin() {
    /* TODO */
	 int input_given = 0;
	 char ch;
	 int i =0;
	 outbuf[0] = '\0';
	 errno = 0;
	 while((ch = getc(stdin)) != '\n') {
		 if(ch==EOF){
			return -1;
		 }
	       if(i < MAX_MSG_LEN){ outbuf[i++] = ch;
		  input_given =1;

	       }
	       else{
                      i++;
               }
	 }
	 /*char temp;
	 if ((temp = getc(stdin)) == EOF){
		 if(send(client_socket, outbuf, strlen(outbuf)+1, 0) == -1){
                                 perror("send");
                                 return -1;
                        }
		close(client_socket);
                        fflush(stdout);
                        exit(0);
                
	 } else {
		 ungetc(temp, stdin);
	 */

	 
	 	if(i > MAX_MSG_LEN){
                      printf("Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
		      i=0;
		}
		else{
			if(send(client_socket, outbuf, strlen(outbuf)+1, 0) == -1){
		  		 perror("send");
		  		 return -1;
			}
	   }
		//memset(outbuf, 0, sizeof(outbuf));
		 if (strcmp(outbuf, "bye") == 0) {
		 	printf("Goodbye.\n");
			close(client_socket);
			fflush(stdout);
			exit(0);
		}
		// int end = strlen(outbuf);
		//if (end > 0 && outbuf[end-1] == EOF) {
    		//	printf("Goodbye.\n");
                     //   close(client_socket);
                  //      fflush(stdout);
                   //     exit(0);
		//}		
		 memset(outbuf, 0, sizeof(outbuf));
		 	    return input_given;
	 
}

int handle_client_socket() {
      int bytes_recv = recv(client_socket, inbuf, MAX_MSG_LEN, 0);
      if (bytes_recv < 0) {
           if (errno != EINTR) {
                 perror("Warning: Failed to receive incoming message.");
     		 return 0;
	   }
  	} else if (bytes_recv == 0) {
		printf("\nConnection to server has been lost.\n");
		close(client_socket);	
		goto exit;
 	 } else {
   		 inbuf[bytes_recv] = '\0';
		 if(strcmp(inbuf, "bye")!=0){
			 printf("\n%s\n", inbuf);
		 }
	 }
      if(strcmp(inbuf, "bye")==0){
		printf("\nServer initiated shutdown.\n");
		close(client_socket);
                return 0;
		      }
 	 return 0;
exit:
	 exit(0);
      }



int main(int argc, char **argv) {
    /* TODO */
   if(argc != 3){
      fprintf(stderr, "Usage: %s <server IP> <port>\n", strerror(errno));
      return EXIT_FAILURE;
   }
   char* ip_server = argv[1];
   char* input_port = argv[2];
   int port = atoi(input_port);
  // int client_socket,
      int bytes_recvd;
              struct sockaddr_in server_addr;
	       socklen_t addrlen = sizeof(struct sockaddr_in);
              memset(&server_addr, 0, addrlen);
	       server_addr.sin_family = AF_INET;
	      server_addr.sin_port = htons(port);
              client_socket = socket(AF_INET, SOCK_STREAM, 0);
   int ip_val;
   if((ip_val=inet_pton(AF_INET, ip_server, &server_addr.sin_addr))>0){
      //int port = atoi(input_port);
      if(port>1024 && port<65535){
	      //create TCP socket
//	      char buf[BUFLEN];
	      //char *addr_str = "127.0.0.1";
	      // Create a reliable, stream socket using TCP.
	      if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Error: Failed to create socket. %s.\n",strerror(errno));
	        return EXIT_FAILURE;	
	      }
	      //snprintf(buf, BUFLEN, "%s:%s", ip_server, input_port);
	      if (connect(client_socket, (struct sockaddr *)&server_addr, addrlen) < 0) {
		 fprintf(stderr, "Error: Failed to connect to server. %s.\n",strerror(errno));
		 return EXIT_FAILURE;
	      }
	      if((bytes_recvd = recv(client_socket, inbuf, BUFLEN - 1, 0)) < 0) {
    		 fprintf(stderr, "Error: Failed to receive message from server. %s.\n",strerror(errno));
		 return EXIT_FAILURE;	
	      }
	    //  inbuf[bytes_recvd] = '\0';
	    //  printf("Welcome message from server: %s\n\n", inbuf);
//	      if(handle_stdin() == 1){
//		 if(strlen(outbuf) < MAX_NAME_LEN){
	         //snprintf(inbuf, BUFLEN, "%s", username);
//		  strcpy(username, outbuf);
//		    if (send(sockfd, inbuf, strlen(inbuf), 0) < 0) {
//		       fprintf(stderr, "Error: can't send to server. %s\n", strerror(errno));
//		       return EXIT_FAILURE; 
  // 		    }
//		 }
	     // }
      }
      else{
	     fprintf(stderr, "Error: port out of range. %s\n", strerror(errno));
      }
   }
   else{
      fprintf(stderr, "ERROR: IP address is invalid. %s\n", strerror(errno));
      return EXIT_FAILURE;
   }
	//we handle the username outside of the while loop
	//we dont need fdset for this part
   //	char username[MAX_NAME_LEN];
       //heyyyy
       // while((ch = getc(stdin))!= '\n'){
         //      if(i < MAX_MSG_LEN){
        //          outbuf[i++] = ch;
      //            input_given =1;
      //         }else{
      //                i++;
     //          }
        //    }
     //           if(i > MAX_MSG_LEN){
       //               printf("Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
         //             i=0;
           //     }
       //heyyyy 
       bool too_long = false;
	while(1){
		int j=0;
		char cha;
		if(isatty(STDIN_FILENO)){
			printf("Enter your username: ");
			fflush(stdout);
	}
	//	if (fgets(username, MAX_NAME_LEN, stdin) == NULL) {
           //     	fprintf(stderr, "Error reading username from stdin.\n");
             //  		 exit(1);
         //   }
		while((cha = getc(stdin))!= '\n'){
              		 if(j< MAX_NAME_LEN){
                  		username[j++] = cha;
               		}else{
                      	too_long = true;	
               }
            }
      	 username[j] = '\0';
	 if(too_long){
                    fprintf(stderr, "Sorry, limit your username to 1 line of at most %d characters.\n", MAX_NAME_LEN - 1);
			too_long = false;
            }
	 else{

		printf("Hello, %s let's try to connect you to the server\n", username);
        	printf("\n%s\n\n", inbuf);
       		 if (send(client_socket, username, strlen(username)+1, 0) == -1) {
                	perror("send");
              		 exit(1);
            }
 		break;		 
	 }
	// username[strcspn(username, "\n")] = '\0';
	}

//	printf("Hello, %s let's try to connect you to the server\n", username); 
	// printf("\n%s\n\n", inbuf);
//	if (send(client_socket, username, strlen(username)+1, 0) == -1) {
  //              perror("send");
    //           exit(1);
     //      }
     
  //  int num_bytes;
    fd_set read_fds;
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);
        fd_set temp_fds = read_fds;
        if(isatty(STDIN_FILENO) && strcmp(inbuf, "bye")){
	//	if(recv(client_socket, inbuf, MAX_MSG_LEN, 0)!=0){
		printf("[%s]: ", username);
		fflush(stdout);
	//	}
	}
	if (select(client_socket+ 1, &temp_fds, NULL, NULL, NULL) == -1) {
           // perror("select");
            exit(1);
	}

	 if(FD_ISSET(STDIN_FILENO, &temp_fds)){
		if(handle_stdin() == -1){
		  printf("\n");
		  printf("\n");
		  break;
		  close(client_socket);
		  fflush(stdout);
		  return(EXIT_FAILURE);
			// snprintf(outbuf,strlen(outbuf));
		}
	 }
        if (FD_ISSET(client_socket, &temp_fds)) {
            if (handle_client_socket()==-1) {
                    perror("Error with client socket");
		    break;
        
            } 
	} 

                //  if message is "bye" end it
              //  if (strcmp(message, "bye\0") == 0) {
                //    printf("Goodbye.\n");
                  //  break;
              //  }
    }
    close(client_socket);
    return 0;
}
