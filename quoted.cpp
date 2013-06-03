

namespace std
{

  namespace __detail {
    template <class _CharT, class _Traits, class _Allocator>
      struct _Quoted_string
      {
	_Quoted_string(const basic_string<_CharT, _Traits, _Allocator>& __str)
	: __string{__str}
	{ }

	basic_string<_CharT, _Traits, _Allocator> __string;
      };
  } // namespace __detail

  template <class _CharT>
    T11
    quoted(const _CharT* __str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
    }

  template <class _CharT, class _Traits, class _Allocator>
    T12
    quoted(const basic_string<_CharT, _Traits, _Allocator>& __str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
    }

  template <class _CharT, class _Traits, class _Allocator>
    T13
    quoted(basic_string<_CharT, _Traits, _Allocator>& str,
	   _CharT __delim = _CharT('"'), _CharT __escape = _CharT('\\'))
    {
    }

  //  Hacked from filesystem (templatized, etc.)
  template <class _CharT, class _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const path& pth)
    {
      std::size_t __ppos = 0;
      __os << __delim;
      do
	{
	  std::size_t __pos = pth.string().find(__delim, ppos);
	  __os << pth.string().substr(ppos, pos - ppos);
	  if (pos == std::string::npos)
	    break;
	  else
	    {
	      __os << __escape << '"';
              __ppos = __pos + 1;
	    }
	}
      while (true);
      __os << __delim;

      return __os;
    }

  template <class _CharT, class _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       path& pth)
    {
      pth.clear();
      std::string pathname;
      char __c;
      do
	__is.get(__c);
      while (__c != __delim);
      do
	{
	  __is.get(__c);
	  if (!__is.good())
	    break;
	  if (__c == __escape)
	    {
	      __is.get(__c);
	      if (!__is.good())
		break;
	      pathname += static_cast<char>(__c);
	    }
	  else if (__c == __delim)
	    break;
	  else
	    pathname += static_cast<char>(__c);
	}
      while (true);

      pth = path(pathname);

      return __is;
    }

} // namespace std
