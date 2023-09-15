#include <ap_int.h>

void Bn(ap_uint<8> CHin, ap_uint<8> Hin, ap_uint<8> Win, 
		float feature_in[], float W[], float bias[], float feature_out[])
{
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_out
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=bias
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=W
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_in
	#pragma HLS INTERFACE mode=s_axilite port=Win
	#pragma HLS INTERFACE mode=s_axilite port=Hin
	#pragma HLS INTERFACE mode=s_axilite port=CHin
	#pragma HLS INTERFACE mode=s_axilite port=return
	#pragma HLS PIPELINE II=1
	for(int i = 0; i < CHin; i++)
		for(int j = 0; j < Win; j++)
			for(int k = 0; k < Hin; k++)
				feature_out[j*Hin*CHin+k*CHin+i] = feature_in[j*Hin*CHin+k*CHin+i]*W[i]+bias[i];
}
