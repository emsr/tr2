
// /home/ed/bin/bin/g++ -std=c++1z -pedantic -DVOMIT -o fold_expression fold_expression.cpp


template<typename... Args>
  bool
  some(Args... args)
  { return (... || args); }

template<typename... Args>
  bool
  f(Args... args)
  { 
    return (true + ... + args); // OK
  }

template<typename... Args>
  bool
  all(Args... args)
  { return (args && ...); }

bool b = all(true, true, true, false);

bool a = some(true, true, true, false);
