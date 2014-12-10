#include "SM3.h"
#include <string.h>

#define FF(X,Y,Z,j) j<16? X^Y^Z:(X&Y)|(X&Z)|(Y&Z)
#define GG(X,Y,Z,j) j<16? X^Y^Z:(X&Y)|(~X&Z)
#define Shiftp(X,p) ((X<<p)&(X>>(32-p)))

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
	{
		W[i]=B[i]<<24+B[i+1]<<16+B[i+2]<<8+B[i+3];
	}
	for (int j=16;j<68;j++)
		W[j]=P0(W[j-16]^W[j-9]^Shiftp(W[j-3],15))^Shiftp(W[j-13],7)^W[j-6];
	for (int j=0;j<64;j++)
		WW[j]=W[j]^W[j+4];
}
void CF(unsigned int Vin[],char *B,unsigned int &Vout)
{
	unsigned int W[68];
	unsigned int WW[64];
	genW(B,W,WW);

}
int SM3coding(char *in,char *out)
{
	if (in==0||out==0)
		return 0;
	int inlen=strlen(in);//输入的字节数
	if ((inlen%64)!=0)
		return -1;
	int round=inlen/64;
	char *b=in;
	unsigned int V[8]={0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e};
	for (int i=0;i<round;i++,b+=64)
	{
		CF(V,b,&V);
	}
}
