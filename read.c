/* 
 
 Gonul AYCI and Mert TIFTIKCI, Bogazici University, Istanbul, 2016
 Read integers from a text file and then write them into 2D-array.
 To benefit from https://www.quora.com/How-can-I-read-unknown-number-of-integers-from-a-text-file-then-write-into-a-2D-array-in-C

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char *buffer;
	char *token;
	
	// read text file 
    FILE *file;
    file = fopen("input.txt", "r");
	
	// if file has no element then print "List is null"
	if (file == NULL){
		printf("List is null");	
		exit(1);
	}
	
	// line lengths are different...
	int firstLineLength = 0;
	int rowCount = 0, colnCount = 0;
	char c;

    while((c = fgetc(file)) != EOF) {
		if(colnCount == 0) {
			firstLineLength++;
		}
		if(c == '\n') {
			if(colnCount == 0) {
				fseek(file, 0, SEEK_SET);
				fgets(buffer, firstLineLength, file);
				token = strtok(buffer, " ");
				// TODO, find how many elements in token array!!
				// a better way
				do {
					colnCount++;
				} while ((token = strtok(NULL, " ")) != NULL);
				// stands for '\n' character in the end of the first line
				fgetc(file);
			}
			rowCount++;
		}
	}

	int i = 0, j = 0;

	int **array = malloc(rowCount * sizeof(int *));

	for(i = 0;i < rowCount;i++) {
		 array[i] = malloc(colnCount * sizeof(int));
	}
	// return the beggining of the file
	fseek(file, 0, SEEK_SET); 

	for(i = 0;i < rowCount;i++) {
		for(j = 0;j < colnCount;j++) {
			fscanf(file, "%d", &array[i][j]);
		}
	}

	for(i = 0;i < rowCount;i++) {
		for(j = 0;j < colnCount;j++) {
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}

	
    fclose(file);
	return 0;  
}