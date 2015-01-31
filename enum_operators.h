#ifndef _BITMASK_H
#define _BITMASK_H

#include<experimental/type_traits>

  template<typename _Enum>
    constexpr bool enable_bitmask_operators = false;

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum>
    operator|(_Enum __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      return static_cast<_Enum>(static_cast<_UnderT>(__a)
                              | static_cast<_UnderT>(__b));
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum>
    operator&(_Enum __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      return static_cast<_Enum>(static_cast<_UnderT>(__a)
                              & static_cast<_UnderT>(__b));
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum>
    operator^(_Enum __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      return static_cast<_Enum>(static_cast<_UnderT>(__a)
                              ^ static_cast<_UnderT>(__b));
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum>
    operator~(_Enum __a)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      return static_cast<_Enum>(~static_cast<_UnderT>(__a));
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum&>
    operator|=(_Enum& __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      __a = static_cast<_Enum>(static_cast<_UnderT>(__a)
                             | static_cast<_UnderT>(__b));
      return __a;
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum&>
    operator&=(_Enum& __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      __a = static_cast<_Enum>(static_cast<_UnderT>(__a)
                             & static_cast<_UnderT>(__b));
      return __a;
    }

  template<typename _Enum>
    std::enable_if_t<enable_bitmask_operators<_Enum>, _Enum&>
    operator^=(_Enum& __a, _Enum __b)
    {
      using _UnderT = std::underlying_type_t<_Enum>;
      __a = static_cast<_Enum>(static_cast<_UnderT>(__a)
                             ^ static_cast<_UnderT>(__b));
      return __a;
    }

#endif // _BITMASK_H

