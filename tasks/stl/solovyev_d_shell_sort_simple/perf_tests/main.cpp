#include <gtest/gtest.h>

#include <chrono>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "core/task/include/task.hpp"
#include "stl/solovyev_d_shell_sort_simple/include/ops_stl.hpp"

namespace {
std::vector<int> GetRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = (int)((gen() % (200)) - 100);
  }
  return vec;
}

bool IsSorted(std::vector<int> data) {
  int last = INT_MIN;
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i] < last) {
      return false;
    }
    last = data[i];
  }
  return true;
}
}  // namespace
TEST(solovyev_d_shell_sort_simple_stl, test_pipeline_run) {
  constexpr int kCount = 5000000;

  // Create data
  std::vector<int> in = GetRandomVector(kCount);
  std::vector<int> out(in.size(), 0);

  // Create task_data
  auto task_data_stl = std::make_shared<ppc::core::TaskData>();
  task_data_stl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_stl->inputs_count.emplace_back(in.size());
  task_data_stl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_stl->outputs_count.emplace_back(out.size());

  // Create Task
  auto task_stl = std::make_shared<solovyev_d_shell_sort_simple_stl::TaskSTL>(task_data_stl);

  // Create Perf attributes
  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perf_analyzer = std::make_shared<ppc::core::Perf>(task_stl);
  perf_analyzer->PipelineRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);
  ASSERT_TRUE(IsSorted(out));
}

TEST(solovyev_d_shell_sort_simple_stl, test_task_run) {
  constexpr int kCount = 5000000;

  // Create data
  std::vector<int> in = GetRandomVector(kCount);
  std::vector<int> out(in.size(), 0);

  // Create task_data
  auto task_data_stl = std::make_shared<ppc::core::TaskData>();
  task_data_stl->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_stl->inputs_count.emplace_back(in.size());
  task_data_stl->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_stl->outputs_count.emplace_back(out.size());

  // Create Task
  auto task_stl = std::make_shared<solovyev_d_shell_sort_simple_stl::TaskSTL>(task_data_stl);

  // Create Perf attributes
  auto perf_attr = std::make_shared<ppc::core::PerfAttr>();
  perf_attr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perf_attr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perf_results = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perf_analyzer = std::make_shared<ppc::core::Perf>(task_stl);
  perf_analyzer->TaskRun(perf_attr, perf_results);
  ppc::core::Perf::PrintPerfStatistic(perf_results);
  ASSERT_TRUE(IsSorted(out));
}
