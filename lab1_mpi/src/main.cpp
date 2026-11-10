#include <algorithm>
#include <array>
#include <chrono>
#include <fmt/base.h>
#include <mpi.h>
#include <vector>

using LongType = long long;

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const double a = 0.0;
  const double b = 1.0;
  const LongType n = 1200000LL;
  const double h = (b - a) / n;
  const LongType basen = n / size;
  const LongType remainder = n % size;
  const LongType local_n = basen + (rank < remainder ? 1 : 0);
  auto f = [](double x) { return x * x; };

  auto bounds_for = [&](int worker) {
    LongType start = worker * basen + std::min<LongType>(worker, remainder);
    LongType count = basen + (worker < remainder ? 1 : 0);
    return std::array<double, 2>{a + start * h, a + (start + count) * h};
  };

  MPI_Barrier(MPI_COMM_WORLD);
  auto start = std::chrono::steady_clock::now();
  std::array<double, 2> local_bounds{};

#if MPI_METHOD == 1
  if (rank == 0) {
    local_bounds = bounds_for(0);
    for (int i = 1; i < size; ++i) {
      auto bounds = bounds_for(i);
      MPI_Send(bounds.data(), 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(local_bounds.data(), 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
#elif MPI_METHOD == 2
  std::vector<double> all_bounds;
  if (rank == 0) {
    all_bounds.resize(2 * size);
    for (int i = 0; i < size; ++i) {
      auto bounds = bounds_for(i);
      all_bounds[2 * i] = bounds[0];
      all_bounds[2 * i + 1] = bounds[1];
    }
  }
  MPI_Scatter(all_bounds.data(), 2, MPI_DOUBLE, local_bounds.data(), 2,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
#elif MPI_METHOD == 3
  local_bounds = bounds_for(rank);
#else
#error "MPI_METHOD must be 1/2/3"
#endif

  double local_sum = 0.0;
  for (LongType i = 0; i < local_n; ++i) {
    double x_mid = local_bounds[0] + (i + 0.5) * h;
    local_sum += f(x_mid);
  }
  double local_result = local_sum * h;
  double total_result = 0.0;

#if MPI_METHOD == 1
  if (rank == 0) {
    total_result = local_result;
    for (int i = 1; i < size; ++i) {
      double remote_result;
      MPI_Recv(&remote_result, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      total_result += remote_result;
    }
  } else {
    MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }
#elif MPI_METHOD == 2
  std::vector<double> results(rank == 0 ? size : 0);
  MPI_Gather(&local_result, 1, MPI_DOUBLE, results.data(), 1, MPI_DOUBLE, 0,
             MPI_COMM_WORLD);
  if (rank == 0) {
    for (double result : results)
      total_result += result;
  }
#elif MPI_METHOD == 3
  MPI_Reduce(&local_result, &total_result, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
#endif

  if (rank == 0) {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    fmt::println("Method: {}", MPI_METHOD);
    fmt::println("Integral: {:.15f}", total_result);
    fmt::println("Time: {:.6f} s", elapsed.count());
  }

  MPI_Finalize();
  return 0;
}