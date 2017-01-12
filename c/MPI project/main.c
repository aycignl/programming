	/* 
	
	Student Name: Gonul AYCI
	Student Number: 2016800003
	Compile Status: Compiling
	Program Status: Working
	Notes: 
	
	*/

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <mpi.h>

	//These are my functions: defined in last part of this c file
	void applyKernel(int **inputImage, int **outputImage, double filterMatrix[3][3], int inputRow, int inputColn);
	void applyThreshold(int **edge1, int **edge2, int **edge3, int **edge4, int **output, int inputRows, int inputCols, int threshold);
	void applyWrite(int **inputImage, char *fileName, int inputRows, int inputCols);
	void readImage(int **image, char *fileName, int *inputRows, int *inputCols);
	void applySmooth(int **inputImage, int **outputImage, int inputRow, int inputColn);

	int main(int argc, char **argv){

	int rank = 0, size = 0;
	int threshold = 40;


		MPI_Init(&argc, &argv); //Initialize the MPI execution environment 
		MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Determines the rank of the calling process in the communicator 
		MPI_Comm_size(MPI_COMM_WORLD, &size); //Determines the size of the group associated with a communicator 

		// rowCount: #of rows, colnCount: #of Columns
		int rowCount = 0, colnCount = 0;
		int i = 0, j = 0;

		threshold = atoi(argv[3]);

		//read data from input file using by readImage function (defined at the end of this file)
		//allocate an array which length is 200 by 200
		if(rank == 0){
			int **array = malloc(200 * sizeof(int *));

			for(i = 0; i < 200;i++) {
				array[i] = malloc(200 * sizeof(int));
			}
			//readImage is a function which reads input.txt file & rowCount:# of row & colnCount:# of column
			//readImage(array, "input.txt", &rowCount, &colnCount);
			readImage(array, argv[1], &rowCount, &colnCount);

		//distribute data among slave processes
		//size = 5, rowCount = 200, partSize = 50
		int partSize = rowCount / (size-1);

		for(i = 1; i < size; i++){ // to distribute the data to the # of processors.  
			for(j = 0; j < partSize; j++){ // upper bound of each slave processes: row size
				MPI_Send(&array[(i-1)*partSize+j][0], colnCount, MPI_INT, i, 0, MPI_COMM_WORLD);
				//btw. (0,0)-(49,0)-->Slave1(by 200), (50,0)-(99,0)-->Slave2(by 200) and so on.
				//to send from processor i(slaves) to 0(master)
					}

				}    
			}
		//receive data from Master process
		int partSize = 200 / (size-1);
		int **recArr ;

		if(rank == 1){ // first slave processor

			recArr = malloc((partSize +1)* sizeof(int *)); //to set 50 length

			// receive data from Master process(rank == 0)
			for(i = 0; i < partSize; i++) {
				recArr[i] = malloc(200 * sizeof(int)); //dim(recArr) = 50 by 200
				MPI_Recv(&recArr[i][0], 200, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}

		} else if(rank == size-1){ // last slave processor

			recArr = malloc((partSize +1)* sizeof(int *)); 

			for(i = 1; i < partSize+1; i++) {
				recArr[i] = malloc(200 * sizeof(int)); 
				MPI_Recv(&recArr[i][0], 200, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}

		}else if(rank != 0){ // not first, not last, not Master processor

			recArr = malloc((partSize+2)* sizeof(int *)); 

			for(i = 1; i < partSize+1; i++) {
				recArr[i] = malloc(200 * sizeof(int)); 
				MPI_Recv(&recArr[i][0], 200, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}

		}

		//send data to other processes
		//first slave sends its last row to next slave process
		//last one sends its first row to previous slave process
		//it processes between first and last slave then they send their data to their neighbour (which are first and last slave)
		if(rank == 1 ){

		MPI_Send(&recArr[partSize-1][0], 200, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

		} else if(rank == size-1){
		MPI_Send(&recArr[1][0], 200, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

		}   else if(rank != 0){
		MPI_Send(&recArr[1][0], 200, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
		MPI_Send(&recArr[partSize][0], 200, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

		}
		
		//MPI_Barrier avoids any comfliction
		MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine. 

		//receive data from other processes (it is the same as send part)
		//and they receive data from first and last slave
		if(rank == 1){
			recArr[partSize] = malloc(200*sizeof(int));
			MPI_Recv(&recArr[partSize][0], 200, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		}else if(rank == size -1){
			recArr[0] = malloc(200*sizeof(int));
			MPI_Recv(&recArr[0][0], 200, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		}else if(rank != 0){
			recArr[0] = malloc(200*sizeof(int));
			recArr[partSize+1] = malloc(200*sizeof(int));
			MPI_Recv(&recArr[0][0], 200, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&recArr[partSize+1][0], 200, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}
			MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine. 

		//outSmooth is smooth data file
		//smoothing of 200 by 200 matrix with a 3 by 3 mean filter matrix by doing successive convolution steps
		//smoothed image is of size (N-2) by (N-2) 
		int **outSmooth;
		double meanFilter[3][3];

		for(i = 0; i < 3; i++){
			for(j = 0;j < 3; j++){
				meanFilter[i][j] = 1.0/9.0;
			}
		}
		char fileName[50];
		sprintf(fileName, "edge%d.txt", rank);

		if(rank == 1){
				outSmooth = malloc((partSize)* sizeof(int *)); 
				for(i = 0; i < partSize;i++) {
				outSmooth[i] = malloc(198 * sizeof(int));
			}
			
		applySmooth(recArr, outSmooth, partSize+1, 200);
		//applyWrite(outSmooth, fileName, partSize-1, 198);

		}else if(rank == size-1){
				outSmooth = malloc((partSize)* sizeof(int *)); 
				for(i = 0; i < partSize;i++) {
				outSmooth[i] = malloc(198 * sizeof(int));
			}
		applySmooth(recArr, &outSmooth[1], partSize+1, 200);

		}
		else if(rank != 0){
				outSmooth = malloc((partSize+2)* sizeof(int *)); 
				for(i = 0; i < partSize+2;i++) {
				outSmooth[i] = malloc(198 * sizeof(int));
			}
		applySmooth(recArr, &outSmooth[1], partSize+2, 200);

		}
/*
		char smtFile[20];
		sprintf(smtFile, "smt%d.txt", rank);

		if (rank != 0) {
			if (rank == 1) {
				applyWrite(outSmooth, smtFile, 49, 198);
			} else if (rank == size-1) {
				applyWrite(&outSmooth[1], smtFile, 49, 198);
			} else {
				applyWrite(&outSmooth[1], smtFile, 50, 198);
			}

		}*/
		
	if(rank == 1 ){
		MPI_Send(&outSmooth[partSize-2][0], 198, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

		} else if(rank == size-1){
		MPI_Send(&outSmooth[1][0], 198, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

		}   else if(rank != 0){
		MPI_Send(&outSmooth[1][0], 198, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
		MPI_Send(&outSmooth[partSize-1][0], 198, MPI_INT, rank+1, 0, MPI_COMM_WORLD);

		}
		
		MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine. 


		if(rank == 1){
		MPI_Recv(&outSmooth[partSize-1][0], 198, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else if(rank == size -1){
		MPI_Recv(&outSmooth[0][0], 198, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else if(rank != 0){
		MPI_Recv(&outSmooth[0][0], 198, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&outSmooth[partSize+1][0], 198, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine.

		
		//apply kernel
		//now, I can do similar convolution procedures with four different 3 by 3 matrices.
		//the first filter for detecting lines in a given image detects horizontal lines --> horizontalMatrix
		//the second filter for detecting lines in a given image detects vertical lines --> verticalMatrix
		//the third filter for detecting lines in a given image detects oblique lines (+45) --> pObliqueMatrix
		//the last filter for detecting lines in a given image detects oblique lines (-45) --> nObliqueMatrix
		int **outKernel;

		double horizontalMatrix[3][3];

		horizontalMatrix[0][0] = -1;
		horizontalMatrix[0][1] = -1;
		horizontalMatrix[0][2] = -1;
		horizontalMatrix[1][0] = 2;
		horizontalMatrix[1][1] = 2;
		horizontalMatrix[1][2] = 2;
		horizontalMatrix[2][0] = -1;
		horizontalMatrix[2][1] = -1;
		horizontalMatrix[2][2] = -1;	

		double verticalMatrix[3][3];

		verticalMatrix[0][0] = -1;
		verticalMatrix[0][1] = 2;
		verticalMatrix[0][2] = -1;
		verticalMatrix[1][0] = -1;
		verticalMatrix[1][1] = 2;
		verticalMatrix[1][2] = -1;
		verticalMatrix[2][0] = -1;
		verticalMatrix[2][1] = 2;
		verticalMatrix[2][2] = -1;

		double pObliqueMatrix[3][3];

		pObliqueMatrix[0][0] = -1;
		pObliqueMatrix[0][1] = -1;
		pObliqueMatrix[0][2] = 2;
		pObliqueMatrix[1][0] = -1;
		pObliqueMatrix[1][1] = 2;
		pObliqueMatrix[1][2] = -1;
		pObliqueMatrix[2][0] = 2;
		pObliqueMatrix[2][1] = -1;
		pObliqueMatrix[2][2] = -1;

		double nObliqueMatrix[3][3];

		nObliqueMatrix[0][0] = 2;
		nObliqueMatrix[0][1] = -1;
		nObliqueMatrix[0][2] = -1;
		nObliqueMatrix[1][0] = -1;
		nObliqueMatrix[1][1] = 2;
		nObliqueMatrix[1][2] = -1;
		nObliqueMatrix[2][0] = -1;
		nObliqueMatrix[2][1] = -1;
		nObliqueMatrix[2][2] = 2;

		int **edgeImage1;
		int **edgeImage2;
		int **edgeImage3;
		int **edgeImage4;

		int **outputEdge;
		
		// allocate edgeImages
		// create edge images matrices for four filters lines detectors
		// pay attention to their sizes
		if(rank == 1 || rank == size-1){
			edgeImage1 = malloc((partSize-2) * sizeof(int *));
			edgeImage2 = malloc((partSize-2) * sizeof(int *));
			edgeImage3 = malloc((partSize-2) * sizeof(int *));
			edgeImage4 = malloc((partSize-2) * sizeof(int *));
			outputEdge = malloc((partSize-2) * sizeof(int *));
		
		for(i = 0; i < partSize-2; i++){
			edgeImage1[i] = malloc((196) * sizeof(int));
			edgeImage2[i] = malloc((196) * sizeof(int));
			edgeImage3[i] = malloc((196) * sizeof(int));
			edgeImage4[i] = malloc((196) * sizeof(int));
			outputEdge[i] = malloc((196) * sizeof(int));


		}
		} else if(rank != 0){
			edgeImage1 = malloc((partSize) * sizeof(int *));
			edgeImage2 = malloc((partSize) * sizeof(int *));
			edgeImage3 = malloc((partSize) * sizeof(int *));
			edgeImage4 = malloc((partSize) * sizeof(int *));
			outputEdge = malloc((partSize) * sizeof(int *));
		
		for(i = 0; i < partSize; i++){
			edgeImage1[i] = malloc((196) * sizeof(int));
			edgeImage2[i] = malloc((196) * sizeof(int));
			edgeImage3[i] = malloc((196) * sizeof(int));
			edgeImage4[i] = malloc((196) * sizeof(int));
			outputEdge[i] = malloc((196) * sizeof(int));

		}
		}

		// this is my result part. 
		//First of all, I apply kernel edges to my Smoothed image
		//Then, to apply threshold
		//Finally, to write output 
		if((rank == 1 || rank == size-1) && size != 101){
		applyKernel(outSmooth, edgeImage1, horizontalMatrix, partSize, 198);
		applyKernel(outSmooth, edgeImage2, verticalMatrix,  partSize, 198);
		applyKernel(outSmooth, edgeImage3, pObliqueMatrix,  partSize, 198);
		applyKernel(outSmooth, edgeImage4, nObliqueMatrix,  partSize, 198);
		applyThreshold(edgeImage1, edgeImage2, edgeImage3, edgeImage4, outputEdge, partSize-2, 196, threshold);
		//applyWrite(outputEdge, fileName, partSize-2, 196);
		
		}else if(rank != 0){
				
		applyKernel(outSmooth, edgeImage1, horizontalMatrix, partSize+2, 198);
		applyKernel(outSmooth, edgeImage2, verticalMatrix, partSize+2, 198);
		applyKernel(outSmooth, edgeImage3, pObliqueMatrix, partSize+2, 198);
		applyKernel(outSmooth, edgeImage4, nObliqueMatrix, partSize+2, 198);
		applyThreshold(edgeImage1, edgeImage2, edgeImage3, edgeImage4, outputEdge, partSize, 196, threshold);
		//applyWrite(outputEdge, fileName, partSize, 196);

		}
		
		if((rank == 1 || rank == size-1) && size != 101){
			for(i=0; i<partSize-2; i++){

				MPI_Send(&outputEdge[i][0], 196, MPI_INT, 0, 0, MPI_COMM_WORLD);

			}

		}else if(rank != 0){
			for(i=0; i<partSize; i++){

				MPI_Send(&outputEdge[i][0], 196, MPI_INT, 0, 0, MPI_COMM_WORLD);

			}
		}
		MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine. 

		if(rank == 0){
			int **result;
			result = malloc(196 * sizeof(int *));
			for(i=0; i< 196; i++){
				result[i] = malloc(196 * sizeof(int));
			}
			for(i=0; i<partSize-2; i++){
				if (size != 101)
					MPI_Recv(&result[i][0], 196, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}
			for(j=2; j< size-1; j++){
				for(i=0; i<partSize; i++){
				MPI_Recv(&result[(partSize-2)+(j-2)*partSize + i][0], 196, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				}
			}

			for(i=0; i<partSize-2; i++){
				if (size != 101)
					MPI_Recv(&result[196-(partSize-2)+i][0], 196, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			}

		//applyWrite(result, "output10.txt", 196, 196);
		applyWrite(result, argv[2], 196, 196);


		}

		MPI_Barrier(MPI_COMM_WORLD);//Blocks until all processes in the communicator have reached this routine. 
		MPI_Finalize();//Terminates MPI execution environment 
/*
		// Now, I delete space from memory which means allocate --> deallocate:free
		if(rank == 1 || rank == size-1){
			for(i=0; i<partSize+1; i++){
				free(recArr[i]);
			} 
			free(recArr);
		} else if(rank != 0){
			for(i=0; i<partSize+2; i++){
				free(recArr[i]);
			} 
			free(recArr);

		}
*/

		return 0;  

	}

	// definitions if functions
	// smooth input image with (n-2, n-2)
	// I change smooth function when Metehan Doyran's e-mail
	void applySmooth(int **inputImage, int **outputImage, int inputRow, int inputColn){

		double sum = 0.0;
		int j=0, i=0, k=0, l=0;
		for(i = 1; i < inputRow-1; i++){
			for(j = 1; j < inputColn-1; j++){
					double sum = 0.0;
				for(k=0; k<3; k++){
					for(l=0; l<3; l++){
							sum += inputImage[i-1+k][j-1+l];

			}
		}
		outputImage[i-1][j-1] = sum/9.0;

			}
		}

	}

	// this is Kernel function
	// get the values around marked index
	void applyKernel(int **inputImage, int **outputImage, double filterMatrix[3][3], int inputRow, int inputColn){

		int i = 0, j = 0;
		
		for(i = 1; i < inputRow-1; i++){
			for(j = 1; j < inputColn-1; j++){
				
				outputImage[i-1][j-1] = 
				inputImage[i-1][j-1]*filterMatrix[0][0] + inputImage[i-1][j]*filterMatrix[0][1] + inputImage[i-1][j+1]*filterMatrix[0][2]
				+ inputImage[i][j-1]*filterMatrix[1][0] + inputImage[i][j]*filterMatrix[1][1] + inputImage[i][j+1]*filterMatrix[1][2]
				+ inputImage[i+1][j-1]*filterMatrix[2][0] + inputImage[i+1][j]*filterMatrix[2][1] + inputImage[i+1][j+1]*filterMatrix[2][2];

			}
		}

	}

	// create a single binary image by thresholding together
	// apply threshold to get 0 or 255 matrix
	void applyThreshold(int **edge1, int **edge2, int **edge3, int **edge4, int **output, int inputRows, int inputCols, int threshold){

		int i = 0, j = 0;

		for(i = 0; i < inputRows; i++){
			for(j = 0; j < inputCols; j++){
				if(edge1[i][j] > threshold || edge2[i][j] > threshold || edge3[i][j] > threshold || edge4[i][j] > threshold){
					output[i][j] = 255;
				} else{
					output[i][j] = 0;		
				}
			}
		}

	}

	// write outputs to txt file
	void applyWrite(int **inputImage, char *fileName, int inputRows, int inputCols){

		int i = 0, j = 0;

		FILE *edgeWrite;
		edgeWrite = fopen(fileName, "w");

		for(i = 0; i < inputRows; i++){
			for(j = 0; j < inputCols; j++){
				fprintf(edgeWrite, "%d ", inputImage[i][j]); //write to file
			}
			fprintf(edgeWrite, "\n");
		}


	}

	// read txt (image) file with dynamically. It works when the array size change
	void readImage(int **image, char *fileName, int *inputRows, int *inputCols){

		char buffer[1000];
		char *token;
		
		// read text file 
		FILE *file;
		file = fopen(fileName, "r");
		
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
			*inputRows = rowCount;
			*inputCols = colnCount;

			int i = 0, j = 0;

		// return the beginning of the file
		fseek(file, 0, SEEK_SET); 

		for(i = 0; i < rowCount;i++) {
			for(j = 0; j < colnCount;j++) {
				fscanf(file, "%d", &image[i][j]);
			}
		}


	}