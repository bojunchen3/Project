#include <ap_int.h>
#include <iostream>

void Fc(ap_uint<8> CHin,ap_uint<8> Hin,ap_uint<8> Win,ap_uint<8> CHout,
		  float feature_in[],ap_uint<1> W[],float bias[],float feature_out[])
{
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_out
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=bias
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=W
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_in
	#pragma HLS INTERFACE mode=s_axilite port=CHout
	#pragma HLS INTERFACE mode=s_axilite port=Win
	#pragma HLS INTERFACE mode=s_axilite port=Hin
	#pragma HLS INTERFACE mode=s_axilite port=CHin
	#pragma HLS INTERFACE mode=s_axilite port=return

	#pragma HLS PIPELINE II=1
	for(int cout=0;cout<CHout;cout++)
		for(int i=0;i<Hin;i++)
			for(int j=0;j<Win;j++){
				float sum=0;
				if(i>=0 && j>=0 && i<Hin && j<Win)
					for(int cin=0;cin<CHin;cin++)
						sum+=(W[cin*CHout+cout])?
							 feature_in[i*CHin*Win+j*CHin+cin] : -feature_in[i*CHin*Win+j*CHin+cin];
				feature_out[i*Win*CHout+j*CHout+cout]=sum+bias[cout];
			}
}
