#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int nprocs, rank, namelen;
    char name[MPI_MAX_PROCESSOR_NAME];
    int value = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &namelen);
    // rank 0のプロセスだけが入力を受け付ける
    if (rank == 0) {
        scanf("%d", &value);
    }
    // broadcast
    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("%s(%d/%d): value = %d\n", name, rank, nprocs, value);
    MPI_Finalize();
}