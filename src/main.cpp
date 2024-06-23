#include <sycl/sycl.hpp>
#include <format>
#include <iostream>
#include <string>

#include "platform.hpp"
#include "formatting.hpp"

int score_device(const sycl::device& dev) {
  using namespace sycl;

  auto backend = dev.get_backend();

  int device_score = 0;

  switch (dev.get_info<info::device::device_type>()) {
  case info::device_type::custom:
    return -0xFFFF;
  case info::device_type::gpu:
    device_score += 11 << 12;
    break;
  case info::device_type::cpu:
    device_score += 10 << 12;
    break;
  default:
    break;
  }

  // Backend defs are implementation-specific. Prioritize GPU-specific backends over OpenCL and OpenCL over anything else.
  #if defined(SYCL_IMPLEMENTATION_ONEAPI)
  switch (dev.get_backend()) {
  case backend::ext_oneapi_cuda:
  case backend::ext_oneapi_hip:
  case backend::ext_oneapi_level_zero:
    device_score += 12 << 8;
    break;
  case backend::opencl:
    device_score += 6 << 8;
    break;
  default:
    break;
  }
  #elif defined(SYCL_IMPLEMENTATION_HIPSYCL)
  switch (dev.get_backend()) {
  case backend::cuda:
  case backend::hip:
  case backend::level_zero:
    device_score += 12 << 8;
    break;
  case backend::ocl:
    device_score += 6 << 8;
    break;
  default:
    break;
  }
  #endif
  return device_score;
}

int main() {
  sycl::queue q(score_device);

  std::cout << std::format("SYCL device:   {}\n", q.get_device().get_info<sycl::info::device::name>());
  std::cout << std::format("SYCL platform: {}\n", q.get_device().get_platform().get_info<sycl::info::platform::name>());

  std::formatter<int> x;
}