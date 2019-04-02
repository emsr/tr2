#ifndef _LEVEL_TCC
#define _LEVEL_TCC 1


#include <iostream>


  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits>&
    operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	       const level<_Tp, _Unit>& __x)
    {
      __os << _Tp(__x);

      return __os;
    }

  template<typename _Tp, typename _Unit,
	   typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits>&
    operator>>(std::basic_istream<_CharT, _Traits>& __is,
	       level<_Tp, _Unit>& __x)
    {
      _Tp __xval;
      __is >> __xval;
      __x = level<_Tp, _Unit>{__xval};

      return __is;
    }

  template<typename _Tp, typename _CharT, typename _Traits>
    std::basic_ostream<_CharT, _Traits> &
    operator<<(std::basic_ostream<_CharT, _Traits> & __os,
	       const neper<_Tp> & __x)
    {
      __os << _Tp(__x);

      return __os;
    }

  template<typename _Tp, typename _CharT, typename _Traits>
    std::basic_istream<_CharT, _Traits> &
    operator>>(std::basic_istream<_CharT, _Traits> & __is,
	       neper<_Tp> & __x)
    {
      _Tp __xval;
      __is >> __xval;
      __x = neper<_Tp>{__xval};

      return __is;
    }


#endif // _LEVEL_TCC
