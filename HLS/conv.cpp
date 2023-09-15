#include <ap_int.h>
#include <iostream>

void Conv(ap_uint<8> CHin,ap_uint<8> Hin,ap_uint<8> Win,ap_uint<8> CHout,
		  ap_uint<5> Kx,ap_uint<5> Ky,
		  float feature_in[],ap_uint<1> W[],float bias[],float feature_out[])
{
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_out
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=bias
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=W
	#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_in
	#pragma HLS INTERFACE mode=s_axilite port=Ky
	#pragma HLS INTERFACE mode=s_axilite port=Kx
	#pragma HLS INTERFACE mode=s_axilite port=CHout
	#pragma HLS INTERFACE mode=s_axilite port=Win
	#pragma HLS INTERFACE mode=s_axilite port=Hin
	#pragma HLS INTERFACE mode=s_axilite port=CHin
	#pragma HLS INTERFACE mode=s_axilite port=return

	ap_uint<8> Hout,Wout;
	Hout = Hin - Ky + 1;
	Wout = Win - Kx + 1;
	#pragma HLS PIPELINE II=1
	for(int cout=0;cout<CHout;cout++)
		for(int i=0;i<Hout;i++)
			for(int j=0;j<Wout;j++){
				float sum=0;
				for(int ii=0;ii<Ky;ii++)
					for(int jj=0;jj<Kx;jj++){
						ap_int<8> h=i+ii;
						ap_int<8> w=j+jj;
						if(h>=0 && w>=0 && h<Hin && w<Win)
							for(int cin=0;cin<CHin;cin++)
								sum+=(W[ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout])?
									 feature_in[h*CHin*Win+w*CHin+cin] : -feature_in[h*CHin*Win+w*CHin+cin];
					}
				sum+=bias[cout];
				feature_out[i*Wout*CHout+j*CHout+cout]=sum;
			}
}
