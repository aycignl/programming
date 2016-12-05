// Gonul AYCI, Bogazici University, Istanbul, 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char *lineStr;
	char *token;

	
	FILE *file;
    file = fopen("input.txt", "r");
	
	if (file == NULL){
		printf("null");	
		exit(1);
	}
	
	while(fgets(lineStr, 4000, file) != NULL){
		
		token = strtok(lineStr, " ");
		printf("%s\n", token );
		token = strtok(NULL, " ");
		printf("%s\n", token);
		
		while ((token = strtok(NULL, " ")) != NULL ){
			printf("%s\n", token);
		}
	}
	
    fclose(file);
	return 0;  
}