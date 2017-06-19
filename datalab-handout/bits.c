/* 
 * CS:APP Data Lab 
 * 
 * Choi Min Yeop 20160655
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
/* 
 * fp_func4 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func4(unsigned uf) {
/* extract each part */
  //unsigned sign = (uf >> 31) & 0x1;
  //unsigned exp  = (uf >> 23) & 0xFF;
  //unsigned frac = (uf      ) & 0x7FFFFF;  
/* if exponent part is 0, frac * 2.
 * Even if frac*2 is overflow, overflowed part  will goto exponent part
 */
  //if(exp == 0x00) return (sign << 31) + (frac << 1);
/* if expont part is 0xFF, It's NaN or Infinity
 * Infinity * 2 = Infinity, -Infinity * 2 = -Infinity, NaN -> return NaN
 */
  //else if(exp == 0xFF) return uf;
/* Otherwise just plus 1 exp
 */
  //else return (sign << 31) + ((exp + 1) << 23) + frac;
unsigned sign_mask = 0x80000000;
unsigned exp_mask  = 0x7F800000; 
unsigned frac_mask = 0x007FFFFF;
unsigned result;
unsigned tmp;
if((uf&exp_mask) == exp_mask && (uf & frac_mask) != 0){
return uf;
}
else if ((uf&exp_mask) == 0){
result = (uf & sign_mask) | ((uf << 1) & (exp_mask + frac_mask));
}
else if ((uf&exp_mask) != 0 && (uf & exp_mask) != exp_mask){
result = uf + 0x800000;
}
else{
result = uf;
}
return result;
}
/* 
 * is_addition_no_problem - Will it be okay(no overflow) when we add x and y?
 *   If there is an overflow, return 0. Otherwise, return 1.
 *   Example: is_addition_no_problem(0x80000000,0x80000000) = 0,
 *            is_addition_no_problem(0x80000000,0x70000000) = 1, 
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 20
 *   Points: 3
 */
int is_addition_no_problem(int x, int y) {
/* Quetion : Let's look at adding 0x00000001 and 0xFFFFFFFF
 * there is no problem with adding but there is an overflow!
 */
/* Explain)
 * There are some cases when adding
 * 1. If x's and y's sign are : (x ^ y) >> 31 & 0x1
 *      1) different (== 1)
 *        There is no problem. return 1 (But look at the Question)
 *      2) same (== 0)
 *        2. If x's (or y's) sign and sign of the result of adding x, y are : (x ^ (x + y)) >> 31 & 0x1
 *             1) different (== 1)
 *               Overflow! return 0
 *             2) same (== 0)
 *               No Problem return 1
 * Only when (1.) == 0 && (2.) == 1, return 0, Otherwise return 1
 * We can do this in one line : return (1.) | ~(2.) 
 */
  return ((x ^ y) | ~(x ^ (x + y))) >> 31 & 0x1;
}
/* 
 * is_x_fits_in_16_bit - Returns 1 if x fits in 16-bit, 2's complement.
 *   Examples: is_x_fits_in_16_bit(34000) = 0, is_x_fits_in_16_bit(-32768) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 8
 *   Points: 1
 */
int is_x_fits_in_16_bit(int x) {
/* if x is fits in 16 bits,
 * 16th ~ 31st bits in x must be all same with sign bit(32nd bit)
 */
  return ! ((x >> 15) ^ (x >> 31));
}
/* 
 * fp_func1 - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func1(unsigned uf) {
/* Extract parts in uf
 */
  unsigned sign = (uf >> 31) & 0x1;
  unsigned exp  = (uf >> 23) & 0xFF;
  unsigned frac = (uf      ) & 0x7FFFFF;
/* If uf is NaN or Infinity, return argument(Infinity * 0.5 = Infinity)
 */
  if(exp == 0xff) return uf;
  else if(exp <= 1){ //Denormalized value
    if((frac & 0x3) == 0x3){ // frac = k-epsilon -> k * 0.5
      return (sign << 31) + (exp << 22) + ((frac + 1) >> 1); 
    }
    else{// other frac / 2
      return (sign << 31) + (exp << 22) + (frac >> 1);
    }
  }
  // otherwise, exp - 1
  return (sign << 31) + ((exp - 1) << 23) + frac;  
}
/* 
 * fp_func3 - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 10
 *   Points: 2
 */
unsigned fp_func3(unsigned uf) {
/* Extract uf
 * Delete unused part(sign)
 */
  unsigned exp  = (uf >> 23) & 0xFF;
  unsigned frac = (uf      ) & 0x7FFFFF;
/* if NaN, return uf */
  if(exp == 0xFF && frac != 0x0) return uf;
/* otherwise set sign bit to 0;*/
  return (exp << 23) + frac;
}
/* 
 * fp_func2 - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
int fp_func2(unsigned uf) {
  unsigned sign =  (uf >> 31) & 0x1;
  unsigned exp  =  (uf >> 23) & 0xFF;
  unsigned frac = ((uf      ) & 0x7FFFFF) | 0x800000;
  int res;
/* if uf is infinity or NaN or absolute value is too large, return 0x80000000*/
  if(exp == 0xFF || exp >= 0x80 + 22) return 0x80000000;
/* if uf is absulute value is too small, return 0*/
  else if(exp < 127) return 0;
  else{
/* actually frac real value is frac >> 22(in expression)
 * And calculating including exponent is like this
 * (real value) = frac >> 22 << (exp-0x80(BIAS))
 * and we can do in one shift like this(and not to loose data by shifting)
 * frac >> (22 - (exp - 0x80)
 */
    res = frac >> (22 - (exp - 0x80));
/* set sign
 */
    if(sign == 1) return -res;
    return res;
  } 
}
/* 
 * extract_nth_byte - Extract the nth byte from a word x and return it.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: extract_nth_byte(0xFF34AB78,1) = 0xAB
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 6
 *   Points: 2
 */
int extract_nth_byte(int x, int n) {
/* extract byte with (0xFF & ?)
 * need to rshift x to move bytes that we want to extract to LSB
 * if n == 0, rshift 0, n == 1 rshift 8, n == 2 rshift 16, n == 3 rshift 24
 * same as rshift n*8 , same as rshift n << 3
 */
  return 0xFF & (x >> (n << 3));
}
/* 
 * substitute_byte(x,n,c) - In a word x, replace the nth byte with a new byte c.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: substitute_byte(0xffffffff,1,0xab) = 0xffffabff
 *   Assume that 0 <= n <= 3 and 0 <= c <= 255.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 10
 *   Points: 3
 */
int substitute_byte(int x, int n, int c) {
  int pb = n<<3;
/*     ((0xFF & (x >> pb)) << pb)             : extract nth byte(keep in nth) 
 * x ^ ((0xFF & (x >> pb)) << pb)             : set 0x00 nth byte 
 * x ^ ((0xFF & (x >> pb)) << pb) ^ (c << pb) : set nth byte with c
 */
  return x ^ ((0xFF & (x >> pb)) << pb) ^ (c << pb);
}
/*
 * floor_log_2 - return floor(log base 2 of x), where x > 0
 *   Example: floor_log_2(16) = 4
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 90
 *   Points: 4
 */
int floor_log_2(int x) {
/*
  calculate floor_log_2 is same as finding 1 in the bits that is in the biggest position in x
  find it with binary searching
 */
  int tmp; // if 0, no bits. if not 0, there is some bits(where in finding interval)
  int sw; // if tmp is 0, sw is 0. if not, set sw with the size of the half of the finding interval 
  int cnt = 0; // position of biggest position of 1

  tmp = (x >> 16) & ((0xFF << 8) | 0xFF); // extract bits on the top of 16bits(half of 32bits)
  sw  = ((!tmp) + (~0)) & 16; /* if tmp == 0, !tmp + (~0) = 0, (!tmp + (~0)) & 16 = 0
  				 if tmp != 0, !tmp + (~0) = -1(0xFFFFFFFF), (!tmp + (~0)) & 16 = 16*/
  cnt = cnt + sw; // if sw == 16, it is true that there is some bits in top 16bits. So add sw in cnt. if not add 0. 
  x   = x >> sw; /* if sw == 16, we need to search on top 16bits.
                    So we need to rshift to take it down to LSB.
                    if sw == 0, we need to search on bottom 16bits. But bottom 16bits are already in LSB. rshift 0.
                 */ 
  tmp = (x >> 8) & 0xFF; // same method with 16bits
  sw  = ((!tmp) + (~0)) & 8;
  cnt = cnt + sw;
  x   = x >> sw;

  tmp = (x >> 4) & 0xF; // same method with 8bits
  sw  = ((!tmp) + (~0)) & 4;
  cnt = cnt + sw;
  x   = x >> sw;
  
  tmp = (x >> 2) & 0x3; // same method with 4bits
  sw  = ((!tmp) + (~0)) & 2;
  cnt = cnt + sw;
  x   = x >> sw;
  
  tmp = (x >> 1) & 0x1; // same method with 2bits
  sw  = ((!tmp) + (~0)) & 1;
  cnt = cnt + sw;
  return cnt;
}
/* 
 * absolute_of_x - Return abs(x)
 *   Example: absolute_of_x(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 10
 *   Points: 4
 */
int absolute_of_x(int x) {
/* if x is positive, sign = 0. else, sign = -1(0xFFFFFFFF)
 */
  int sign = x >> 31;
/* if x is positive, (0 + x) ^ 0 = x
   if x is negative, (x - 1) ^ 0xFFFFFFFFF = ~(x - 1) = -x
   Prove of ~(x - 1) = -x
   p) (x-1) + ~(x-1)     = 0b111...11
      (x-1) + ~(x-1) + 1 = 0
       x    + ~(x-1)     = 0
              ~(x-1)     = -x 
 */ 
  return (sign + x) ^ sign;
}
/* 
 * is_fits_in_bits - Can x be represented in 2's complement integer with n-bit?
 *   1 <= n <= 32
 *   Examples: is_fits_in_bits(5,3) = 0, is_fits_in_bits(-4,3) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 15
 *   Points: 2
 */
int is_fits_in_bits(int x, int n) {
/* if x is fits in n bits,
 * nth ~ 31st bits in x must be all same with sign bit(32nd bit)
 */
  return !((x >> (n + (~0))) ^ (x >> 31));  
}
/* hmb - How many bits are  required to represent x in 2's complement?
 *  Examples: hmb(12) = 5
 *            hmb(298) = 10
 *            hmb(-5) = 4
 *            hmb(0)  = 1
 *            hmb(-1) = 1
 *            hmb(0x80000000) = 32
 *  Operations allowed to use: ! ~ & ^ | + << >>
 *  Max # of operations: 90
 *  Points: 4
 */
int hmb(int x) {
/* find the bit 1 in most biggest position in x(if x is positive) or ~x(if x is negative) and plus 1(because of sign bit)
 * similar idea with floor_log_2 function
 */
  int tmp;
  int sign = x >> 31;
  int sw;
  int cnt = 0;
/* if x is positive ((x + 0) ^ 0) + 0 = x
 * if x is negative ((x + -1) ^ 0xFFFFFFFF) - 1 = - x - 1 = ~x
 * prove of -x-1 = ~x
 * p) x + ~x = -1
 *        ~x = -x-1
 */
  x = ((x + sign) ^ sign) + sign ; 
    
  tmp = (x >> 16) & ((0xFF << 8) | 0xFF);
  sw  = ((!tmp) + (~0)) & 16;
  cnt = cnt + sw;
  x   = x >> sw;
    
  tmp = (x >> 8) & 0xFF;
  sw  = ((!tmp) + (~0)) & 8;
  cnt = cnt + sw;
  x   = x >> sw;
    
  tmp = (x >> 4) & 0xF;
  sw  = ((!tmp) + (~0)) & 4;
  cnt = cnt + sw;
  x   = x >> sw;
    
  tmp = (x >> 2) & 0x3;
  sw  = ((!tmp) + (~0)) & 2;
  cnt = cnt + sw;
  x   = x >> sw;
    
  tmp = (x >> 1) & 0x1;
  sw  = ((!tmp) + (~0)) & 1;
  cnt = cnt + sw;
  x   = x >> sw;

  tmp = x & 0x1;
  sw = ((!tmp)+ (~0)) &1;
  cnt = cnt + sw;
  x   = x >> sw;

  return cnt + 1;
}

/*
 * is_tmin - If x is the min of 2's complement, returns 1.
 *           Otherwise, return 0.
 *   Operations allowed to use: ! ~ & ^ | +
 *   Max # of operations: 10
 *   Points: 1
 */
int is_tmin(int x) {
/* if x + x is 0 x woud be min of 2's complement(because of overflow) and 0
 * 0 is not min of 2's complement. Except that. 
 */
  return !(x + x) & (!!x);
}
/* 
 * bang - return !x, but you should not use ! in the function.
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Operations allowed to use: ~ & ^ | + << >>
 *   Max # of operations: 12
 *   Points: 4 
 */
int bang(int x) {
/* calculate -abs(x)
 * if x is not 0, -abs(x) is negative(sign bit == 1)
 * if x is 0, -abs(x) is 0(sign bit == 0)
 */ 
  int sign = ~(x >> 31);
  return (((sign + x) ^ sign) >> 31) + 1;
}
/* 
 * is_ge - Return 1 when x >= y, else return 0 
 *   Example: is_ge(4,5) = 0.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 24
 *   Points: 3
 */
int is_ge(int x, int y) {
/* sx = sign of x, sy = sign of y
 * if sign of x, y are same, compare it with x + (~y) and get sign of it   ((!(sx ^ sy)) & ((y + (~x)) >> 31))
 * if sign of x, y are different, result will be depend on sign of x y     (!(sx) & sy))
 * if x y is same 							   (!(x ^ y))
 */
  int sx = x >> 31;
  int sy = y >> 31;
  return ((!(sx ^ sy)) & ((y + (~x)) >> 31)) | ((!sx) & sy) | (!(x ^ y));
}
