

//Including to use ap_uint<> datatype
#include <ap_int.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 64
#define DATAWIDTH 512
#define VECTOR_SIZE (DATAWIDTH / 32) // vector size is 16 (512/32 = 16)

#define lm 8
#define ln 8
#define lp 8

#define m (1<<lm)
#define n (1<<ln)
#define p (1<<lp)

typedef ap_uint<DATAWIDTH> uint512_dt;

//TRIPCOUNT identifier
const unsigned int c_chunk_sz = BUFFER_SIZE;
const unsigned int c_size     = VECTOR_SIZE;


   
const int nn=n;
const int pp=p;
const int mm=m;

extern "C"
{
    void wide_vadd(
        const uint512_dt *in1, // Read-Only Vector 1
        const uint512_dt *in2, // Read-Only Vector 2
        uint512_dt *out,       // Output Result
		int size
    )
    {
#pragma HLS INTERFACE m_axi port = in1 max_write_burst_length = 32 max_read_burst_length = 32 offset = slave bundle = gmem //isws theloun allagh
#pragma HLS INTERFACE m_axi port = in2 max_read_burst_length = 32 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = out max_write_burst_length = 32 max_read_burst_length = 32 offset = slave bundle = gmem2
#pragma HLS INTERFACE s_axilite port = in1 bundle = control
#pragma HLS INTERFACE s_axilite port = in2 bundle = control
#pragma HLS INTERFACE s_axilite port = out bundle = control
#pragma HLS INTERFACE s_axilite port = size bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

        uint512_dt v1_local[BUFFER_SIZE*4]; // Local memory to store vector1 -> praktika fernoume 4 grammes tou A
        uint512_dt v2_local[BUFFER_SIZE*4]; //
        uint512_dt result_local[BUFFER_SIZE]; // Local Memory to store result
	
		ap_int<32> A[16][256];
		ap_int<32> B[256][16];
        // Input vector size for integer vectors. However kernel is directly
        // accessing 512bit data (total 16 elements). So total number of read
        // from global memory is calculated here:
        int size_in16 = (size - 1) / VECTOR_SIZE + 1; //4096

        //Per iteration of this loop perform BUFFER_SIZE vector addition
        for (int i = 0; i < size_in16; i += BUFFER_SIZE*4) {
//#pragma HLS PIPELINE
#pragma HLS DATAFLOW
#pragma HLS stream variable = v1_local depth = 256 //v1_local is now fifo with depth 64
#pragma HLS stream variable = v2_local depth = 256

            //int chunk_size = BUFFER_SIZE; //64

            //boundary checks
            //if ((i + BUFFER_SIZE) > size_in16)
                //chunk_size = size_in16 - i;

        //burst read first vector from global memory to local memory
        v1_rd:
            for (int j = 0; j < 256; j++) {
#pragma HLS pipeline
#pragma HLS LOOP_TRIPCOUNT min = 1 max = 256
                v1_local[j] = in1[i + j];//phrame ta 64 prwta stoixeia toy A vector, dhl 4 grammes tou A array
                v2_local[j] = in2[i/256 + (j%16)*16];
            }
			int l=0;
			
			for(int k=0; k<256;k++){ // edw ftiaxnoume ton A 32rh pinaka
		
				for( int m=0;m<16;l++){
					A[k/16][(k%16)*16+m]=v1_local[k].range(32*m+31,32*m);
				}
			}	

			for(int k=0;k<256; k++){
				for(int m=0;m=16;m++){					
					B[k][m]=v2_local[k].range(32*m+31,32*m);
					}
				
				
				
			}

        //burst read second vector and perform vector addition
        v2_rd_add:
            for(int e = 0; e < 16; e++){
#pragma HLS loop_tripcount min=nn max=nn
				for(int j = 0; j < 16; j++){
#pragma HLS loop_tripcount min=pp max=pp

				int result = 0;
#pragma HLS UNROLL factor=2
#pragma HLS PIPELINE II=1
					for(int k = 0; k < 256; k++){
#pragma HLS loop_tripcount min=mm max=mm

						result+= A[e][k]*B[k][j];
					}
				out[(e+(i/16))*p+j] = result;

				}
			}
        }
    }
}
