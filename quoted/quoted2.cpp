// $HOME/bin/bin/g++ -std=c++1y -fdiagnostics-color=auto -o quoted2 quoted2.cpp

#include <string>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>

namespace std2
{

  namespace __detail {

    /**
     * @brief Struct for delimited strings.
     *        The left and right delimiters can be different.
     */
    template<typename _String, typename _CharT>
      struct _Quoted_string
      {
	static_assert(std::is_reference<_String>::value
		   || std::is_pointer<_String>::value,
		      "String type must be pointer or reference");

	_Quoted_string(_String __str, _CharT __del, _CharT __esc)
	: _M_string(__str), _M_delim{__del}, _M_escape{__esc}, _M_delim2{__del}
	{ }

	_Quoted_string(_String __str, _CharT __del, _CharT __esc,
		       _CharT __del2)
	: _M_string(__str), _M_delim{__del}, _M_escape{__esc}, _M_delim2{__del2}
	{ }

	_Quoted_string&
	operator=(_Quoted_string&) = delete;

	_String _M_string;
	_CharT _M_delim;
	_CharT _M_escape;
	_CharT _M_delim2;
      };

    /**
     * @brief Inserter for delimited strings.
     *        The left and right delimiters can be different.
     */
    template<typename _CharT, typename _Traits>
      auto&
      operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		 const _Quoted_string<const _CharT*, _CharT>& __str)
      {
	std::basic_ostringstream<_CharT, _Traits> __ostr;
	__ostr << __str._M_delim;
	for (const _CharT* __c = __str._M_string; *__c; ++__c)
	  {
	    if (*__c == __str._M_delim || *__c == __str._M_escape)
	      __ostr << __str._M_escape;
	    __ostr << *__c;
	  }
	__ostr << __str._M_delim2;
	std::streamsize __n = std::max(__os.width(),
			static_cast<std::streamsize>(__ostr.str().length()));
	__os.rdbuf()->sputn(__ostr.str().data(), __n);
	__os.width(0);
	return __os;
      }

    /**
     * @brief Inserter for delimited strings.
     *        The left and right delimiters can be different.
     */
    template<typename _CharT, typename _Traits, typename _String>
      auto&
      operator<<(std::basic_ostream<_CharT, _Traits>& __os,
		 const _Quoted_string<_String, _CharT>& __str)
      {
	std::basic_ostringstream<_CharT, _Traits> __ostr;
	__ostr << __str._M_delim;
	for (auto& __c : __str._M_string)
	  {
	    if (__c == __str._M_delim || __c == __str._M_escape)
	      __ostr << __str._M_escape;
	    __ostr << __c;
	  }
	__ostr << __str._M_delim2;
	std::streamsize __n = std::max(__os.width(),
			static_cast<std::streamsize>(__ostr.str().length()));
	__os.rdbuf()->sputn(__ostr.str().data(), __n);
	__os.width(0);

	return __os;
      }

    /**
     * @brief Extractor for delimited strings.
     *        The left and right delimiters can be different.
     */
    template<typename _CharT, typename _Traits, typename _Alloc>
      auto&
      operator>>(std::basic_istream<_CharT, _Traits>& __is,
		 const _Quoted_string<std::basic_string<_CharT, _Traits, _Alloc>&,
				      _CharT>& __str)
      {
	__str._M_string.clear();

	_CharT __c;
	__is >> __c;
	if (!__is.good())
	  return __is;
	if (__c != __str._M_delim)
	  {
	    __is.unget();
	    __is >> __str._M_string;
	    return __is;
	  }
	std::ios_base::fmtflags __flags
	  = __is.flags(__is.flags() & ~std::ios_base::skipws);
	do
	  {
	    __is >> __c;
	    if (!__is.good())
	      break;
	    if (__c == __str._M_escape)
	      {
		__is >> __c;
		if (!__is.good())
		  break;
	      }
	    else if (__c == __str._M_delim2)
	      break;
	    __str._M_string += __c;
	  }
	while (true);
	__is.setf(__flags);

	return __is;
      }

  } // namespace __detail

  /**
   * @brief Manipulator for quoted strings.
   * @param __str    String to quote.
   * @param __delim  Character to quote string with.
   * @param __escape Escape character to escape itself or quote character.
   */
  template<typename _CharT>
    inline auto
    quoted(const _CharT* __string,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<const _CharT*, _CharT>(__string, __delim,
							     __escape);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline auto
    quoted(const std::basic_string<_CharT, _Traits, _Alloc>& __string,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<
			const std::basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
				__string, __delim, __escape);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline auto
    quoted(std::basic_string<_CharT, _Traits, _Alloc>& __string,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
      return __detail::_Quoted_string<
			std::basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
				__string, __delim, __escape);
    }

} // namespace std2

namespace __gnu_cxx
{

  template<char _Left>
    struct delim_pair
    {
      constexpr static char left = _Left;
      constexpr static char right = _Left;
    };

  template<>
    struct delim_pair<'('>
    {
      constexpr static char left = '(';
      constexpr static char right = ')';
    };

  template<>
    struct delim_pair<'['>
    {
      constexpr static char left = '[';
      constexpr static char right = ']';
    };

  template<>
    struct delim_pair<'{'>
    {
      constexpr static char left = '{';
      constexpr static char right = '}';
    };

  template<>
    struct delim_pair<'<'>
    {
      constexpr static char left = '<';
      constexpr static char right = '>';
    };

  template<typename _CharT>
    auto
    delimited(const _CharT* __str, _CharT __delim = _CharT('('),
	      _CharT __escape = _CharT('\\'), _CharT __delim2 = _CharT(')'))
    {
      return std2::__detail::_Quoted_string<const _CharT*, _CharT>(
		__str, __delim, __escape, __delim2);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    auto
    delimited(const std::basic_string<_CharT, _Traits, _Alloc>& __str,
	      _CharT __delim = _CharT('('), _CharT __escape = _CharT('\\'),
	      _CharT __delim2 = _CharT(')'))
    {
      return std2::__detail::_Quoted_string<
		const std::basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
				__str, __delim, __escape, __delim2);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    auto
    delimited(std::basic_string<_CharT, _Traits, _Alloc>& __str,
	      _CharT __delim = _CharT('('), _CharT __escape = _CharT('\\'),
	      _CharT __delim2 = _CharT(')'))
    {
      return std2::__detail::_Quoted_string<
			std::basic_string<_CharT, _Traits, _Alloc>&, _CharT>(
				__str, __delim, __escape, __delim2);
    }

} // namespace __gnu_cxx

void
error(const std::string& message)
{
  std::cerr << "\n  **  Error: " << std2::quoted(message) << "  **\n";
}

int
main()
{
  std::stringstream ss;
  std::string original = "foolish me";
  std::string round_trip;

  ss << std2::quoted(original);
  std::cout << "original: " << original << '\n';
  std::cout << "std2::quoted(original): " << std2::quoted(original) << '\n';
  ss >> std2::quoted(round_trip);
  std::cout << "round_trip: " << round_trip << '\n';

  std::cout << "original: " << original << '\n';     // outputs: foolish me
  std::cout << "round_trip: " << round_trip << '\n';   // outputs: foolish me

  assert(original == round_trip); // assert will not fire

  std::cout << std2::quoted("She said \"Hi!\"") << '\n';  // outputs: "She said \"Hi!\""
  std::cout << std2::quoted("She said \"Hi!\"", '\'', '&') << '\n';  // outputs: 'She said "Hi!"'
  std::cout << std2::quoted("Harry & Sally", '\'', '&') << '\n';  // outputs: 'Harry && Sally'

  std::cout << "Enter quoted string: ";
  std::string quote;
  //std::cin >> std2::quoted(quote);
  std::cout << "Quoted string: " << quote << '\n';

  std::cout << "Quoted string: " << std2::quoted("\"There's a dead bishop on the landing!\"") << '\n';

  //  Test skipws correctness.
  ss.seekg(0);
  ss.seekp(0);
  ss.clear();
  ss << std2::quoted("Hello Goodbye") << ' ' << 1 << ' ' << 2;
  std::cout << "ss.str(): " << ss.str() << '\n';
  std::string song;
  int thing1, thing2;
  ss >> std2::quoted(song) >> thing1 >> thing2;
  std::cout << "song: " << song << '\n';
  std::cout << "thing1: " << thing1 << '\n';
  std::cout << "thing2: " << thing2 << '\n';
  assert(song == "Hello Goodbye");
  assert(thing1 == 1);
  assert(thing2 == 2);

  //  Test read of unquoted string.
  ss.seekg(0);
  ss.seekp(0);
  ss.clear();
  ss << "Alpha Omega";
  std::string test;
  ss >> std2::quoted(test);
  assert(test == "Alpha");

  //  Test delimited string extension.
  ss.seekg(0);
  ss.seekp(0);
  ss.clear();
  ss << __gnu_cxx::delimited(original);
  std::cout << "original: " << original << '\n';
  std::cout << "__gnu_cxx::delimited(original): " << __gnu_cxx::delimited(original) << '\n';
  ss >> __gnu_cxx::delimited(round_trip);
  std::cout << "round_trip: " << round_trip << '\n';

  error(original);
  error("My biscuits are burnin'!");

  std::cout << "[" << std::left << std::setfill('x') << std::setw(20) << R"("AB \"CD\" EF")" << "]" << std::endl;
  std::cout << "[" << std::left << std::setfill('y') << std::setw(20) << std::quoted(R"(GH "IJ" KL)") << "]" << std::endl;
  std::cout << "[" << std::left << std::setfill('z') << std::setw(20) << std2::quoted(R"(GH "IJ" KL)") << "]" << std::endl;
  std::cout << "[" << std::left << std::setfill('z') << std::setw(20) << std2::quoted(R"(GH "IJ" KL)") << "]" << std::endl;
}
