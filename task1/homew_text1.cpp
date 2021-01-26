#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int numprocs, myid, source;
	MPI_Status status;
	char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	if (myid != 0) { //
		sprintf(message, "hello this is %d", myid);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
	}
	else { // myid == 0
		for (source = 1; source < numprocs; source++) {
			int number_amount = 0;
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_CHAR, &number_amount);
			char recved[number_amount];
			MPI_Recv(recved, number_amount, MPI_CHAR, MPI_ANY_SOURCE, 99,
				MPI_COMM_WORLD, &status);
			printf("recved number %d messages ：%s\n", source, recved);
		}
	}
	MPI_Finalize();
} /* end main */