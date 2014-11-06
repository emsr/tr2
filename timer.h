#include <chrono>

class Timer
{
public:

  using millisecs = std::chrono::duration<long, std::milli>;
  using microsecs = std::chrono::duration<long, std::micro>;

  using clock_type = std::chrono::system_clock;
  using time_point_type = std::chrono::system_clock::time_point;
  using duration_type = millisecs;

  Timer()
  { this->start(); }

  time_point_type
  start()
  { return this->m_prev_time = std::chrono::system_clock::now(); }

  time_point_type
  stop()
  { return this->m_curr_time = std::chrono::system_clock::now(); }

  long
  time_elapsed() const
  {
    long tm = std::chrono::duration_cast<duration_type>(this->m_curr_time - this->m_prev_time).count();
    return tm;
  }

private:

  time_point_type m_prev_time;
  time_point_type m_curr_time;
};
