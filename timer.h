#include <chrono>

class Timer
{
public:

  using clock_type = std::chrono::system_clock;
  using time_point_type = std::chrono::system_clock::time_point;
  using millisecs = std::chrono::duration<long, std::milli>;
  using microsecs = std::chrono::duration<long, std::micro>;

  time_point_type
  start()
  { return m_prev_time = std::chrono::system_clock::now(); }

  time_point_type
  stop()
  { return m_curr_time = std::chrono::system_clock::now(); }

  long
  time_elapsed() const
  {
    long tm = std::chrono::duration_cast<millisecs>(m_curr_time - m_prev_time).count();
    return tm;
  }

private:

  time_point_type m_prev_time;
  time_point_type m_curr_time;
};
