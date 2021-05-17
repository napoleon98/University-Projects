#include "file.h"

#define uint8 ap_uint<8>
#define uint32 ap_uint<32>

const int nn=n;
const int pp=p;
const int mm=m;



void copy1(uint8 BA[n][m],uint8 A1[n][m] ){
	//BRAM---------

	for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min=nn max=nn
		for(int j = 0; j < m; j++){
		#pragma HLS loop_tripcount min=mm max=mm
		#pragma HLS UNROLL factor=2
		#pragma HLS PIPELINE II=1

			BA[i][j]=A1[i][j];

		}
	}//BRAM--------


}
void copy2(uint8 BB[m][p],uint8 B1[m][p]){
	//BRAM---------

	for(int i = 0; i < m; i++){
	#pragma HLS loop_tripcount min=mm max=mm
		for(int j = 0; j < p; j++){
		#pragma HLS loop_tripcount min=pp max=pp
		#pragma HLS UNROLL factor=2
		#pragma HLS PIPELINE II=1

			BB[i][j]=B1[i][j];}

	}//BRAM--------


}
void mult_hw(uint8 A[n][m], uint8 B[m][p], uint32 AB[n][p])
{


uint8 BRAM_A[n][m];
uint8 BRAM_B[m][p];

#pragma HLS ARRAY_PARTITION variable=BRAM_A complete dim=2
#pragma HLS ARRAY_PARTITION variable=BRAM_B complete dim=1
#pragma HLS DATAFLOW

copy1(BRAM_A,A);

copy2(BRAM_B,B);



for(int i = 0; i < n; i++){
#pragma HLS loop_tripcount min=nn max=nn

	for(int j = 0; j < p; j++){
#pragma HLS loop_tripcount min=pp max=pp
		int result = 0;
#pragma HLS UNROLL factor=2
#pragma HLS PIPELINE II=1
		for(int k = 0; k < m; k++){
#pragma HLS loop_tripcount min=mm max=mm
			result += BRAM_A[i][k]*BRAM_B[k][j];

		}
		AB[i][j] = result;
	}
}


}
