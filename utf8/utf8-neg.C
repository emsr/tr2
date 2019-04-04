/* { dg-do compile } */
/* { dg-options "-std=c++1z -Wall -Werror" } */

const static char c0 = u8'';		// { dg-error "empty character" }
const static char c1 = u8'ab';  	// { dg-warning "constant too long" }
const static char c2 = u8'\u0124';	// { dg-warning "constant too long" }
const static char c3 = u8'\U00064321';  // { dg-warning "constant too long" }
