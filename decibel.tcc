
#include <iostream>

  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const decibel<_Tp>& __x)
    {
      typedef std::basic_ostream<_CharT, _Traits>  __ostream_type;
      typedef typename __ostream_type::ios_base    __ios_base;

      const std::streamsize __precision = __os.precision();
      __os.precision(std::numeric_limits<_Tp>::max_digits10);
      const _CharT __fill = __os.fill();
      const _CharT __space = __os.widen(' ');
      __os.fill(__space);
      const typename __ios_base::fmtflags __flags = __os.flags();
      __os.flags(__ios_base::scientific | __ios_base::left);

      const _CharT * __dB = __os.widen("_dB");
      __os << _Tp(__x) << __dB;

      __os.flags(__flags);
      __os.fill(__fill);
      __os.precision(__precision);

      return __os;
    }

  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       decibel<_Tp>& __x)
    {
      typedef std::basic_istream<_CharT, _Traits>  __istream_type;
      typedef typename __istream_type::ios_base    __ios_base;

      const typename __ios_base::fmtflags __flags = __is.flags();
      __is.flags(__ios_base::dec | __ios_base::skipws);

      // Think on this bit:  stringstream might be better.
      std::string __dB;

      _Tp __xval;
      __is >> __xval >> __dB;
      __x = decibel<_Tp>{__xval};

      __is.flags(__flags);

      return __is;
    }
