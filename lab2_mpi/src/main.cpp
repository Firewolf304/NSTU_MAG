#include <array>
#include <cstdio>
#include <cstring>
#include <fmt/base.h>
#include <mpi.h>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // какая-то группа (допустим чет/нечет)
  MPI_Comm even_comm = MPI_COMM_NULL;
  MPI_Comm odd_comm = MPI_COMM_NULL;

// by communicator
#if MPI_METHOD == 1
  if (rank == 0)
    fmt::println("Method 1");
  MPI_Group world_group, even_group, odd_group;
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);
  std::vector<int> even_ranks;
  std::vector<int> odd_ranks;
  for (int i = 0; i < size; ++i)
    if (i % 2)
      odd_ranks.push_back(i);
    else
      even_ranks.push_back(i);
  // even group
  MPI_Group_incl(world_group, static_cast<int>(even_ranks.size()),
                 even_ranks.data(), &even_group);
  MPI_Comm_create(MPI_COMM_WORLD, even_group, &even_comm);

  // odd group
  MPI_Group_incl(world_group, static_cast<int>(odd_ranks.size()),
                 odd_ranks.data(), &odd_group);
  MPI_Comm_create(MPI_COMM_WORLD, odd_group, &odd_comm);

  // free resources!
  MPI_Group_free(&even_group);
  MPI_Group_free(&odd_group);
  MPI_Group_free(&world_group);

// by communicator split
#elif MPI_METHOD == 2
  if (rank == 0)
    fmt::println("Method 2");
  MPI_Comm_split(MPI_COMM_WORLD, rank % 2 == 0 ? 0 : MPI_UNDEFINED, rank,
                 &even_comm);
  MPI_Comm_split(MPI_COMM_WORLD, rank % 2 != 0 ? 0 : MPI_UNDEFINED, rank,
                 &odd_comm);
#else
#error "MPI_METHOD must be 1 or 2"
#endif

  std::array<char, 128> message{};
  MPI_Comm new_comm = rank % 2 == 0 ? even_comm : odd_comm;
  if (new_comm != MPI_COMM_NULL) {
    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);
    if (new_rank == 0) {
      if (rank % 2 == 0)
        std::strcpy(message.data(), "Even");
      else
        std::strcpy(message.data(), "Odd");
    }
    MPI_Bcast(message.data(), static_cast<int>(message.size()), MPI_CHAR, 0,
              new_comm);
    fmt::println("MPI_COMM_WORLD: Rank={} (from {}). New_comm={} (from {}). "
                 "Message = {}\n",
                 rank, size, new_rank, new_size, message.data());
  }

  if (even_comm != MPI_COMM_NULL)
    MPI_Comm_free(&even_comm);
  if (odd_comm != MPI_COMM_NULL)
    MPI_Comm_free(&odd_comm);

  MPI_Finalize();
  return 0;
}