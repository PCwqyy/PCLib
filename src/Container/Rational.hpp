#pragma once
#define PCL_RATIONAL

#include<cstring>

namespace pcpri
{
	template<typename Tp>
	Tp gcd(Tp a,Tp b)
	{
		Tp c;
		while(a!=0)
			c=b%a,b=a,a=c;
		return b;
	}
	template<typename Tp>
	void swap(Tp&a,Tp&b)
	{
		const Tp c=a;
		a=b,b=c;
	}
}

#ifndef pcB_DOUBLEACC
#define pcB_DOUBLEACC 1e8
#endif

#ifndef ERR_DIV0
#define ERR_DIV0 "Divide 0 occurs"
#endif

/**
 * @brief Basic class for rational
 * @tparam Tp The type of numerator and denominator
 */
template<typename Tp>
class basicRational
{
private:
	Tp a=0,b=1;
	void deduct()
	{
		const Tp gcdx=pcpri::gcd(a,b);
		a/=gcdx,b/=gcdx;
		if(b<0) a=-a,b=-b;
		return;
	}
	void make(Tp m){a=m,b=1;}
	void make(double m)
	{
		a=m*pcB_DOUBLEACC;
		b=pcB_DOUBLEACC;
		deduct();
		return;
	}
	void make(basicRational m)
		{a=m.a,b=m.b;deduct();}
	void make(const char* m)
	{
		int now=0;
		a=0,b=1;
		bool neg=false;
		while(m[now]==' ')	now++;
		if(m[now]=='-')
			now++,neg=true;
		while(m[now]!='\0')
		{
			if(m[now]<'0'||m[now]>'9')
				break;
			a*=10,
			a+=m[now++]-'0';
		}
		if(neg)	a=-a;
		if(m[now++]!='/')	return;
		b=0;
		while(m[now]!='\0')
		{
			if(m[now]<'0'||m[now]>'9')
				break;
			b*=10,b+=m[now++]-'0';
		}
		if(b==0)	throw ERR_DIV0;
		return;
	}
public:
	/**
	 * @brief Convert to target number type
	 * @tparam Tpm The target type
	 */ 
	template<typename Tpm>
	Tpm To(){return 1.0*a/b;}
#ifdef _INC_STDIO
	/// @todo Large??? int??? how to do???
	/// @brief Convert to string
	void ToString(char* Dest)
	{
		deduct();
		sprintf(Dest,b==1?"%d":"%d/%d",a,b);
		return;
	}
#endif
	basicRational friend operator+ (basicRational m,basicRational n)
	{
		const Tp gcdx=pcpri::gcd(m.b,n.b);
		m.a=m.a*(n.b/gcdx)+n.a*(m.b/gcdx);
		m.b=m.b*n.b/gcdx;
		m.deduct();
		return m;
	}
	basicRational friend operator- (basicRational m,basicRational n)
	{
		const Tp gcdx=pcpri::gcd(m.b,n.b);
		m.a=m.a*(n.b/gcdx)-n.a*(m.b/gcdx);
		m.b=m.b*n.b/gcdx;
		m.deduct();
		return m;
	}
	basicRational friend operator* (basicRational m,basicRational n)
	{
		m.deduct(),n.deduct();
		pcpri::swap(m.a,n.a);
		m.deduct(),n.deduct();
		m.a*=n.a,m.b*=n.b;
		return m;
	}
	basicRational friend operator/ (basicRational m,basicRational n)
	{
		pcpri::swap(m.a,m.b);
		return m*n;
	}
	/// @brief Make from number 
	template<typename Tpm>
	basicRational(Tpm m){make(m);}
	/// @brief Make from numerator and denominator
	basicRational(Tp m,Tp n){a=m,b=n;}
	/// @brief Default to 0
	basicRational(){a=0,b=1;}
	template<typename Tpm>
	basicRational operator= (Tpm m)
		{make(m);return *this;}
};

/// @brief Rational. Numerator and denominator are stored with `int`
typedef basicRational<int> rational;
/// @brief Rational. Numerator and denominator are stored with `long long`
typedef basicRational<long long> long_rational;

#include"../Multinclude.hpp"