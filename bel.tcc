#ifndef _BEL_TCC
#define _BEL_TCC 1


#include <iostream>


  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const bel<_Tp, _Unit>& __x)
    {
      __os << _Tp(__x);

      return __os;
    }

  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       bel<_Tp, _Unit>& __x)
    {
      _Tp __xval;
      __is >> __xval;
      __x = bel<_Tp, _Unit>{__xval};

      return __is;
    }


#endif // _BEL_TCC
