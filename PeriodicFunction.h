#ifndef PERIODICFUNCTION_H_
#define PERIODICFUNCTION_H_
 
#include <cstdint>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
 
namespace pf {
 
using std::chrono::steady_clock;
using std::chrono::duration;
using std::chrono::milliseconds;
 
template<typename Functor>
class PeriodicFunction {
public:
  //Initialize the timer state and start the timing loop
  PeriodicFunction(uint32_t periodMillis, uint32_t durationMillis,
      Functor& callback, int32_t yieldMillis = DEFAULT_YIELD_MILLIS) :
      func_(callback),
      periodMillis_(periodMillis),
      expirationTime_(steady_clock::now()
              + steady_clock::duration(milliseconds(durationMillis))),
      nextCallTimeMillis_(steady_clock::now()), yieldMillis_(yieldMillis) {
    //Start the timing loop
    t_ = std::thread { PeriodicFunction::threadLoop, this };
  }
 
  //Command & wait for the threadLoop to stop
  //before this object gets de-constructed
  ~PeriodicFunction() {
    stop();
  }
 
  bool hasExpired() const {
    return hasExpired_;
  }
 
  void stop() {
    isRunning_ = false;
    if (t_.joinable())
      t_.join();
  }
 
  void restart(uint32_t periodMillis, uint32_t durationMillis) {
    std::lock_guard<std::mutex> lg(stateMutex_);
    //Stop the current timer if needed
    stop();
 
    //What time is it right at this instant?
    auto now = steady_clock::now();
 
    //Set the state for the new timer
    expirationTime_ = now + milliseconds(durationMillis);
    nextCallTimeMillis_ = now;
    periodMillis_ = periodMillis;
    hasExpired_ = false;
 
    //Start the timing loop
    isRunning_ = true;
    t_ = std::thread { PeriodicFunction::threadLoop, this };
  }
 
  //Since we retain a reference to a Functor object, prevent copying
  //and moving
  PeriodicFunction(const PeriodicFunction& rhs) = delete;
  PeriodicFunction& operator=(const PeriodicFunction& rhs) = delete;
  PeriodicFunction(PeriodicFunction&& rhs) = delete;
  PeriodicFunction& operator=(PeriodicFunction&& rhs) = delete;
 
private:
  //The function to be executed periodically until we're done
  Functor& func_;
 
  //The period at which the function is executed
  uint32_t periodMillis_;
 
  //The absolute time at which we declare "done done!"
  steady_clock::time_point expirationTime_;
 
  //The next scheduled function execution time
  steady_clock::time_point nextCallTimeMillis_;
 
  //The thread sleep duration; the larger the value,
  //the more we decrease the periodic execution accuracy;
  //allows other sibling threads threads to use the cpu
  uint32_t yieldMillis_;
 
  //The default sleep duration of each pass thru
  //the timing loop
  static constexpr uint32_t DEFAULT_YIELD_MILLIS { 10 };
 
  //Indicates whether the timer has expired or not
  std::atomic<bool> hasExpired_ { false };
 
  //Indicates whether the monitoring loop is active
  //probably doesn't need to be atomic, but good practice
  std::atomic<bool> isRunning_ { true };
 
  //Our precious thread resource!
  std::thread t_ { };
 
  //Protects the timer state from data races
  //between our private thread and the caller's thread
  std::mutex stateMutex_ { };
 
  //The timing loop
  void threadLoop() {
    while (isRunning_) {
      auto now = steady_clock::now();//What time is it right at this instant?
      std::lock_guard<std::mutex> lg(stateMutex_);
      if (now >= expirationTime_) { //Has the timer expired?
        hasExpired_ = true;
        return;
      }
      else if (now > nextCallTimeMillis_) {//Time to execute function?
        nextCallTimeMillis_ = now + milliseconds(periodMillis_);
        std::bind(&Functor::operator(), std::ref(func_))(); //Execute!
        continue; //Skip the sleep
      }
      //Unselfish sharing; let other threads have the cpu for a bit
      std::this_thread::sleep_for(milliseconds(yieldMillis_));
    }
  }
};
//End of the class definition
}//namespace pf
 
#endif /* PERIODICFUNCTION_H_ */

