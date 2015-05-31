// /home/ed/bin_concepts/bin/g++ -std=c++1z -o concept1 concept1.cpp

//  Constrained type specifiers.
//  dcl.spec.auto.constr
template<typename T>
  concept bool C1 = false;

template<int N>
  concept bool C2 = false;

template<template<typename> typename X>
  concept bool C3 = false;

template<typename T, int N>
  class Array
  {};

template<typename T, template<typename> typename A>
  class Stack
  {};

template<typename T>
  class Alloc
  {};

void
f1(C1); // C1 designates a placeholder type.

void
f2(Array<auto, C2>); // C2 designates a placeholder for int value.

void
f3(Stack<auto, C3>); // C3 designates a placeholder for a class template.


// #1
template<typename T>
  concept bool
  C()
  { return true; }

// #2
template<typename T, typename U>
  concept bool
  C()
  { return true; }

// #3
template<typename T>
  concept bool
  D = true;

// The set of concepts referred to by C includes #1 and #2;
// Concept resolution selects #1.
void
f(C);

// The concept-name D refers only to #3.
void
g(D);


//  Partial-concept-id.
template<typename T, int N = 0>
  concept bool
  Seq = true;

void
f1(Seq<3>);

void
f2(Seq<>);


//  Concept specifier.
//  dcl.spec.concept

//  Function concept.
template<typename T>
  concept bool
  F1()
  { return true; }

//  Variable concept.
template<typename T>
  concept bool
  V1{true};
