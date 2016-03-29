//Ally Warner - u0680103
//Assignment 3
//High Performance Computing - CS 6230
//Ring Communication

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <cstring>
#include <mpi.h>

using namespace std;

//MAIN FUNCTION
//Inputs: the numebr of times you'd like to go around the ring
int main(int argc, char* argv[]){
    
    //Initialize
    MPI_Init(&argc,&argv);
    
    //Throws an error if you do not input a number of times you'd like to go around the ring.
    if (argc < 2){
        cerr << "Error. Please input how many times you'd like the number of times you'd like the number to go around the ring.";
        return 1;
    }
    
    //Number of times you'd like to go around the ring
    int numAroundRing = atoi(argv[1]);
    
    //Getting the rank and the size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    //Initialize the integer to be passed around the ring as zero
    int number = 0;
    
    //Start the timing
    double startTime = MPI_Wtime();
    //Going around the ring as many times as specified
    for (int i = 0; i < numAroundRing; i++) {
        //checking for boundary conditions
        if (world_rank != world_size - 1) {
            MPI_Send(&number, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
        } else if (world_rank == world_size - 1) {
            MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        
        //checking for boundary conditions
        if (world_rank != 0) {
            MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            number += world_rank;
        } else if (world_rank == 0) {
            MPI_Recv(&number, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            number += world_rank;
        }
        
    }
    
    double endTime = MPI_Wtime();
    double totalTime = endTime - startTime;
    if(world_rank == 0)
        printf("Time to complete with %d time(s) around the ring: %f. \n",numAroundRing,totalTime);

    
    MPI_Finalize();
}