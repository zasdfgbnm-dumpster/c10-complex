#include <c10/test/util/complex_test_common.h>

TEST(NonStaticTests, all) {
  constructors::test_thrust_conversion();
  assignment::test_assign_thrust();
  io::test_io();
  test_std::test_values();
}

// main
int main() {
  NonStaticTests_all();
}