#include <array>
#include <fmt/base.h>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::array<int, 2> dims{3, 3};
  std::array<int, 2> periods{1, 0};
  MPI_Comm newcomm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims.data(), periods.data(), 0, &newcomm);

  int cart_rank;
  MPI_Comm_rank(newcomm, &cart_rank);
  std::array<int, 2> coords{};
  MPI_Cart_coords(newcomm, cart_rank, 2, coords.data());

  int source, destination;
  MPI_Comm exchange_comm = newcomm;
#if MPI_METHOD == 1
  MPI_Cart_shift(newcomm, 0, 1, &source, &destination);
#elif MPI_METHOD == 2
  std::array<int, 2> remain_dims{1, 0};
  MPI_Cart_sub(newcomm, remain_dims.data(), &exchange_comm);
  int column_rank, column_size;
  MPI_Comm_rank(exchange_comm, &column_rank);
  MPI_Comm_size(exchange_comm, &column_size);
  source = (column_rank + column_size - 1) % column_size;
  destination = (column_rank + 1) % column_size;
#else
#error "MPI_METHOD must be 1 or 2"
#endif

  std::array<int, 2> message{};
  MPI_Sendrecv(coords.data(), 2, MPI_INT, destination, 0, message.data(), 2,
               MPI_INT, source, 0, exchange_comm, MPI_STATUS_IGNORE);
  fmt::println("Process: {} coords:{}, {}. Message = {}, {}", rank, coords[0],
               coords[1], message[0], message[1]);

#if MPI_METHOD == 2
  MPI_Comm_free(&exchange_comm);
#endif
  MPI_Comm_free(&newcomm);
  MPI_Finalize();
  return 0;
}