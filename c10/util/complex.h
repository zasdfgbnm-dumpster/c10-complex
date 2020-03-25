#include <complex>
#if defined(__CUDACC__) || defined(__HIPCC__)
#include <thrust/thrust.h>
#endif

namespace c10 {

using Half = short;  // Just for the convenience of prototyping

// c10::complex is an implementation of complex numbers that aims
// to work on all devices supported by PyTorch
//
// Most of the APIs duplicates std::complex
// Reference: https://en.cppreference.com/w/cpp/numeric/complex
//
// [Note on Constructors]
//
// The APIs of constructors are mostly copied from C++ standard:
//   https://en.cppreference.com/w/cpp/numeric/complex/complex
//
// Since C++14, all constructors are constexpr in std::complex
//
// There are three types of constructors:
// - initializing from real and imag: 
//     `constexpr complex( const T& re = T(), const T& im = T() );`
// - implicitly-declared copy constructor
// - converting constructors
//
// Converting constructors: 
// - std::complex defines converting constructor between float/double/long double,
//   while we define converting constructor between c10::Half/float/double.
// - For these converting constructors, upcasting is implicit, downcasting is
//   explicit.
// - We also define explicit casting from std::complex/thrust::complex
//   - Note that the conversion from thrust is not constexpr, because
//     thrust does not define them as constexpr
//
//
// [Operator =]
//
// The APIs of operator = are mostly copied from C++ standard:
//   https://en.cppreference.com/w/cpp/numeric/complex/operator%3D
//
// Since C++20, all operator= are constexpr. Although we are not bulding with
// C++20, we also obey this behavior.
//
// There are three types of assign operator:
// - Assign a real value from the same scalar type
//   - In std, this is templated as complex& operator=(const T& x)
//     with specialization `complex& operator=(T x)` for float/double/long double
//     Since we only support c10::Half, float and double, on will use `complex& operator=(T x)`
// - Copy assignment operator and converting assignment operator
//   - There is no specialization of converting assignemnt operators, which type is
//     convertible is soly depend on whether the scalar type is convertable
//
//
// [Operator""]
//
// std::complex has custom literals `i`, `if` and `if` defined in namespace `std::literals::complex_literals`.
// We define our own custom literals in the namespace `c10::complex_literals`. Our custom literals does not
// follow the same behavior as in std::complex, instead, we define _ih, _if, _id to construct half/float/double
// complex literals.

template<typename T>
struct complex {
  static_assert(false, "c10::complex only support c10::Half, float, double as scalar type");
};

namespace {

template<typename T>
struct alignas(sizeof(T) * 2) complex_common {
  T storage[2];

  constexpr complex_common(const T& re = T(), const T& im = T()): storage{re, im} {}
  template<typename U>
  constexpr complex_common(const std::complex<U> &other): complex_common(other.real(), other.imag()) {}
#if defined(__CUDACC__) || defined(__HIPCC__)
  template<typename U>
  complex_common(const thrust::complex<U> &other): complex_common(other.real(), other.imag()) {}
#endif

  constexpr complex<T> &operator =(T re) {
    T[0] = re;
    return reinterpret_cast<complex<T> &>(*this);
  }
  template<typename U>
  constexpr complex<T> &operator =(const complex<U> &rhs) {
    storage[0] = rhs.real();
    storage[1] = rhs.imag();
    return reinterpret_cast<complex<T> &>(*this);
  }

  constexpr T real() const {
    return storage[0];
  }
  constexpr T imag() const {
    return storage[1];
  }
};

}

template<>
struct alignas(4) complex<c10::Half>: public complex_common<c10::Half> {
  using complex_common<c10::Half>::complex_common;
  explicit constexpr complex(const complex<float> &other): complex_common(static_cast<c10::Half>(other.real()), static_cast<c10::Half>(other.imag())) {}
  explicit constexpr complex(const complex<double> &other): complex_common(static_cast<c10::Half>(other.real()), static_cast<c10::Half>(other.imag())) {}
};

template<>
struct alignas(8) complex<float>: public complex_common<float> {
  using complex_common<float>::complex_common;
  constexpr complex(const complex<c10::Half> &other): complex_common(other.real(), other.imag()) {}
  explicit constexpr complex(const complex<double> &other): complex_common(static_cast<c10::Half>(other.real()), static_cast<c10::Half>(other.imag())) {}
};

template<>
struct alignas(16) complex<double>: public complex_common<double> {
  using complex_common<double>::complex_common;
  constexpr complex(const complex<c10::Half> &other): complex_common(other.real(), other.imag()) {}
  constexpr complex(const complex<float> &other): complex_common(other.real(), other.imag()) {}
};

namespace complex_literals {

constexpr complex<c10::Half> operator"" _ih(long double imag) {
  return complex<c10::Half>(c10::Half(), static_cast<c10::Half>(imag));
}

constexpr complex<float> operator"" _if(long double imag) {
  return complex<float>(float(), static_cast<float>(imag));
}

constexpr complex<double> operator"" _id(long double imag) {
  return complex<double>(double(), static_cast<double>(imag));
}

} // namespace complex_literals

} // namespace c10

#include <c10/util/complex_math.h>
