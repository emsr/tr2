// $HOME/bin_git/bin/g++ -std=c++11 -c -o fold_barfage.o fold_barfage.cpp

// $HOME/bin_git/bin/g++ -std=c++11 -pedantic -c -o fold_barfage.o fold_barfage.cpp

// $HOME/bin_git/bin/g++ -std=c++1z -c -o fold_barfage.o fold_barfage.cpp

/*
__MATH_INLINE int
__NTH (__finite (double __x))
{
  return (__extension__
	  (((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
	     | 0x800fffffu) + 1) >> 31));
}


/usr/include/bits/mathinline.h: In function \u2018int __finite(double)\u2019:
/usr/include/bits/mathinline.h:948:54: error: expected cast-expression before \u2018)\u2019 token
    (((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
                                                      ^
/usr/include/bits/mathinline.h:949:7: error: expected \u2018)\u2019 before \u2018|\u2019 token
       | 0x800fffffu) + 1) >> 31));
       ^
/usr/include/bits/mathinline.h:949:34: error: expected \u2018)\u2019 before \u2018;\u2019 token
       | 0x800fffffu) + 1) >> 31));
                                  ^
/usr/include/bits/mathinline.h:949:34: error: expected \u2018)\u2019 before \u2018;\u2019 token
/usr/include/bits/mathinline.h:949:34: error: expected \u2018)\u2019 before \u2018;\u2019 token
*/

inline int
__finite (double __x)
{
  return ((((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
	     | 0x800fffffu) + 1) >> 31));
}
