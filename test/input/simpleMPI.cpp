// forward declarations for MPI functions (we cannot include the header because MPI might not be available on the machine running the tests)
int MPI_Init(int *argc, char*** argv);
int PMPI_Init(int *argc, char*** argv) { return 0; }

typedef int MPI_Comm;
#define MPI_COMM_WORLD 0
int MPI_Comm_rank(MPI_Comm comm, int *rank);
int PMPI_Comm_rank(MPI_Comm comm, int *rank) { return 0; }

int MPI_Finalize();
int PMPI_Finalize() { return 0; }

#include <stdio.h>

void doSend() { }
void doRecv() { }

// This definition should be skipped if no prefix is used
int MPI_SomeTestFunction1(int x, int y)
{
	return x * y + 42;
}

// A forward declaration for PMPI_SomeTestFunction2 should be generated
int MPI_SomeTestFunction2(int x);
int PMPI_SomeTestFunction2(int x) { return 0; }

int main(int argc,char** argv)
{
	MPI_Init(&argc,&argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);	
	if (rank==0) doSend();
	if (rank==1) doRecv();

	MPI_SomeTestFunction1(2,3);
	MPI_SomeTestFunction2(0);
	MPI_Finalize();
}
