#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFF 500

int main(int argc, char **argv) {
	
    if (argc < 5) {
        perror("Required 5 arguments\n");
        exit(EXIT_FAILURE);
    }

    int user1_to_user2[2];
    int user2_to_user1[2];

    // Create pipes for communication
    if (pipe(user1_to_user2) == -1 || pipe(user2_to_user1) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t user1_pid, user2_pid;

    // Fork for user 1
    if ((user1_pid = fork()) == -1) {
        perror("Fork for user 1 failed");
        exit(EXIT_FAILURE);
    }
	

    if (user1_pid == 0) {  // Child process for user 1
        close(user1_to_user2[1]);  // Close unused write end
        close(user2_to_user1[0]);  // Close unused read end
        //dup2(user1_to_user2[0], STDIN_FILENO);  // Redirect standard input
        //dup2(user2_to_user1[1], STDOUT_FILENO); // Redirect standard output
        //close(user1_to_user2[0]);
        //close(user2_to_user1[1]);

        // Execute user program
        //execl("./task2_user", "./task2_user", "0", "1", argv[1], argv[2], (char *)NULL);
		//execl("./task2_user", "task2_user", "0", "1", argv[1], argv[2], NULL);
		char int_str1[5];
		char int_str2[5];
		sprintf(int_str1, "%d",user1_to_user2[0]);
		sprintf(int_str2, "%d",user2_to_user1[1]);
		//printf("Var1: %s\n", int_str1);
		//printf("Var2: %s\n", int_str2);
		execl("./Task-2/task2_user", "task2_user", int_str1, int_str2, argv[1], argv[2], NULL);

        perror("User 1 execl failed");
        exit(EXIT_FAILURE);
    }
	

    // Fork for user 2
    if ( user1_pid > 0 && (user2_pid = fork()) == -1) {
        perror("Fork for user 2 failed");
        exit(EXIT_FAILURE);
    }
	

    if (user2_pid == 0) {  // Child process for user 2
        close(user1_to_user2[0]);  // Close unused read end
        close(user2_to_user1[1]);  // Close unused write end
        //dup2(user2_to_user1[0], STDIN_FILENO);  // Redirect standard input
        //dup2(user1_to_user2[1], STDOUT_FILENO); // Redirect standard output
        //close(user2_to_user1[0]);
        //close(user1_to_user2[1]);

        // Execute user program
        //execl("./task2_user", "./task2_user", "1", "0", argv[3], argv[4], (char *)NULL);
		//execl("./task2_user", "task2_user", "1", "0", argv[3], argv[4], NULL);
		char int_str3[5];
		char int_str4[5];
		sprintf(int_str3, "%d",user1_to_user2[1]);
		sprintf(int_str4, "%d",user2_to_user1[0]);
		//printf("Var1: %s\n", int_str3);
		//printf("Var2: %s\n", int_str4);
		execl("./Task-2/task2_user", "task2_user", int_str4, int_str3, argv[3], argv[4], NULL);

        perror("User 2 execl failed");
        exit(EXIT_FAILURE);
    }

    // Close unused pipe ends in the parent
    /*close(user1_to_user2[0]);
    close(user1_to_user2[1]);
    close(user2_to_user1[0]);
    close(user2_to_user1[1]);*/

    // Wait for both user processes to finish
    waitpid(user1_pid, NULL, 0);
    waitpid(user2_pid, NULL, 0);

    return 0;
}

