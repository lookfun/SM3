#include "SM3.h"
#include <string.h>

//#define FF(X,Y,Z,j) j<16? X^Y^Z:(X&Y)|(X&Z)|(Y&Z)
//#define GG(X,Y,Z,j) j<16? X^Y^Z:(X&Y)|(~X&Z)
#define FF0(X,Y,Z) (X^Y^Z)
#define FF1(X,Y,Z) ((X&Y)|(X&Z)|(Y&Z))
#define GG0(X,Y,Z) (X^Y^Z)
#define GG1(X,Y,Z) ((X&Y)|(~X&Z))
#define Shiftp(X,p) (((X)<<p)|((X)>>(32-p)))

const unsigned int Tj0=0x79cc4519;
const unsigned int Tj1=0x7a879d8a;
unsigned int P0(unsigned int X)
{
	return X^Shiftp(X,9)^Shiftp(X,17);
}
unsigned int P1(unsigned int X)
{
	return X^Shiftp(X,15)^Shiftp(X,23);
}
void genW(char *B,unsigned int *W,unsigned int *WW)
{
	for (int i=0;i<16;i++)
		W[i]=(0xff000000&(B[i*4]<<24))|(0x00ff0000&(B[i*4+1]<<16))|(0x0000ff00&(B[i*4+2]<<8))|(0xff&B[i*4+3]);
	for (int j=16;j<68;j++)
		W[j]=(P1(W[j-16]^W[j-9]^Shiftp(W[j-3],15)))^Shiftp(W[j-13],7)^W[j-6];
	for (int j=0;j<64;j++)
		WW[j]=W[j]^W[j+4];
}
void CF(unsigned int *Vin,char *b)
{
	unsigned int W[68];
	unsigned int WW[64];
	unsigned int A,B,C,D,E,F,G,H;
	A=Vin[0];	B=Vin[1];	C=Vin[2];	D=Vin[3];	E=Vin[4];	F=Vin[5];	G=Vin[6];	H=Vin[7];
	genW(b,W,WW);
	for (int i=0;i<16;i++)
	{
		unsigned int SS1,SS2,TT1,TT2;
		SS1=Shiftp((Shiftp(A,12)+E+Shiftp(Tj0,i)),7);
		SS2=SS1^Shiftp(A,12);
		TT1=FF0(A,B,C)+D+SS2+WW[i];
		TT2=GG0(E,F,G)+H+SS1+W[i];
		D=C;
		C=Shiftp(B,9);
		B=A;
		A=TT1;
		H=G;
		G=Shiftp(F,19);
		F=E;
		E=P0(TT2);
	}
	for (int i=16;i<64;i++)
	{
		unsigned int SS1,SS2,TT1,TT2;
		SS1=Shiftp((Shiftp(A,12)+E+Shiftp(Tj1,i)),7);
		SS2=SS1^Shiftp(A,12);
		TT1=FF1(A,B,C)+D+SS2+WW[i];
		TT2=GG1(E,F,G)+H+SS1+W[i];
		D=C;
		C=Shiftp(B,9);
		B=A;
		A=TT1;
		H=G;
		G=Shiftp(F,19);
		F=E;
		E=P0(TT2);
	}
	Vin[0]^=A;
	Vin[1]^=B;
	Vin[2]^=C;
	Vin[3]^=D;
	Vin[4]^=E;
	Vin[5]^=F;
	Vin[6]^=G;
	Vin[7]^=H;
}
int SM3coding(char *in,int *out,int len)
{
	if (in==0||out==0)
		return 0;
	//输入的字节数
	if ((len%64)!=0)
		return -1;
	int round=len/64;
	char *b=in;
	unsigned int V[8]={0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e};
	for (int i=0;i<round;i++,b+=64)
	{
		CF(V,b);
	}
	for (int i=0;i<8;i++)
		out[i]=V[i];

}
int SM3(char *in,char *out,int len)
{
	int intout[8];
	int resault=SM3coding(in,intout,len);
	for (int i=0;i<8;i++)
	{
		out[i*4+0]=(intout[i]>>24)&0xff;
		out[i*4+1]=(intout[i]>>16)&0xff;
		out[i*4+2]=(intout[i]>>8)&0xff;
		out[i*4+3]=(intout[i])&0xff;
	}
	return 1;
}
