#include <charconv>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

struct Stats {
  size_t size;
  size_t bucket_count;
  float load_factor;
  float max_load_factor;
};

int main(int arc, char* argv[]) {
  unordered_set<int> numbers;
  vector<Stats> stats;
  vector<size_t> insert_durations;
  int item_count = atoi(argv[1]);
  numbers.max_load_factor(atoi(argv[2]) / 10.0);
  for (int i = 0; i < 240; ++i) {
    numbers.insert(1'000'000 + i);
  }
  for (int i = 0; i < item_count; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    numbers.insert(i);
    insert_durations.push_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - start).count());
    stats.push_back({.size = numbers.size(),
                     .bucket_count = numbers.bucket_count(),
                     .load_factor = numbers.load_factor(),
                     .max_load_factor = numbers.max_load_factor()});
  }

  {
    ostringstream fname;
    fname << "load_factor_items_" << item_count << "_max_load_factor_" << fixed
          << setprecision(1) << numbers.max_load_factor() << ".csv";
    ofstream out(fname.str());
    out << "size,bucket_count,load_factor,max_load_factor\n";
    out << fixed << setprecision(2);
    for (const auto& stat_item : stats) {
      out << stat_item.size << ',' << stat_item.bucket_count << ','
          << stat_item.load_factor << ',' << stat_item.max_load_factor << '\n';
    }
  }
  {
    ostringstream fname;
    fname << "bucket_sizes"
          << "_items_" << item_count << "_max_load_factor_" << fixed
          << setprecision(1) << numbers.max_load_factor() << ".csv";
    ofstream out(fname.str());
    out << "bucket_num,bucket_size\n";
    for (size_t i = 0; i < numbers.bucket_count(); ++i) {
      out << i + 1 << ',' << numbers.bucket_size(i) << '\n';
    }
  }
  {
    ostringstream fname;
    fname << "insert_durations"
          << "_items_" << item_count << "_max_load_factor_" << fixed
          << setprecision(1) << numbers.max_load_factor() << ".csv";
    ofstream out(fname.str());
    out << "iterations,duration_ns,bucket_count\n";
    for (size_t i = 0; i < insert_durations.size(); ++i) {
      out << i + 1 << ',' << insert_durations[i] << ',' << stats[i].bucket_count << '\n';
    }
  }
}