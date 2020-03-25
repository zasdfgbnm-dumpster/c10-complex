#include <c10/util/complex.h>

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

void test_constructors() {
  constexpr c10::Half num1 = c10::Half(1.23);
  constexpr c10::Half num2 = c10::Half(4.56);
  static_assert(c10::complex<c10::Half>(num1, num2).real() == num1, "");
}

int main() {}