
#include <type_traits>

//  common_float performs the usual floating point conversion rules.
//  It uses common_type and it's biggest difference from that
//  is integral to double conversion.

// __common_float_help and __common_float_base could probably be squashed together.

template<typename... _Tp>
  struct common_float;

template<typename _Tp, bool>
  struct __common_float_help;

template<typename _Tp>
  struct __common_float_help<_Tp, true>
  : public std::common_type<_Tp>
  {
    using type = std::common_type_t<_Tp>;
  };

template<typename _Tp, bool>
  struct __common_float_base;

template<typename _Tp>
  struct __common_float_base<_Tp, true>
  : public __common_float_help<_Tp, std::is_arithmetic<_Tp>::value>
  {
    using type = double;
  };

template<typename _Tp>
  struct __common_float_base<_Tp, false>
  : public __common_float_help<_Tp, std::is_arithmetic<_Tp>::value>
  {
    using type = std::common_type_t<_Tp>;
  };

template<typename _Tp>
  struct common_float<_Tp>
  : public __common_float_base<_Tp, std::is_integral<_Tp>::value>
  {
  };

template<typename _Tp, typename... _Up>
  struct common_float<_Tp, _Up...>
  : public std::common_type<typename common_float<_Tp>::type,
			    typename common_float<_Up>::type...>
  {
  };

template<typename... _Tp>
  using common_float_t = typename common_float<_Tp...>::type;
