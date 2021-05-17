#ifndef MULT_HW_H
#define MULT_HW_H

#include <ap_int.h>
#define uint8 ap_uint<8>
#define uint32 ap_uint<32>

#define lm 8
#define ln 8
#define lp 8

#define m 2<<(lm-1)
#define n 2<<(ln-1)
#define p 2<<(lp-1)




void mult_hw(uint8 A[n][m], uint8 B[m][p], uint32 AB[n][p]);

#endif
