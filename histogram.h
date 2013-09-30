
#include <vector>
#include <algorithm>
#include <stdexcept>

/**
 *  @brief  A simple histogram class.
 */
template<typename Tp>
  class histogram
  {
  public:

    typedef Tp value_type;
    typedef std::size_t size_type;

    /**
     *  @brief  Create a histogram
     */
    histogram(std::size_t n, value_type xmin, value_type xmax)
    : _M_bin(n + 1), _M_count(n + 2)
    {
      _M_bin[n] = xmax;
      value_type delta = (xmax - xmin) / n;
      for (std::size_t i = 0; i < n; ++i)
	_M_bin[i] = xmin + i * delta;
    }

    /**
     *  @brief  Create a histogram
     */
    template<typename Iter>
      histogram(Iter xbegin, Iter xend)
      : _M_bin{}, _M_count{}
      {
        for (; xbegin != xend; ++xbegin)
        {
          _M_bin.push_back(*xbegin);
          _M_count.push_back(0);
        }
        _M_count.push_back(0);
        if (!std::is_sorted(std::begin(_M_bin), std::end(_M_bin)))
          throw std::domain_error("histogram: bin limits not sorted");
      }

    /**
     *  @brief  Constructor.
     */
    histogram(std::initializer_list<value_type> il)
    : _M_bin(il), _M_count(il.size() + 1)
    {
      if (!std::is_sorted(std::begin(_M_bin), std::end(_M_bin)))
        throw std::domain_error("histogram: bin limits not sorted");
    }

    /**
     *  @brief  Insertion operator.
     */
    histogram&
    operator<<(value_type x)
    {
      if (x < _M_bin.front())
      {
	++_M_count.front();
	return *this;
      }
      else if (x >= _M_bin.back())
      {
	++_M_count.back();
	return *this;
      }
      else
      {
	auto pos = std::lower_bound(std::begin(_M_bin), std::end(_M_bin), x);
	if (pos == _M_bin.end())
	  ++_M_count.back();
	else
	  ++_M_count[pos - std::begin(_M_bin)];
	return *this;
      }
    }

    /**
     *  @brief  Insert a range of value.
     */
    template<typename Iter>
      void
      insert(Iter begin, Iter end)
      {
	for(; begin != end; ++begin)
          *this << *begin;
      }

    /**
     *  @brief  Return the number of bins.
     */
    std::size_t
    size() const noexcept
    { return _M_bin.size() - 1; }

    /**
     *  @brief  Return the number of items including items above and below the limits.
     */
    std::size_t
    count() const noexcept
    { return std::accumulate(std::begin(_M_count), std::end(_M_count), 0); }

    /**
     *  @brief  Return the number of items from below the lower limit up to bin i.
     */
    std::size_t
    count(std::size_t i) const noexcept
    {
      std::size_t m = (i < _M_count.size() ? i : _M_count.size());
      return std::accumulate(_M_count.begin(), _M_count.begin() + m, 0);
    }

    /**
     *  @brief  Return the mean - excluding items above and below the limit.
     */
    value_type
    mean() const noexcept
    {
      std::size_t num = 0;
      value_type sum = 0;
      for (std::size_t i = 1; i <= size(); ++i)
      {
        num += _M_count[i];
        sum += _M_count[i] * (value(i) + value(i + 1)) / 2;
      }

      return sum / num;
    }

    /**
     *  @brief  Return the standard deviation - excluding items above and below the limits.
     */
    value_type
    sigma() const noexcept
    {
      std::size_t num = 0;
      value_type sum = 0;
      for (std::size_t i = 1; i <= size(); ++i)
      {
        num += _M_count[i];
        sum += _M_count[i] * (value(i) + value(i + 1)) / 2;
      }
      value_type mean = sum / num;

      value_type sig = 0;
      for (std::size_t i = 1; i <= size(); ++i)
      {
        value_type del = (value(i) + value(i + 1)) / 2 - mean;
        sig += _M_count[i] * del * del;
      }
      sig /= num;

      return std::sqrt(sig);
    }

    /**
     *  @brief  Return the count of the ith bin where 1 <= i <= size().
     *                 the count below the lower limit for i == 0.
     *                 the count above the upper limit for i == size() + 1.
     */
    std::size_t
    operator[](const std::size_t i) const noexcept
    { return _M_count[i]; }

    /**
     *  @brief  Return the left value of the ith bin where 1 <= i <= size().
     */
    value_type
    value(const std::size_t i) const noexcept
    { return _M_bin[i - 1]; }

    /**
     *  @brief  Reset the histogram by setting all the counts to zero.
     */
    void
    reset()
    { std::fill(std::begin(_M_count), std::end(_M_count), 0); }

  private:

    std::vector<value_type> _M_bin;
    std::vector<std::size_t> _M_count;
  };
