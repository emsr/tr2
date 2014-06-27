//  https://isocpp.org/files/papers/n4051.html

template<typename T> struct A {};
template<typename T> using B = int;

template<template<typename> class X> struct C;
C<A> ca; // ok
C<B> cb; // ok, not a class template
template<template<typename> typename X> struct D; // error, cannot use typename here

/*
static tree
cp_parser_type_parameter (cp_parser* parser, bool *is_parameter_pack)

Try this:
static bool
cp_parser_is_type_parameter_key(cp_parser* parser)
{
  cp_token *token;

  token = cp_lexer_peek_token (parser->lexer);

  switch (token->keyword)
    {
    case RID_CLASS:
    case RID_TYPENAME:
      return true;
    default:
      gcc_unreachable ();
      break;
    }

  return false;
}

This iexists...
static enum tag_types
cp_parser_token_is_class_key (cp_token* token)
{

In cp-tree.h
/* An enumeration of the kind of tags that C++ accepts.  */
enum tag_types {
  none_type = 0, /* Not a tag type.  */
  record_type,   /* "struct" types.  */
  class_type,    /* "class" types.  */
  union_type,    /* "union" types.  */
  enum_type,     /* "enum" types.  */
  typename_type  /* "typename" types.  */
};

*/
