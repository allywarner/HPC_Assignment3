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
int main(int argc, char* argv[]){
    
    MPI_Init(NULL,NULL);
    
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int number;
    if (world_rank != 0) {
        MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process %d\n", world_rank, number,
               world_rank - 1);
    } else {
        // Set the number's value if you are process 0
        number = -1;
    }
    MPI_Send(&number, 1, MPI_INT, (world_rank + 1) % world_size, 0,
             MPI_COMM_WORLD);
    // Now process 0 can receive from the last process. This makes sure that at
    // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
    // deadlock)
    if (world_rank == 0) {
        MPI_Recv(&number, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process %d\n", world_rank, number,
               world_size - 1);
    }
    MPI_Finalize();
}
    
}