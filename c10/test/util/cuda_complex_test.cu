#include <c10/test/util/complex_test_common.h>

__managed__ bool pass;

TEST(NonStaticHostTests, all) {
  constructors::test_thrust_conversion();
  assignment::test_assign_thrust();
  io::test_io();
  test_std::test_values();
}

int main() {
  NonStaticHostTests_all();
}