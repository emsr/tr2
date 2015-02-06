//
//  COPYRIGHT:  Copyright 2010 - 2014
//              Alion Science and Technology
///
///  \def  HISTOGRAM_H
///
///  \brief  A guard for the histogram class header.
///
#ifndef HISTOGRAM_H
#define HISTOGRAM_H 1

///
///  \file histogram.h  This file contains the definition of the histogram class.
///

#include <vector>
#include <algorithm>
#include <stdexcept>

/**
 *  @brief  A simple histogram class.
 *          A number N of bins are described by a sorted array of N+1
 *          numbers of which the first N represent the left or starting value
 *          of the bins and the last is the ending point of the last bin.
 *          Bin 0 is the left wing, and bin N+1 is te right wing.
 */
template<typename Tp>
  class histogram
  {
  public:

    ///  Type of datum.
    using value_type = Tp;
    ///  Type for sizes.
    using size_type = std::size_t;

    class iterator
    {
    public:
      iterator()
      : m_index{-1},
        m_histogram{nullptr}
      { }

      iterator(size_type index, histogram<Tp> * hist)
      : m_index{index},
        m_histogram{hist}
      { }

      iterator
      operator++()
      {
        if (this->m_index == m_histogram->_M_bin.size() - 1) // Right tail
          this->m_index = static_cast<size_type>(-1);
        else
          ++this->m_index;
        return *this;
      }

      iterator
      operator++(int)
      {
        auto __temp(*this);
        ++this;
        return __temp;
      }

      iterator
      operator--()
      {
        if (this->m_index == 0 || this->m_index = static_cast<size_type>(-1))
          this->m_index = static_cast<size_type>(-1); // What should we do?  Whis maybe shouldn't be end.
        else
          --this->m_index;
        return *this;
      }

      iterator
      operator--(int)
      {
        auto __temp(*this);
        --this;
        return __temp;
      }

      //bin&
      //operator*()

    private:
      size_type m_index = -1;
      histogram<Tp> * m_histogram = nullptr;
    };

    /**
     *  @brief  Create a histogram from a pair of bounds and a number of bins.
     *
     *  @param[in]  num_bins  Number of bins.
     *  @param[in]  xmin      Lower bound of the first bin.
     *  @param[in]  xmax      Upper bound of the last bin.
     */
    histogram(size_type num_bins, value_type xmin, value_type xmax)
    : _M_bin(num_bins + 1), _M_count(num_bins + 2)
    {
      _M_bin[num_bins] = xmax;
      value_type delta = (xmax - xmin) / num_bins;
      for (size_type i = 0; i < num_bins; ++i)
	_M_bin[i] = xmin + i * delta;
    }

    /**
     *  @brief  Create a histogram with a range of bin lower bounds.
     *
     *  @param[in]  xbegin  Beginning iterator of a range of bin lower bounds.
     *  @param[in]  xend    Ending iterator of a range of bin lower bounds.
     */
    template<typename Iter>
      histogram(Iter xbegin, Iter xend)
      : _M_bin{}, _M_count{}
      {
        for (; xbegin != xend; ++xbegin)
        {
          _M_count.push_back(0);
          _M_bin.push_back(*xbegin);
        }
        _M_count.push_back(0);
        if (!std::is_sorted(std::begin(_M_bin), std::end(_M_bin)))
          throw std::domain_error("histogram: bin limits not sorted");
      }

    /**
     *  @brief  Constructor taking initializer list of bin boundaries.
     *
     *  @param[in]  il  Input initializer list of bin boundaries.
     */
    histogram(std::initializer_list<value_type> il)
    : _M_bin{il},
      _M_count(il.size() + 1)
    {
      if (!std::is_sorted(std::begin(_M_bin), std::end(_M_bin)))
        throw std::domain_error("histogram: bin limits not sorted");
    }

    /**
     *  @brief  Insertion operator.
     *  @param[in]  x  The input value.
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
     *  @brief  Insert a range of values.
     *  @param[in] begin The beginning iterator for input data.
     *  @param[in] end The ending iterator for input data.
     */
    template<typename Iter>
      void
      insert(Iter begin, Iter end)
      {
	for(; begin != end; ++begin)
          *this << *begin;
      }

    /**
     *  @brief  Insert a range of values.
     *  @param[in] begin The beginning iterator for input data.
     *  @param[in] end   The ending iterator for input data.
     *  @param[in] fun   The function taking *Iter and returning value_type.
     */
    template<typename Iter, typename Conv>
      void
      insert(Iter begin, Iter end, Conv fun)
      {
	for(; begin != end; ++begin)
          *this << fun(*begin);
      }

    /**
     *  @brief  Return the number of bins.
     */
    size_type
    size() const noexcept
    { return _M_bin.size() - 1; }

    /**
     *  @brief  Return the iterator to the start of the histogram - left wing.
     */
    iterator
    begin()
    { return iterator{0, this}; }

    /**
     *  @brief  Return the iterator to one past the end of the histogram.
     */
    iterator
    end()
    { return iterator{}; }

    void
    swap(histogram<value_type> & hist) noexcept
    {
      this->_M_bin.swap(hist._M_bin);
      this->_M_count.swap(hist._M_count);
    }

    /**
     *  @brief  Return the number of items including items above and below the limit.
     */
    size_type
    count() const noexcept
    { return std::accumulate(std::begin(this->_M_count),
			     std::end(this->_M_count), 0); }

    /**
     *  @brief  Return the number of items from below the lower limit up to bin @c i.
     *  @param[in]  i  The bin index.
     */
    size_type
    count(size_type i) const noexcept
    {
      size_type m = (i < this->_M_count.size() ? i : this->_M_count.size());
      return std::accumulate(this->_M_count.begin(),
			     this->_M_count.begin() + m, 0);
    }

    /**
     *  @brief  Return the mean - excluding items above and below the limit.
     */
    value_type
    mean() const noexcept
    {
      size_type num = 0;
      value_type sum = 0;
      for (size_type i = 1; i <= size(); ++i)
      {
        num += this->_M_count[i];
        sum += this->_M_count[i] * (value(i) + value(i + 1)) / 2;
      }

      return sum / num;
    }

    /**
     *  @brief  Return the standard deviation - excluding items above and below the limit.
     */
    value_type
    sigma() const noexcept
    {
      size_type num = 0;
      value_type sum = 0;
      for (size_type i = 1; i <= size(); ++i)
      {
        num += this->_M_count[i];
        sum += this->_M_count[i] * (value(i) + value(i + 1)) / 2;
      }
      value_type mean = sum / num;

      value_type sig = 0;
      for (size_type i = 1; i <= size(); ++i)
      {
        value_type del = (value(i) + value(i + 1)) / 2 - mean;
        sig += this->_M_count[i] * del * del;
      }
      sig /= num;

      return std::sqrt(sig);
    }

    /**
     *  @brief  Return the count of the ith bin where 1 <= i <= size().
     *                 the count below the lower limit for i == 0.
     *                 the count above the upper limit for i == size() + 1.
     *  @param[in]  i  The bin index.
     */
    size_type
    operator[](size_type i) const noexcept
    { return this->_M_count[i]; }

    /**
     *  @brief  Return the left value of the ith bin where 1 <= i <= size().
     *  @param[in]  i  The bin index.
     *  @return  The left boundary value of the bin.
     */
    value_type
    value(size_type i) const noexcept
    { return this->_M_bin[i - 1]; }

    /**
     *  @brief  Return the left value of the ith bin where 1 <= i <= size().
     *  @param[in]  i  The bin index.
     *  @return  The left boundary value of the bin.
     */
    value_type
    lower_bound(size_type i) const noexcept
    { return this->_M_bin[i - 1]; }

    /**
     *  @brief  Return the right value of the ith bin where 1 <= i <= size().
     *  @param[in]  i  The bin index.
     *  @return  The left boundary value of the bin.
     */
    value_type
    upper_bound(size_type i) const noexcept
    { return this->_M_bin[i]; }

    /**
     *  @brief  Reset the histogram by setting all the counts to zero.
     */
    void
    reset()
    { std::fill(std::begin(this->_M_count),
		std::end(this->_M_count), 0); }

  private:

    ///  The array of bin starting values.
    ///  The first element [0] is the upper bound of the left tail
    ///  and the lower bound of the first bin.
    ///  The last element [size() - 1] is the upper bound of the last bin
    ///  and the lower bound of the right tail.
    std::vector<value_type> _M_bin;

    ///  The array of counts in each bin.
    ///  The first element [0] is the count of the left tail.
    ///  The last element [size() - 1] is the count of the right tail.
    ///  This array is one larger than the bin starting value array.
    std::vector<size_type> _M_count;
  };


#endif // HISTOGRAM_H
