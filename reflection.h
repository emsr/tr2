namespace std
{

  enum access_levels
  {
    public_access,
    protected_access,
    private_access
  };

  enum enumerator
  {
    template<typename _Enum>
      struct list_size;

    template<typename _Enum, std::size_t _Idx>
      struct identifier;

    template<typename _Enum, std::size_t _Idx>
      struct value;
  };

  enum base_class
  {
    template<typename _Enum>
      struct list_size;

    template<typename _Enum, std::size_t _Idx>
      struct type;

    template<typename _Enum, std::size_t _Idx>
      struct is_virtual;

    template<typename _Enum, std::size_t _Idx>
      struct access_level;
  };

  enum class_member
  {
    template<typename _Enum>
      struct list_size;

    template<typename _Enum, std::size_t _Idx>
      struct name;

    template<typename _Enum, std::size_t _Idx>
      struct pointer;

    template<typename _Enum, std::size_t _Idx>
      struct access_level;
  };

  enum nested_type
  {
    template<typename _Enum>
      struct list_size;

    template<typename _Enum, std::size_t _Idx>
      struct identifier;

    template<typename _Enum, std::size_t _Idx>
      struct type;

    template<typename _Enum, std::size_t _Idx>
      struct access_level;
  };

}

