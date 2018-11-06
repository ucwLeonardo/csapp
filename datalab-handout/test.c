#include <stdio.h>

unsigned float_neg(unsigned uf) {
  int mask = 0x000000FF << 23;
  int exp = mask & uf;
  int denom = !~((exp >> 23) + ~0x000000FF);
  int frac = ~((1 << 31) + mask) & uf;
  printf("%x\n", frac);
  printf("%i  %i\n", denom, !!frac);
  if (denom & !!frac)
    return uf;
  else
    return (1 << 31) ^ uf;
}

unsigned float_twice(unsigned uf) {
  int expMask = 0x000000FF << 23;
  int exp = expMask & uf;
  int fracMask = ~((1 << 31) + expMask);

  if (!exp) // denom
    return ((uf & fracMask) << 1) | (0x80000000 & uf);

  else if (!!~(exp + ~expMask))  // Normalized
    return uf + 0x00800000;

  return uf;  // special
  // int frac = fracMask & uf;
  // if (denom & !frac | !(uf & ~(1 << 31))) // infinity or +-0
  //   return uf;
  // else if (denom & !!frac) {  // NaN
  //   int sign = 0x80000000 & uf;
  //   return (uf << 1) | sign;
  // }
  // // else if (!exp)  // exp is 0
  // //   printf("%x\n", uf);
  // //   return ((frac << 1) & fracMask) + (uf & ~fracMask);
  // int expAddOne = 1 << 23;
  // int newExp = exp + expAddOne;
  // // printf("%x\n", newExp & mask);
  // // printf("%x\n", uf & ~mask);
  // return (uf & ~mask) + (newExp & mask);
}

int ilog2(int x) {
  // change bits after MSB to 1
  x = x | x >> 1;
  x = x | x >> 2;
  x = x | x >> 4;
  x = x | x >> 8;
  x = x | x >> 16;
  // count 1
  int c;
  c = (x & 0x55555555) + ((x >> 1) & 0x55555555);
  c = (c & 0x33333333) + ((c >> 2) & 0x33333333);
  c = (c & 0x0F0F0F0F) + ((c >> 4) & 0x0F0F0F0F);
  c = (c & 0x00FF00FF) + ((c >> 8) & 0x00FF00FF);
  c = (c & 0x0000FFFF) + ((c >> 16) & 0x0000FFFF);
  return c + (~1 + 1);
}

int main() {
  // printf("%x\n", float_twice(0x7f800000));
  // printf("%x\n", float_twice(0));
  // printf("%x\n", float_twice(0x80800000));
  // printf("%x\n", float_twice(1));
  // printf("%x\n", float_twice(0x80000001));
  // printf("%x\n", float_twice(0x7fffff));
  printf("%x\n", ilog2(16));
  return 0;
}
