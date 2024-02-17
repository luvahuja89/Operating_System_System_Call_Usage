#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <calc.h>


int main(int argc, char **argv){

  /*************************************************************
   write code for the main process that will establish
   the communication channel between cleint and server using
   pipes
  *************************************************************/

  pid_t pid;
  // printf("\n 1");
  // Here as per the Workflow of Client and Server based calculator, we require 2 pipes
  // below algorithm will create 2 pipes as below
  // 1. Client will write onto Pipe and Read it from server 
  // 2. Server will write onto Pipe and Read it from Client
  
  int readfd;
  int writefd;
  int clientToServerPipe[2]; 
  int serverToClientPipe[2];
  
  // printf("Entered : %s: %d\n", __FILE__, __LINE__);
  // printf("\n 2"); 
  // Create a pipe 
  if (pipe(clientToServerPipe) < 0 || pipe(serverToClientPipe) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
  // printf("%s:%d: client-to-server readfd: %d; writefd: %d\n", __FILE__,  __LINE__ , clientToServerPipe[0], clientToServerPipe[1]);
  // printf("%s:%d: server-to-client readfd: %d; writefd: %d\n", __FILE__, __LINE__ , serverToClientPipe[0], serverToClientPipe[1]);
  // fork to create parents (Client) and child (server)
  pid = fork();    
  // printf("\n 4");
  if(pid < 0){
    perror("Fork failed\n");
    return 1;
  }
  // printf("\n pid == %d", pid); 
  if(pid == 0){
  // printf("\n 5");
  	readfd  = clientToServerPipe[0];
    	writefd = serverToClientPipe[1];
  // Child process, acting as the server // Close unnecessary pipe ends
  	//close(clientToServerPipe[1]);
  	//printf("\n 15");  	
  	//close(serverToClientPipe[0]);
  // printf("\n 6");
  // printf("%s:%d: readfd : %d and writefd: %d\n", __FILE__,  __LINE__, readfd, writefd);	
  // Redirect stdin and stdout to the pipes
        //dup2(clientToServerPipe[0], STDIN_FILENO);
        //dup2(serverToClientPipe[1], STDOUT_FILENO);
  // printf("\n 7");
  // Close remaining pipe ends
        //close(clientToServerPipe[0]);
        //close(serverToClientPipe[1]);
  // printf("\n 8");
  // call to server here: server(int readfd, int writefd);
  //server(STDIN_FILENO, STDOUT_FILENO);
    server(readfd,writefd); 
    exit(0);
  }
  // printf("\n 9");
  // Parent process, acting as the client // Close unnecessary pipe ends
       //close(clientToServerPipe[0]);
       //printf("\n 14");
       //close(serverToClientPipe[1]);
  // printf("\n 10");
  // Redirect stdin and stdout to the pipes
       //dup2(serverToClientPipe[0], STDIN_FILENO);
       //dup2(clientToServerPipe[1], STDOUT_FILENO);
  // printf("\n 11");
  // Close remaining pipe ends
       //close(serverToClientPipe[0]);
       //close(clientToServerPipe[1]);
  // printf("\n 12");
  readfd  = serverToClientPipe[0];
  writefd = clientToServerPipe[1];
  //call to client here client(int readfd, int writefd);
       //client(STDIN_FILENO, STDOUT_FILENO);
       client(readfd,writefd);
       wait(NULL);
  return 0;
}
