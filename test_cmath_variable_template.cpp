// clang++ -c++1y -o test_cmath_variable_template test_cmath_variable_template.cpp

// /home/ed/bin_var_template_2/bin/g++ -std=c++1y -fdiagnostics-color=auto -o test_cmath_variable_template test_cmath_variable_template.cpp


// ./test_cmath_variable_template

#include "cmath_variable_template"

int
main()
{
  using namespace std::constants::math_constants;

  auto twopie = m_2pi<long double>;

  float height = 250.0f;
  auto width = m_golden_ratio<float> * height;
}
