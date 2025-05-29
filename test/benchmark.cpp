#include "linkedlistmedian.h"

#include <catch2/catch_test_macros.hpp>
// Also include benchmarking headers, i don't really know which one is necessary
#include <catch2/benchmark/catch_benchmark.hpp>
#include <vector>

template <typename T, int medlen>
void baseline_nth_element(const std::vector<T>& x, std::vector<T>& med){
  std::vector<T> m(medlen);
  for (int i = 0; i < x.size() - medlen + 1; ++i){
    m.assign(x.begin() + i, x.begin() + i + medlen);
    std::nth_element(m.begin(), m.begin() + medlen / 2, m.end());
    med.at(i) = m.at(medlen / 2);
  }
}

TEST_CASE("baseline std::nth_element", "[baseline],[std],[nth_element]") {
  SECTION("int, length 1000"){
    constexpr size_t length = 1000;
    std::vector<int> x(length);
    for (auto& v : x)
      v = std::rand() % 256;

    // Overallocated but never mind
    std::vector<int> med(length);

    BENCHMARK("median length 3"){
      baseline_nth_element<int, 3>(x, med);
    };
    BENCHMARK("median length 19"){
      baseline_nth_element<int, 19>(x, med);
    };
    BENCHMARK("median length 99"){
      baseline_nth_element<int, 99>(x, med);
    };
  }

  SECTION("int, length 10000"){
    constexpr size_t length = 10000;
    std::vector<int> x(length);
    for (auto& v : x)
      v = std::rand() % 256;

    // Overallocated but never mind
    std::vector<int> med(length);

    BENCHMARK("median length 3"){
      baseline_nth_element<int, 3>(x, med);
    };
    BENCHMARK("median length 19"){
      baseline_nth_element<int, 19>(x, med);
    };
    BENCHMARK("median length 99"){
      baseline_nth_element<int, 99>(x, med);
    };
  }
}

template <typename T, int medlen>
void benchmark_linkedlistmedian(const std::vector<T>& x, std::vector<T>& med){
  LinkedListMedian<T> m(x.begin(), x.begin()+medlen);
  med.at(0) = m.median();
  for (int i = 1; i < x.size() - medlen + 1; ++i){
    m.replace(x.at(i+medlen-1), x.at(i-1));
    med.at(i) = m.median();
  }
}

TEST_CASE("LinkedListMedian", "[ammf],[linkedlistmedian]"){
  SECTION("int, length 1000"){
    constexpr size_t length = 1000;
    std::vector<int> x(length);
    for (auto& v : x)
      v = std::rand() % 256;

    // Overallocated but never mind
    std::vector<int> med(length);

    BENCHMARK("median length 3"){
      benchmark_linkedlistmedian<int, 3>(x, med);
    };
    BENCHMARK("median length 19"){
      benchmark_linkedlistmedian<int, 19>(x, med);
    };
    BENCHMARK("median length 99"){
      benchmark_linkedlistmedian<int, 99>(x, med);
    };
  }

  SECTION("int, length 10000"){
    constexpr size_t length = 10000;
    std::vector<int> x(length);
    for (auto& v : x)
      v = std::rand() % 256;

    // Overallocated but never mind
    std::vector<int> med(length);

    BENCHMARK("median length 3"){
      benchmark_linkedlistmedian<int, 3>(x, med);
    };
    BENCHMARK("median length 19"){
      benchmark_linkedlistmedian<int, 19>(x, med);
    };
    BENCHMARK("median length 99"){
      benchmark_linkedlistmedian<int, 99>(x, med);
    };
  }
}
