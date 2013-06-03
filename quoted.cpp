// /home/ed/bin/bin/g++ -std=c++11 -o quoted quoted.cpp

#include <string>
#include <cstddef>
#include <iostream>

namespace std
{

  namespace __detail {

    template<typename _String, typename _CharT>
      struct _Quoted_string;

    template<typename _CharT>
      struct _Quoted_string<const _CharT*, _CharT>
      {
	_Quoted_string(const _CharT* __str, _CharT __del, _CharT __esc)
	: __string{__str}, __delim{__del}, __escape{__esc}
	{ }

	_Quoted_string&
	operator=(_Quoted_string&) = delete;

	const _CharT* __string;
	_CharT __delim;
	_CharT __escape;
      };

    template<typename _CharT, typename _Traits, typename _Alloc>
      struct _Quoted_string<const basic_string<_CharT, _Traits, _Alloc>&, _CharT>
      {
	_Quoted_string(const basic_string<_CharT, _Traits, _Alloc>& __str,
		       _CharT __del, _CharT __esc)
	: __string{__str}, __delim{__del}, __escape{__esc}
	{ }

	const basic_string<_CharT, _Traits, _Alloc>& __string;
	_CharT __delim;
	_CharT __escape;
      };

    template<typename _CharT, typename _Traits, typename _Alloc>
      struct _Quoted_string<basic_string<_CharT, _Traits, _Alloc>&, _CharT>
      {
	_Quoted_string(const basic_string<_CharT, _Traits, _Alloc>& __str,
		       _CharT __del, _CharT __esc)
	: __string{__str}, __delim{__del}, __escape{__esc}
	{ }

	basic_string<_CharT, _Traits, _Alloc>& __string;
	_CharT __delim;
	_CharT __escape;
      };

  } // namespace __detail

  template<typename _CharT>
    __detail::_Quoted_string<const _CharT*, _CharT>
    quoted(const _CharT* __str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<const _CharT*, _CharT>(
		__str, __delim, __escape);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    __detail::_Quoted_string<const basic_string<_CharT, _Traits, _Alloc>&, _CharT>
    quoted(const basic_string<_CharT, _Traits, _Alloc>& __str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<const basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
		__str, __delim, __escape);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    __detail::_Quoted_string<basic_string<_CharT, _Traits, _Alloc>&, _CharT>
    quoted(basic_string<_CharT, _Traits, _Alloc>& __str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
		__str, __delim, __escape);
    }

  //  Hacked from filesystem (templatized, etc.)
  template<typename _CharT, typename _Traits, typename _String>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const __detail::_Quoted_string<_String, _CharT>& __str)
    {
      __os << __str.__delim;
      for (auto& __c : __str)
	{
	  if (__c == __str.__delim || __c == __str.__escape)
	    __os << __str.__escape;
	  __os << __c;
	}
      __os << __str.__delim;

      return __os;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       __detail::_Quoted_string<basic_string<_CharT, _Traits, _Alloc>&, _CharT>& __str)
    {
      __str.__string.clear();

      _CharT __c;
      do
	{
	  __is >> __c;
	  if (!__is.good())
	    break;
	}
      while (__c != __str.__delim);
      do
	{
	  __is >> __c;
	  if (!__is.good())
	    break;
	  if (__c == __str.__escape)
	    {
	      __is >> __c;
	      if (!__is.good())
		break;
	      __str.__string += __c;
	    }
	  else if (__c == __str.__delim)
	    break;
	  else
	    __str.__string += __c;
	}
      while (true);

      return __is;
    }

} // namespace std

int
main()
{
  std::cout << "Enter quoted string: ";
  std::string quote;
  std::cin >> std::quoted(quote);
  std::cout << "Quoted string: " << quote;
}
