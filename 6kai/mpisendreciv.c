#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    int send_value, recv_value = -1;
    int nprocs, rank, namelen;
    const int TAG = 0;
    MPI_Status st;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &namelen);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    send_value = rank;
    if (rank == 0) {
        MPI_Sendrecv(&send_value, 1, MPI_INT, 1, TAG,
        &recv_value, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &st);
    } else if (rank == 1) {
        MPI_Sendrecv(&send_value, 1, MPI_INT, 0, TAG,
        &recv_value, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &st);
    }
    printf("%s(%d/%d): recv_value = %d\n",
    name, rank, nprocs, recv_value);
    MPI_Finalize();
}