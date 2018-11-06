/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
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
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int mask = 255; // 0...011111111
  int shift = n << 3;

  return ((x & (mask << shift)) >> shift) & mask;

}
/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int logicalShift(int x, int n) {
  int mask = ~((1 << 31) >> n << 1);
  return (x >> n) & mask;
  // int mask = 1 << 31; // 100...0
  // int overflow = (~mask + n) & mask;  // 100...0 if n > 0, 0000...0 if n == 0
  //
  // return (x >> n) & ~(overflow >> n);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // https://stackoverflow.com/questions/3815165/how-to-implement-bitcount-using-only-bitwise-operators
  // 0x55555555 is not allowed, so do the other constant, need to construct them.
  // int c;
  // c = (x & 0x55555555) + ((x >> 1) & 0x55555555);
  // c = (c & 0x33333333) + ((c >> 2) & 0x33333333);
  // c = (c & 0x0F0F0F0F) + ((c >> 4) & 0x0F0F0F0F);
  // c = (c & 0x00FF00FF) + ((c >> 8) & 0x00FF00FF);
  // c = (c & 0x0000FFFF) + ((c >> 16) & 0x0000FFFF);
  // return c;

  int tmp, mask1, mask2, mask3, mask4, mask5;
  tmp = 0x55 | (0x55 << 8);
  mask1 = tmp | (tmp << 16); // 0x55555555

  tmp = 0x33 | (0x33 << 8);
  mask2 = tmp | (tmp << 16); // 0x33333333

  tmp = 0x0F | (0x0F << 8);
  mask3 = tmp | (tmp << 16); // 0x0F0F0F0F

  mask4 = 0xFF | (0xFF << 16); // 0x00FF00FF

  tmp = 0xFF | (0xFF << 8);
  mask5 = tmp | (0 << 16); // 0x0000FFFF

  int c = (x & mask1) + ((x >> 1) & mask1);
  c = (c & mask2) + ((c >> 2) & mask2);
  c = (c & mask3) + ((c >> 4) & mask3);
  c = (c & mask4) + ((c >> 8) & mask4);
  c = (c & mask5) + ((c >> 16) & mask5);
  return c;
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
  int overflow = x + ~(1 << 31);
  return (((x >> 31) & 1) | ((overflow >> 31) & 1)) ^ 1;
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int mask = x >> 31;
  int masked = x ^ mask;  // ~ negative x, positive x won't change

  return !(masked >> (n + (~1 + 1))); // can't fit if > 0
}
/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    int negOne = ~1 + 1;
    int mask = (1 << n) + negOne;
    int bias = (!!n) & ((x >> 31) & 1) & (!!(mask & x)); // n not 0 & x neg & x has 1 in range n
    return (x >> n) + bias;
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/*
 * isPositive - return 1 if x > 0, return 0 otherwise
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  int leadingZero = (x >> 31) ^ 1;  // 1 if most significant bit is 0
  return leadingZero & !!x;
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int leadingX = (x >> 31) & 1;
  int leadingY = (y >> 31) & 1;
  int xNegYPos = leadingX & 1 & (leadingY ^ 1); // x < y
  int xPosYNeg = leadingY & 1 & (leadingX ^ 1); // x > y

  int mask = ~(1 << 31);
  int maskX = x & mask;
  int maskY = y & mask;
  int compare = (~maskX + 1) + maskY;

  return xNegYPos | ((!xPosYNeg) & !((compare >> 31) & 1));
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  // change bits after MSB to 1
  x = x | x >> 1;
  x = x | x >> 2;
  x = x | x >> 4;
  x = x | x >> 8;
  x = x | x >> 16;

  // count 1
  int tmp, mask1, mask2, mask3, mask4, mask5;
  tmp = 0x55 | (0x55 << 8);
  mask1 = tmp | (tmp << 16); // 0x55555555

  tmp = 0x33 | (0x33 << 8);
  mask2 = tmp | (tmp << 16); // 0x33333333

  tmp = 0x0F | (0x0F << 8);
  mask3 = tmp | (tmp << 16); // 0x0F0F0F0F

  mask4 = 0xFF | (0xFF << 16); // 0x00FF00FF

  tmp = 0xFF | (0xFF << 8);
  mask5 = tmp | (0 << 16); // 0x0000FFFF

  int c;
  c = (x & mask1) + ((x >> 1) & mask1);
  c = (c & mask2) + ((c >> 2) & mask2);
  c = (c & mask3) + ((c >> 4) & mask3);
  c = (c & mask4) + ((c >> 8) & mask4);
  c = (c & mask5) + ((c >> 16) & mask5);

  return c + (~1 + 1);  // c - 1, do not include MSB
}
/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  if ((uf << 1) >> 24 == 0xFF && ((uf << 9 != 0)))  // notice uf is unsigned int, logical right shift
    return uf;
  else
    return (1 << 31) ^ uf;
}
/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  // https://yieldnull.com/blog/4b85a93fe9e4c80735233aa6dff922245f303698/#float_i2f
  return 2;
}
/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  int expMask = 0x000000FF << 23;
  int exp = expMask & uf;
  int fracMask = ~((1 << 31) + expMask);

  if (!exp) // denom
    return ((uf & fracMask) << 1) | (0x80000000 & uf);

  else if (!!~(exp + ~expMask))  // Normalized
    return uf + 0x00800000;

  return uf;  // special
}
