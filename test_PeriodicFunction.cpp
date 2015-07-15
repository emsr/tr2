#include <iostream>
#include <chrono>
#include "PeriodicFunction.h"
 
int main() {
  //Create a lambda and plcae into execution
  //every 200 millis over a duration of 1 second
  using namespace std::chrono;
  steady_clock::time_point startTime { steady_clock::now() };
  steady_clock::time_point execTime { startTime };
  auto lambda = [&] { //We don't need the ()
    using namespace std::chrono;
    execTime = steady_clock::now();
    std::cout << "lambda executed at T="
              << duration_cast<milliseconds>(execTime - startTime).count()
              << std::endl;
    startTime = execTime;
  };
  pf::PeriodicFunction<decltype(lambda)> pf1 { 200, 1000, lambda };
  while (!pf1.hasExpired()) ;
  std::cout << std::endl;
 
  //Re-run the lambda every half second for a second
  startTime = steady_clock::now();
  execTime = startTime;
  pf1.restart(500, 1500);
  while (!pf1.hasExpired()) ;
  std::cout << std::endl;
 
  //Define a stateful Functor class
  class Functor {
  public:
    Functor() :
        startTime_ { steady_clock::now() }, execTime_ { startTime_ } {
    }
    void operator()() {
      execTime_ = steady_clock::now();
      std::cout << "Functor::operator() executed at T="
          << duration_cast<milliseconds>(execTime_ - startTime_).count()
          << std::endl;
      startTime_ = execTime_;
    }
  private:
    steady_clock::time_point startTime_;
    steady_clock::time_point execTime_;
  };
 
  //Create a Functor object and place into execution
  //every 50 millis over a duration of 250 millis
  Functor myFunction { };
  pf::PeriodicFunction<Functor> pf2 { 50, 250, myFunction, 0 };
  while (!pf2.hasExpired()) ;
}

