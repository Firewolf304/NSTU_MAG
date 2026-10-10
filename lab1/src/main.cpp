// #include "mpi.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fmt/base.h>
#include <functional>
#include <generator>
#include <iostream>
#include <latch>
#include <omp.h>
#include <thread>
#include <vector>

// libs
#include "../include/dotenv-cpp.hpp"

using namespace std::chrono_literals;
using ResultType = __int128;

int main() {
  // std::atomic<int> x{0};
  alignas(128) ResultType x = 0;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  fmt::print("Filepath {}\n", std::filesystem::current_path().c_str());

  // auto threads = num_threads = std::thread::hardware_concurrency();

  int a = 0;
  int b = -1;
  std::cin >> b;
  // 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10
  // liner
  /*for (double a = 1; a <= b; a++) {
    x += a;
  }*/

  // parallel
  // raw thread
  // Not thread security: 1 block sum = 1 thread -> Thread overflow
  /*std::vector<std::jthread> funcs;
  std::atomic<int> current_index{0};
  std::latch start_gate(b + 1);
  std::latch end_gate(b + 1);
  std::generate_n(std::back_inserter(funcs), b + 1, [&]() {
    int i = current_index.fetch_add(1, std::memory_order_relaxed);
    return std::jthread(
        [&x, &start_gate, &end_gate](int i) {
          start_gate.arrive_and_wait();

          x += i;
          std::this_thread::sleep_for(10ms);
          end_gate.arrive_and_wait();
        },
        i);
  });
  funcs.clear();*/

  const unsigned HardwareThreads =
      std::max(1u, std::thread::hardware_concurrency() * 2);

  const int parallels = b + 1;
  const unsigned ThreadCount = std::min<unsigned>(HardwareThreads, parallels);

  std::vector<ResultType> partial_sums(ThreadCount);
  std::vector<std::jthread> threads;
  threads.reserve(ThreadCount);
  fmt::print("hardware={}, iterations={}, threads={}\n", HardwareThreads,
             parallels, ThreadCount);
  for (unsigned thread_id = 0; thread_id < ThreadCount; ++thread_id) {
    threads.emplace_back([&, thread_id] {
      const int begin = static_cast<int>(static_cast<long long>(parallels) *
                                         thread_id / ThreadCount);
      const int end = static_cast<int>(static_cast<long long>(parallels) *
                                       (thread_id + 1) / ThreadCount);
      long long local_sum = 0;

      // chunk summ
      for (int i = begin; i < end; ++i) {
        local_sum += i; // f(x)
      }
      partial_sums[thread_id] = local_sum;
    });
  }
  threads.clear();

  for (const auto &partial : partial_sums) {
    x += partial;
  }

  // OMP
  /*#pragma omp parallel for reduction(+ : x)
    for (int i = 0; i <= b; i++) {
      x += i;
    }
  */
  fmt::println("{}", x);
  return 0;
}