// Based on an answer on SO: http://stackoverflow.com/a/14861289/680359
// But modernized and genericized.

#include <string>
#include <iostream>
#include <iomanip>
#include <experimental/string_view>

  namespace __detail
  {

    /**
     * @brief Struct for centered strings.
     */
    template<typename _String>
      struct _Centered_string
      {
	static_assert(std::is_reference<_String>::value
		   || std::is_pointer<_String>::value,
		      "String type must be pointer or reference");

	_Centered_string(_String __str)
	: _M_str(__str)
	{ }

	_String _M_str;
      };

    /**
     * @brief Struct for centered strings.
     */
    template<typename _CharT>
      struct _Centered_string<const _CharT*>
      {
	_Centered_string(const _CharT* __str)
	: _M_str(__str)
	{ }

	std::experimental::basic_string_view<_CharT> _M_str;
      };

    /**
     * @brief Struct for centered strings.
     */
    template<typename _CharT, typename _Traits, typename _Alloc>
      struct _Centered_string<const std::basic_string<_CharT, _Traits, _Alloc>&>
      {
	_Centered_string(const std::basic_string<_CharT, _Traits, _Alloc>& __str)
	: _M_str(__str)
	{ }

	std::experimental::basic_string_view<_CharT> _M_str;
      };

  } // namespace __detail

  template<typename _CharT>
    inline auto
    centered(const _CharT* __str)
    {
      return __detail::_Centered_string<const _CharT*>(__str);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline auto
    centered(const std::basic_string<_CharT, _Traits, _Alloc>& __str)
    {
      using __StringT = std::basic_string<_CharT, _Traits, _Alloc>;
      return __detail::_Centered_string<const __StringT &>(__str);
    }

  template<typename _CharT, typename _Traits>
    inline auto
    centered(const std::experimental::basic_string_view<_CharT, _Traits>& __str)
    {
      using __StringT = std::experimental::basic_string_view<_CharT, _Traits>;
      return __detail::_Centered_string<const __StringT &>(__str);
    }

  template<typename _String, typename _CharT, typename _Traits>
    auto&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const __detail::_Centered_string<_String>& __str)
    {
      std::streamsize __width = __os.width();
      if (__width > __str._M_str.length())
	{
	  std::streamsize __left = (__width + __str._M_str.length()) / 2;
	  __os.width(__left);
	  __os << __str._M_str;
	  __os.width(__width - __left);
	  __os << "";
	}
      else
	__os << __str._M_str;

      return __os;
    }
