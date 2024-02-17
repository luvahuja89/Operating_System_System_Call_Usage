//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <unistd.h>
//#include <calc.h>

//void server(int readfd, int writefd){
  /*************************************************************
   write server code to be used by the child process
  *************************************************************/
//  char buf[MAX_BUFF];
//  size_t len;

  /* Implement server functionlity here
    use calculate function to evaluate expression
    signature: calculate(char *buf)
  */
//}

// ================================================================================ //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <calc.h>

void server(int readfd, int writefd){
  /*************************************************************
   write server code to be used by the child process
  *************************************************************/
  
  //Declaring the variables for the expression buffer and its length
    char expression[MAX_BUFF];
    size_t expressionLength;
  //printf("Proceeding in server: %s\n", __FILE__);
  //printf("\n inserver-1");
    while (1) {
        // Reading the expression from the pipe connected to the client
        // NULL terminate the string and store its length. 
        expressionLength = read(readfd, expression, sizeof(expression));
        expression[expressionLength] = '\0';
        // printf("\n inserver-2 %s", expression);
        // printf("%s: %s: %d: readfd: %d; Here we have value: %s; Length: %lu\n", 
        //               __FILE__, __FUNCTION__, __LINE__, readfd, expression, expressionLength); 
        // Check for termination condition
        if (strncmp(expression, "END", 3) == 0) {
        	// printf("\n inserver-3 %s", expression);
        	// printf("%s: %s: %d: END reached\n", __FILE__, __FUNCTION__, __LINE__);
//        	   write(writefd, "END", 4);
        break;
        }
        // printf("\n inserver-3 %ld", sizeof(expression));
        // Evaluate the expression using the calculate function
        // Use of 'calculate' function to evaluate the expression
        double result = calculate(expression);
        // printf("%s: %s: %d: return from calculate: %f\n", __FILE__, __FUNCTION__, __LINE__, result);
        // printf("\n inserver-4 %lf", result);
        // Format the result as a string
        char resultString[MAX_BUFF];
        memset(expression, '\0', MAX_BUFF - 1);
//      snprintf(resultString, sizeof(resultString), "%f", result);
        sprintf(expression, "%.3f", result);
//      printf("%s: %s: %d: sending it from server: %s\n", __FILE__, __FUNCTION__, __LINE__, expression);
//      sprintf(resultString, sizeof(resultString), "%f", result);
        // printf("\n inserver-5");
        // Write the result back to the client through the pipe
        write(writefd, expression, strlen(expression)); 
    }
}


