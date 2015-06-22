// $HOME/bin_concepts/bin/g++ -std=c++1z -o concept1-neg concept1-neg.cpp

//  Concept specifier.
//  dcl.spec.concept

//  Function concept.
template<typename T>
  concept bool
  F2(); // error: no definition.

template<typename T>
  constexpr bool
  F3();

template<typename T>
  concept bool
  F3()
  { return true; } // error: Redeclaration.


//  Variable concept.
template<typename T>
  concept bool
  V1; // error: No initializer.


struct S
{
  template<typename T>
    static concept bool
    C = true; // error: Concept in class scope.
};
