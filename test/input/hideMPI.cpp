// forward declarations for MPI functions (we cannot include the header because MPI might not be available on the machine running the tests)
int MPI_Init(int *argc, char*** argv) { return 0; }
int PMPI_Init(int *argc, char*** argv) { return 0; }

typedef int MPI_Comm;
#define MPI_COMM_WORLD 0
int MPI_Comm_rank(MPI_Comm comm, int *rank);
int PMPI_Comm_rank(MPI_Comm comm, int *rank) { return 0; }

typedef int MPI_Datatype;
#define MPI_INT 0
typedef void* MPI_Status;
#define MPI_STATUS_IGNORE NULL
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) { return 0; }
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status) { return 0; }

int MPI_Finalize() { return 0; }
int PMPI_Finalize() { return 0; }

#include <stdio.h>

void doSend() { }
void doRecv() { }

int main(int argc,char** argv)
{
	MPI_Init(&argc,&argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if (rank == 0) {
		int data = 42;
		MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else {
		int data;
		MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	MPI_Finalize();
}
