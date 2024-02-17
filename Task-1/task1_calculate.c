//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <unistd.h>
//#include <calc.h>


//double calculate(char *buf){

// /*************************************************************
//   Implement the expression evaluation functionality which
// will be invoked by the server whenever required.
//  *************************************************************/

//  double operands[20];
//  char op[20];
//  int oprnInd = 0;
//  double result = 0.0;

  // implement expression evaluation functionality here



// return(result);
//}
// ==================================================================================== //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <calc.h>

double cust_modulo(double b, double c) {
    return b - c * (int)(b / c);
}

double calculate(char *buf) {
    double operands[20];
    char op[20];
    int oprnInd = 0;
    double result = 0.0;
    
 // Declaring the char buffer for storing the operands and operations 
    char *buf_operands = buf;
    char *buf_operations = buf;
    int opIndex = 0;
    
 // Initializing the arrays numbers and operators with zeros/empty
    memset(operands, 0, sizeof(operands));
    memset(op, '\0', sizeof(op));


// Extracting operands from the buffer and storing into declared char * buf 
#if 0
    const char *delimiters = "+-*/%";
    char *token = strtok(buf, delimiters);
    while (token != NULL) {
        operands[(oprnInd)++] = strtof(token, NULL);
        token = strtok(NULL, delimiters);
        }
#endif
  //printf ("Operands are collected from the buffer & stored within operands %d\n", operint);
  //void printOperandsAndOperators(const float *operands, const char *op, int j) {
  // while (operands[j] != '\0') {
  //      printf("%d: Operands: %f, Operator: %c\n", __LINE__, operands[j], op[j]);
  //      j++;
    
// Extracting operations from the buffer and storing into declared opIndex variable
    while (*buf_operations != '\0') {
        switch (*buf_operations) {
            case '*':
            case '+':
            case '-':
            case '%':
            case '/':
                op[opIndex] = *buf_operations;
                opIndex++;
                buf_operations++;
                //printf("in process of capturing operations : %d: %c \n", opIndex, op[opIndex]);
                break;
            default:
                buf_operations++;
        }
    }

    for (int i = 0; i < opIndex; i++) {
		//printf("%s:%d:operands are: %c\n", __FILE__, __LINE__, op[i]);
	}
    const char *delimiters = "+-*/%";
    char *token = strtok(buf, delimiters);
    while (token != NULL) {
        operands[(oprnInd)++] = strtof(token, NULL);
        token = strtok(NULL, delimiters);
    }

  
  



	//printf("completed the operation captured : %d\n", opIndex);  
    int i = 0;
    int j = 0;
    while (operands[i] != '\0') {
        //printf("Entering into while loop :%dth time\n", i);
        switch (op[i]) {
            case '*':
                operands[i] *= operands[i + 1];
                j = i + 1;
                while (operands[j] != '\0') {
                    operands[j] = operands[j + 1];
                    op[j - 1] = op[j];
                    j = j + 1;
                }
         	//printf("%s: %d: j = %d; operand: %f; operation: %c\n", __FUNCTION__, __LINE__, i, operands[j], op[j]);
                break;
            case '/':
                operands[i] /= operands[i + 1];
                j = i + 1;
                while (operands[j] != '\0') {
                    operands[j] = operands[j + 1];
                    op[j - 1] = op[j];
                    j = j + 1;
                }
                //printf("%s: %d: j = %d; operand: %f; operation: %c\n", __FUNCTION__, __LINE__, i, operands[j], op[j]);
                break;
            case '%':
                operands[i] = cust_modulo(operands[i], operands[i + 1]);
                j = i + 1;
                while (operands[j] != '\0') {
                    operands[j] = operands[j + 1];
                    op[j - 1] = op[j];
                    j = j + 1;
                }
                //printf("%s: %d: j = %d; operand: %f; operation: %c\n", __FUNCTION__, __LINE__, i, operands[j], op[j]);
                break;
            default:
                i++;
                break;
        }
    }

	i = 0;
    	while (operands[i] != '\0') {
        	if (i == 0)
            		result = operands[i];
        	else {
            		switch (op[i - 1]) {
				case '+':
				    result += operands[i];
				    //printf("j = %d; result: %lf \n", __FUNCTION__, __LINE__, result, op[i]);
				    break;
				case '-':
				    result -= operands[i];
				    //printf("j = %d; result: %lf \n", __FUNCTION__, __LINE__, result, op[i]);
				    break;
				default:
				    result += operands[i];
				    //printf("j = %d; result: %lf \n", __FUNCTION__, __LINE__, result, op[i]);
				    break;
            		}
        	}	
        	i++;
      	}
      return result;
    }

