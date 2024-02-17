#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <calc.h>

void client(int readfd, int writefd) {
  /*************************************************************
   write client code to be used by the parent process
  *************************************************************/
    char inputBuffer[MAX_BUFF];
//  printf("\n inclient-1"); 
//  printf("Proceeding in client : %s: %s\n", __FILE__, __FUNCTION__);
    while (1) {
        // Read input from stdin
        // printf("Enter a mathematical expression (or type 'END' to exit): ");
        memset(inputBuffer, '\0', MAX_BUFF);
        // memset(buf, '\0', sizeof(buf) - 1);

	fgets(inputBuffer, MAX_BUFF, stdin);



        // printf("\n inclient-2");
        // Remove newline character if present
        size_t inputLength = strlen(inputBuffer);
        if (inputLength > 0 && inputBuffer[inputLength - 1] == '\n') {
            inputBuffer[inputLength - 1] = '\0';
        }

        // Write input to the server
        write(writefd, inputBuffer, strlen(inputBuffer));

        // printf("\n inclient-3");
        // Check for termination condition
        if (strncmp(inputBuffer, "END", 3) == 0) {
        	inputBuffer[4] = '\0';
        // printf("%s:%s:%d buf_size: %d: buf: %s\n",__FILE__, __FUNCTION__, __LINE__, (int) inputLength, inputBuffer);
        break;
        }
        // printf("\n inclient-4 %ld %s", strlen(inputBuffer), inputBuffer);


        char result_buf[MAX_BUFF + 128];
        memset(result_buf, '\0', MAX_BUFF);

        // printf("\n inclient-5");
        // Read and print the server's response
         memset(inputBuffer, '\0', MAX_BUFF - 1);
        // printf("\n inclient-6 %s", inputBuffer);
	int lenx;
        lenx =  read(readfd, inputBuffer, MAX_BUFF - 1);
	inputBuffer[lenx] = '\0';
        // Read and print the server's response
        memset(result_buf, '\0', MAX_BUFF);
        // printf("%s: %s: %d: Here we got the value: \'%s\'\n", __FILE__, __FUNCTION__, __LINE__, inputBuffer);
        sprintf(result_buf, "RESULT: %s\n", inputBuffer);
        write(1, result_buf, strlen(result_buf));
    }
}



