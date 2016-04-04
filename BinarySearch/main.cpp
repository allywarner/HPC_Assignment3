//Ally Warner - u0680103
//Assignment 3
//High Performance Computing - CS 6230
//Parallel Binary Search - OpenMP + MPI

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <cstring>
#include <mpi.h>
//#include <omp.h>
#include <algorithm>

using namespace std;

void* parallelSearch(void* keys, void* arrayBase, size_t elementSize, size_t numKeys,size_t arrayLength, int (*compar)(const void*,const void*),size_t numThreads,MPI_Comm comm){
    
    
    //Start timer
    double startTime = MPI_Wtime();
    
    //Getting rank and size
    int rank,size;
    MPI_Comm_rank(comm,&rank);
    MPI_Comm_size(comm,&size);
    unsigned int numKeysPerProcess = size*numKeys;
    
    int *success = new int[numKeysPerProcess];
    void* item = NULL;
    void* allKeys = malloc(elementSize*numKeysPerProcess);
    MPI_Allgather(keys,numKeys*elementSize,MPI_BYTE,allKeys,numKeys*elementSize,MPI_BYTE, comm);

#pragma omp parallel
    for (int i = 0; i < arrayLength; i++) {
        item = bsearch (allKeys+i*elementSize, arrayBase, arrayLength, elementSize, compar);
        if(item == NULL){
            success[i] = 0;
        } else {
            success[i] = 1;
        }
    }
    
    double endTime = MPI_Wtime();
    double totalTime = endTime - startTime;
}

//MAIN FUNCTION
//Inputs:
int main(int argc, char* argv[]){
    
    //Throws an error if you do not input the length of the array to search
    if (argc < 2){
        cerr << "Error. Please input the length of the array to be searched.";
        return 1;
    }
    
    //inputs
    int arrayLength = atoi(argv[1]);
    int numKeys = atoi(argv[2]);

    //Initialize
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    //make arrays for inputs
    int *arrayBase = new int[arrayLength];
    int *keys = new int[numKeys];

    //Make a random sorted array with values from 0 to 20
    genNumbers<int>(arrayBase,arrayLength,0,20);
    std::sort(&arrayBase[0],&arrayBase[arrayLength-1]);
    
    //Generate random keys to look for between 0 and 20
    genNumbers<int>(keys,numKeys,0,20);
    
    //call search function
    parallelSearch(keys,arrayBase,sizeof(int),numKeys,arrayLength,comparisonOp<int>,4,MPI_COMM_WORLD);
    
    MPI_Finalize();
}