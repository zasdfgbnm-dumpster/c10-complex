#include <c10/util/complex.h>
#include <type_traits>

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

void test_constructors() {
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

}  // constructors

// Main

int main() {}