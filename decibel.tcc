#ifndef _DECIBEL_TCC
#define _DECIBEL_TCC 1


#include <iostream>


  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const decibel<_Tp>& __x)
    {
      __os << _Tp(__x);

      return __os;
    }

  template<typename _Tp,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       decibel<_Tp>& __x)
    {
      _Tp __xval;
      __is >> __xval;
      __x = decibel<_Tp>{__xval};

      return __is;
    }


#endif // _DECIBEL_TCC
