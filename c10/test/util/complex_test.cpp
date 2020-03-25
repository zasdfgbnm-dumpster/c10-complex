#include <c10/util/complex.h>
#include <type_traits>
#include <tuple>

// gtest mock
#include <cassert>
#define ASSERT_EQ(a, b) assert(a == b)
#define TEST(a, b) void a##_##b()

namespace memory {

void test_size() {
  static_assert(sizeof(c10::complex<c10::Half>) == 4, "");
  static_assert(sizeof(c10::complex<float>) == 8, "");
  static_assert(sizeof(c10::complex<double>) == 16, "");
}

void test_align() {
  static_assert(alignof(c10::complex<c10::Half>) == 4, "");
  static_assert(alignof(c10::complex<float>) == 8, "");
  static_assert(alignof(c10::complex<double>) == 16, "");
}

}  // memory

namespace constructors {

template<typename scalar_t>
void test_construct_from_scalar() {
  constexpr scalar_t num1 = scalar_t(1.23);
  constexpr scalar_t num2 = scalar_t(4.56);
  constexpr scalar_t zero = scalar_t();
  static_assert(c10::complex<scalar_t>(num1, num2).real() == num1, "");
  static_assert(c10::complex<scalar_t>(num1, num2).imag() == num2, "");
  static_assert(c10::complex<scalar_t>(num1).real() == num1, "");
  static_assert(c10::complex<scalar_t>(num1).imag() == zero, "");
  static_assert(c10::complex<scalar_t>().real() == zero, "");
  static_assert(c10::complex<scalar_t>().imag() == zero, "");
}

template<typename scalar_t, typename other_t>
void test_construct_from_other() {
  constexpr other_t num1 = other_t(1.23);
  constexpr other_t num2 = other_t(4.56);
  constexpr scalar_t num3 = scalar_t(num1);
  constexpr scalar_t num4 = scalar_t(num2);
  static_assert(c10::complex<scalar_t>(c10::complex<other_t>(num1, num2)).real() == num3, "");
  static_assert(c10::complex<scalar_t>(c10::complex<other_t>(num1, num2)).imag() == num4, "");
}

void test_convert_constructors() {
  test_construct_from_scalar<c10::Half>();
  test_construct_from_scalar<float>();
  test_construct_from_scalar<double>();

  static_assert(std::is_convertible<c10::complex<c10::Half>, c10::complex<c10::Half>>::value, "");
  static_assert(!std::is_convertible<c10::complex<float>, c10::complex<c10::Half>>::value, "");
  static_assert(!std::is_convertible<c10::complex<double>, c10::complex<c10::Half>>::value, "");
  static_assert(std::is_convertible<c10::complex<c10::Half>, c10::complex<float>>::value, "");
  static_assert(std::is_convertible<c10::complex<float>, c10::complex<float>>::value, "");
  static_assert(!std::is_convertible<c10::complex<double>, c10::complex<float>>::value, "");
  static_assert(std::is_convertible<c10::complex<c10::Half>, c10::complex<double>>::value, "");
  static_assert(std::is_convertible<c10::complex<float>, c10::complex<double>>::value, "");
  static_assert(std::is_convertible<c10::complex<double>, c10::complex<double>>::value, "");

  static_assert(std::is_constructible<c10::complex<c10::Half>, c10::complex<c10::Half>>::value, "");
  static_assert(std::is_constructible<c10::complex<float>, c10::complex<c10::Half>>::value, "");
  static_assert(std::is_constructible<c10::complex<double>, c10::complex<c10::Half>>::value, "");
  static_assert(std::is_constructible<c10::complex<c10::Half>, c10::complex<float>>::value, "");
  static_assert(std::is_constructible<c10::complex<float>, c10::complex<float>>::value, "");
  static_assert(std::is_constructible<c10::complex<double>, c10::complex<float>>::value, "");
  static_assert(std::is_constructible<c10::complex<c10::Half>, c10::complex<double>>::value, "");
  static_assert(std::is_constructible<c10::complex<float>, c10::complex<double>>::value, "");
  static_assert(std::is_constructible<c10::complex<double>, c10::complex<double>>::value, "");

  test_construct_from_other<c10::Half, c10::Half>();
  test_construct_from_other<c10::Half, float>();
  test_construct_from_other<c10::Half, double>();
  test_construct_from_other<float, c10::Half>();
  test_construct_from_other<float, float>();
  test_construct_from_other<float, double>();
  test_construct_from_other<double, c10::Half>();
  test_construct_from_other<double, float>();
  test_construct_from_other<double, double>();
}

template<typename scalar_t>
void test_construct_from_std() {
  constexpr scalar_t num1 = scalar_t(1.23);
  constexpr scalar_t num2 = scalar_t(4.56);
  static_assert(c10::complex<scalar_t>(std::complex<scalar_t>(num1, num2)).real() == num1, "");
  static_assert(c10::complex<scalar_t>(std::complex<scalar_t>(num1, num2)).imag() == num2, "");
}

void test_std_conversion() {
  test_construct_from_std<float>();
  test_construct_from_std<double>();
}

#if defined(__CUDACC__) || defined(__HIPCC__)
template<typename scalar_t>
void test_construct_from_thrust() {
  constexpr scalar_t num1 = scalar_t(1.23);
  constexpr scalar_t num2 = scalar_t(4.56);
  ASSERT_EQ(c10::complex<scalar_t>(thrust::complex<scalar_t>(num1, num2)).real(), num1);
  ASSERT_EQ(c10::complex<scalar_t>(thrust::complex<scalar_t>(num1, num2)).imag(), num2);
}
#endif

void test_thrust_conversion() {
#if defined(__CUDACC__) || defined(__HIPCC__)
  test_construct_from_thrust<float>();
  test_construct_from_thrust<double>();
#endif
}


}  // constructors

namespace assignment {

template<typename scalar_t>
constexpr c10::complex<scalar_t> one() {
  c10::complex<scalar_t> result;
  result = scalar_t(1);
  return result;
}

void test_assign_real() {
  static_assert(one<c10::Half>().real() == c10::Half(1), "");
  static_assert(one<c10::Half>().imag() == c10::Half(), "");
  static_assert(one<float>().real() == float(1), "");
  static_assert(one<float>().imag() == float(), "");
  static_assert(one<double>().real() == double(1), "");
  static_assert(one<double>().imag() == double(), "");
}

constexpr std::tuple<c10::complex<double>, c10::complex<float>, c10::complex<c10::Half>> ones() {
  constexpr c10::complex<c10::Half> src(1, 1);
  c10::complex<double> ret0;
  c10::complex<float> ret1;
  c10::complex<c10::Half> ret2;
  ret0 = ret1 = ret2 = src;
  return std::make_tuple(ret0, ret1, ret2);
}

void test_assign_other() {
  constexpr auto tup = ones();
  static_assert(std::get<c10::complex<double>>(tup).real() == double(1), "");
  static_assert(std::get<c10::complex<double>>(tup).imag() == double(1), "");
  static_assert(std::get<c10::complex<float>>(tup).real() == float(1), "");
  static_assert(std::get<c10::complex<float>>(tup).imag() == float(1), "");
  static_assert(std::get<c10::complex<c10::Half>>(tup).real() == c10::Half(1), "");
  static_assert(std::get<c10::complex<c10::Half>>(tup).imag() == c10::Half(1), "");
}

constexpr std::tuple<c10::complex<double>, c10::complex<float>, c10::complex<c10::Half>> ones_std() {
  constexpr std::complex<c10::Half> src(1, 1);
  c10::complex<double> ret0;
  c10::complex<float> ret1;
  c10::complex<c10::Half> ret2;
  ret0 = ret1 = ret2 = src;
  return std::make_tuple(ret0, ret1, ret2);
}

void test_assign_std() {
  constexpr auto tup = ones_std();
  static_assert(std::get<c10::complex<double>>(tup).real() == double(1), "");
  static_assert(std::get<c10::complex<double>>(tup).imag() == double(1), "");
  static_assert(std::get<c10::complex<float>>(tup).real() == float(1), "");
  static_assert(std::get<c10::complex<float>>(tup).imag() == float(1), "");
  static_assert(std::get<c10::complex<c10::Half>>(tup).real() == c10::Half(1), "");
  static_assert(std::get<c10::complex<c10::Half>>(tup).imag() == c10::Half(1), "");
}

#if defined(__CUDACC__) || defined(__HIPCC__)
std::tuple<c10::complex<double>, c10::complex<float>, c10::complex<c10::Half>> ones_thrust() {
  thrust::complex<c10::Half> src(1, 1);
  c10::complex<double> ret0;
  c10::complex<float> ret1;
  c10::complex<c10::Half> ret2;
  ret0 = ret1 = ret2 = src;
  return std::make_tuple(ret0, ret1, ret2);
}
#endif

void test_assign_thrust() {
#if defined(__CUDACC__) || defined(__HIPCC__)
  auto tup = ones_thrust();
  ASSERT_EQ(std::get<c10::complex<double>>(tup).real(), double(1));
  ASSERT_EQ(std::get<c10::complex<double>>(tup).imag(), double(1));
  ASSERT_EQ(std::get<c10::complex<float>>(tup).real(), float(1));
  ASSERT_EQ(std::get<c10::complex<float>>(tup).imag(), float(1));
  ASSERT_EQ(std::get<c10::complex<c10::Half>>(tup).real(), c10::Half(1));
  ASSERT_EQ(std::get<c10::complex<c10::Half>>(tup).imag(), c10::Half(1));
#endif
}

} // namespace assignment

namespace literals {

void test_complex_literals() {
  using namespace c10::complex_literals;
  static_assert(std::is_same<decltype(0.5_ih), c10::complex<c10::Half>>::value, "");
  static_assert((0.5_ih).real() == c10::Half(), "");
  static_assert((0.5_ih).imag() == c10::Half(0.5), "");
  static_assert(std::is_same<decltype(0.5_if), c10::complex<float>>::value, "");
  static_assert((0.5_if).real() == float(), "");
  static_assert((0.5_if).imag() == float(0.5), "");
  static_assert(std::is_same<decltype(0.5_id), c10::complex<double>>::value, "");
  static_assert((0.5_id).real() == float(), "");
  static_assert((0.5_id).imag() == float(0.5), "");
}

} // namespace literals

namespace real_imag {

template<typename scalar_t>
constexpr c10::complex<scalar_t> zero_one() {
  c10::complex<scalar_t> result;
  result.imag(scalar_t(1));
  return result;
}

template<typename scalar_t>
constexpr c10::complex<scalar_t> one_zero() {
  c10::complex<scalar_t> result;
  result.real(scalar_t(1));
  return result;
}

void test_real_imag_modify() {
  static_assert(zero_one<c10::Half>().real() == c10::Half(0), "");
  static_assert(zero_one<c10::Half>().imag() == c10::Half(1), "");
  static_assert(zero_one<float>().real() == float(0), "");
  static_assert(zero_one<float>().imag() == float(1), "");
  static_assert(zero_one<double>().real() == double(0), "");
  static_assert(zero_one<double>().imag() == double(1), "");

  static_assert(one_zero<c10::Half>().real() == c10::Half(1), "");
  static_assert(one_zero<c10::Half>().imag() == c10::Half(0), "");
  static_assert(one_zero<float>().real() == float(1), "");
  static_assert(one_zero<float>().imag() == float(0), "");
  static_assert(one_zero<double>().real() == double(1), "");
  static_assert(one_zero<double>().imag() == double(0), "");
}

} // namespace real_imag


TEST(NonStaticTests, all) {
  constructors::test_thrust_conversion();
  assignment::test_assign_thrust();
}

// main
int main() {
  NonStaticTests_all();
}