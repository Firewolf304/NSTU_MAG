#include <algorithm>
#include <filesystem>
#include <fmt/base.h>
#include <functional>
#include <iostream>
#include <mpi.h>

// libs
#include "../include/dotenv-cpp.hpp"

using LongType = long long;

int main(int argc, char **argv) {
  fmt::print("Filepath {}\n", std::filesystem::current_path().c_str());
  // dotenv::init(".env");

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double a = 0.0;         // start
  double b = 1.0;         // end
  LongType n = 1000000;   // iterations
  double h = (b - a) / n; // itration step

  double local_a, local_b;
  long local_n;
  auto f = [](double x) { return x * x; };

  if (rank == 0) {
    LongType basen = n / size;
    LongType r = n % size;
    double current_a = a;

    for (auto i = 0; i < size; ++i) {
      LongType CurrentN = basen + (i < r ? 1 : 0);

      double next = current_a + CurrentN * h;
      if (i == 0) // zero loop
      {
        local_a = current_a;
        local_b = next;
        local_n = CurrentN;
      } else {
        double send[3] = {current_a, next, static_cast<double>(CurrentN)};
        MPI_Send(send, 3, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
      current_a = next;
    }

  } else {
    double recv[3];
    MPI_Recv(recv, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    local_a = recv[0];
    local_b = recv[1];
    local_n = static_cast<long>(recv[2]);
  }

  double local_sum = 0.0;
  for (long i = 0; i < local_n; ++i) {

    double x_mid = local_a + (i + 0.5) * h;
    local_sum += f(x_mid);
  }
  double local_result = local_sum * h;

  if (rank == 0) {
    double total_result = local_result; // start from local result
    double remote_result;

    for (int i = 1; i < size; ++i) {
      MPI_Recv(&remote_result, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      total_result += remote_result;
    }

    fmt::println("worker result: {}", total_result);
  } else {
    // send result
    MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}