#include <c10/test/util/complex_test_common.h>

TEST(HostTests, all) {
  run_all_host_tests();
}

__global__ void test_thrust_kernel() {
  // thrust conversion
  {
  constexpr float num1 = float(1.23);
  constexpr float num2 = float(4.56);
  assert(c10::complex<float>(thrust::complex<float>(num1, num2)).real() == num1);
  assert(c10::complex<float>(thrust::complex<float>(num1, num2)).imag() == num2);
  }
  {
  constexpr double num1 = double(1.23);
  constexpr double num2 = double(4.56);
  assert(c10::complex<double>(thrust::complex<double>(num1, num2)).real() == num1);
  assert(c10::complex<double>(thrust::complex<double>(num1, num2)).imag() == num2);
  }
  // thrust assignment
  auto tup = assignment::one_two_thrust();
  assert(std::get<c10::complex<double>>(tup).real() == double(1));
  assert(std::get<c10::complex<double>>(tup).imag() == double(2));
  assert(std::get<c10::complex<float>>(tup).real() == float(1));
  assert(std::get<c10::complex<float>>(tup).imag() == float(2));
}

__global__ void test_std_functions_kernel() {
}

TEST(DeviceTests, ThrustConversion) {
  cudaDeviceSynchronize();
  test_thrust_kernel<<<1, 1>>>();
  cudaDeviceSynchronize();
  ASSERT_EQ(cudaGetLastError(), cudaSuccess);
}

TEST(DeviceTests, StdFunctions) {
  cudaDeviceSynchronize();
  test_std_functions_kernel<<<1, 1>>>();
  cudaDeviceSynchronize();
  ASSERT_EQ(cudaGetLastError(), cudaSuccess);
}

int main() {
  HostTests_all();
  DeviceTests_ThrustConversion();
  DeviceTests_StdFunctions();
}