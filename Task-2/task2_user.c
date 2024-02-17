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

    int readfd = atoi(argv[1]);   // read handle of channel
    int writefd = atoi(argv[2]);  // write handle of channel
    char *chat_file = argv[3];    // chat content file
    char *store_file = argv[4];   // store file
    FILE *content, *store;        // file stream to read your chat content and store other user's message

    // Open the chat content file for reading
    content = fopen(chat_file, "r");
    if (content == NULL) {
        perror("Error opening chat content file");
        exit(EXIT_FAILURE);
    }

    // Open the store file for writing
    store = fopen(store_file, "w");
    if (store == NULL) {
        perror("Error opening store file");
        fclose(content);
        exit(EXIT_FAILURE);
    }

    char line[MAX_BUFF];
	char lineOth[MAX_BUFF];

    // Read each line from the chat content file
	int counter=0;
    while (fgets(line, sizeof(line), content) != NULL) {
        // Write the line to the communication channel (pipe)
		if(strcmp(line, "initiate\n")==0) {
			counter++;
			continue;
		}
		
		if(counter==1) {
			if( write(writefd, line, strlen(line)) != strlen(line)){
				perror("write");
				exit(-1);
			}
			
			if(strcmp(line, "bye\n")==0) {
				return 0;
			}

			// Read the response from the other user through the communication channel
			memset(lineOth, 0, sizeof lineOth);
			while( read(readfd, lineOth, 500) <=0 ){
			}
			
			//ssize_t bytes_read = read(readfd, line, sizeof(line));
			int linesize=0;
			for(int i = 0 ; lineOth[i]!='\n'; i++ ) {
				lineOth[i]=lineOth[i];
				linesize=i+1;
			}
			lineOth[linesize]='\n';
			// Write the response to the store file
			if(linesize>0){
				char byechk[linesize+1];
				memset(byechk, 0, sizeof byechk);
				for(int i=0;i<linesize;i++){
					byechk[i]=lineOth[i];
				}
				byechk[linesize+1]='\0';
				if(strcmp(byechk, "bye")==0) {
					return 0;
				}
				else{
					fwrite(lineOth, 1, linesize+1, store);
					fflush(store); // Flush the buffer to ensure data is written immediately
				}
			}
			else{
				counter=5;
			}
		}
		
		if(counter==0){
			memset(lineOth, 0, sizeof lineOth);
			while( read(readfd, lineOth, 500) <=0 ){
			}
			int linesize=0;
			for(int i = 0 ; lineOth[i]!='\n'; i++ ) {
				lineOth[i]=lineOth[i];
				linesize=i+1;
			}
			lineOth[linesize]='\n';
			// Write the response to the store file
			if(linesize>0){
				char byechk[linesize+1];
				memset(byechk, 0, sizeof byechk);
				for(int i=0;i<linesize;i++){
					byechk[i]=lineOth[i];
				}
				byechk[linesize+1]='\0';
				if(strcmp(byechk, "bye")==0) {
					return 0;
				}
				else{
					fwrite(lineOth, 1, linesize+1, store);
					fflush(store); // Flush the buffer to ensure data is written immediately
				}
			}
			else{
				counter=5;
			}
			
			if( write(writefd, line, strlen(line)) != strlen(line)){
				perror("write");
				exit(-1);
			}
			
			if(strcmp(line, "bye\n")==0) {
				return 0;
			}
			
		}
		
		if(counter==5){
			if( write(writefd, line, strlen(line)) != strlen(line)){
				perror("write");
				exit(-1);
			}
			
			if(strcmp(line, "bye\n")==0) {
				return 0;
			}
		}
    }

    // Close the file streams and communication channel
    fclose(content);
    fclose(store);

    return 0;
}