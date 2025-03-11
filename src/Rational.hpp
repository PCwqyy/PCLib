#pragma once
#define PCL_RATIONAL

#include<cstring>

template<typename Tp>
Tp gcd(Tp a,Tp b)
{
	Tp c;
	while(a!=0)
		c=b%a,b=a,a=c;
	return b;
}
#ifndef PCL_SORTTING
template<typename Tp>
void Swap(Tp&a,Tp&b)
{
	const Tp c=a;
	a=b,b=c;
}
#endif

#ifndef pcBR_DOUBLEACC
#define pcBR_DOUBELACC 1e5
#endif

#ifndef ERR_DIV0
#define ERR_DIV0
#endif

template<typename Tp>
class basicRational
{
	private:
		Tp a=0,b=1;
		char Dest[50];
		void reduct()
		{
			const Tp gcdx=gcd(a,b);
			a/=gcdx,b/=gcdx;
			if(b<0) a=-a,b=-b;
			return;
		}
	public:
		void Copy(Tp m,Tp n)
		{
			if(n==0)	throw ERR_DIV0;
			a=m,b=n;
			reduct();
		}
#ifdef _INC_STDIO
		char* ToString()
		{
			reduct();
			sprintf(Dest,b==1?"%d":"%d/%d",a,b);
			return Dest;
		}
		void Print()
		{
			printf("%s",ToString());
		}
#endif
		template<typename Tpm>
		Tpm To(){return 1.0*a/b;}

		basicRational friend operator+ (basicRational m,basicRational n)
		{
			const Tp gcdx=gcd(m.b,n.b);
			m.a=m.a*(n.b/gcdx)+n.a*(m.b/gcdx);
			m.b=m.b*n.b/gcdx;
			m.reduct();
			return m;
		}
		basicRational friend operator- (basicRational m,basicRational n)
		{
			const Tp gcdx=gcd(m.b,n.b);
			m.a=m.a*(n.b/gcdx)-n.a*(m.b/gcdx);
			m.b=m.b*n.b/gcdx;
			m.reduct();
			return m;
		}
		basicRational friend operator* (basicRational m,basicRational n)
		{
			m.reduct(),n.reduct();
			Swap(m.a,n.a);
			m.reduct(),n.reduct();
			m.a*=n.a,m.b*=n.b;
			return m;
		}
		basicRational friend operator/ (basicRational m,basicRational n)
		{
			Swap(m.a,m.b);
			return m*n;
		}
		basicRational operator= (Tp m)
			{a=m,b=1;return *this;}
		basicRational operator= (double m)
		{
			a=m*pcBR_DOUBELACC;
			b=pcBR_DOUBELACC;
			reduct();
			return *this;
		}
		basicRational operator= (basicRational m)
			{a=m.a,b=m.b;reduct();return *this;}
		
		basicRational(){a=0,b=1;}
		basicRational(Tp m){a=m,b=1;}
		basicRational(double m)
		{
			a=m*pcBR_DOUBELACC;
			b=pcBR_DOUBELACC;
			reduct();
		}
};

typedef basicRational<int> rational;
typedef basicRational<long long> longrational;

#ifdef PCL_IO
template<typename Tp>
int ssprintpc(char* Dest,basicRational<Tp> Th)
{
	strcpy(pcpri::temp,Th.ToString());
	int tlen=strlen(pcpri::temp);
	pcpri::temp[tlen++]=pcPF_SPACE;
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	return tlen;
}
#endif