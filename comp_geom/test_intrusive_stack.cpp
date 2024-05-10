/*
g++ -o test_intrusive_stack test_intrusive_stack.cpp 
*/

#include "intrusive_stack.h"

int
main()
{
  intrusive_stack<double> is;
  is.push(3.1);
  is.push(2.2);

  is.container()[0];
  is.container()[0] = 4.5;
}
