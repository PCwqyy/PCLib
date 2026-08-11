#pragma once
#define PCL_RATIONAL

#include<cstring>
#include<cctype>
#include"../Exception.hpp"

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

#ifndef pcM_DOUBLEACC
#define pcM_DOUBLEACC 8
#endif

#ifndef pcERR_DIV0
#define pcERR_DIV0 "Divide 0 occurs"
#endif

/**
 * @brief Basic class for rational
 * @tparam Tp The type of numerator and denominator
 */
template<typename Tp>
class Rational
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
		a=m*pcM_DOUBLEACC;
		b=pcM_DOUBLEACC;
		deduct();
		return;
	}
	void make(Rational m)
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
		if(b==0)	throw pc::Exception(pcXPT_DIVIDE_BY_ZERO,pcERR_DIV0);
		return;
	}
public:
	/**
	 * @brief Convert to target number type
	 * @tparam Tpm The target type
	 */ 
	template<typename Tpm>
	Tpm To(int acc=6)const
	{
		if(b==0)	throw pc::Exception(pcXPT_DIVIDE_BY_ZERO,pcERR_DIV0);
		if(std::is_integral<Tpm>::value)
			return a/b;
		else
			return 1.0*a/b;
	}
	Tp Numerator()const{return a;}
	Tp Denominator()const{return b;}
	Rational friend operator+ (Rational m,Rational n)
	{
		const Tp gcdx=pcpri::gcd(m.b,n.b);
		m.a=m.a*(n.b/gcdx)+n.a*(m.b/gcdx);
		m.b=m.b*n.b/gcdx;
		m.deduct();
		return m;
	}
	Rational friend operator- (Rational m,Rational n)
	{
		const Tp gcdx=pcpri::gcd(m.b,n.b);
		m.a=m.a*(n.b/gcdx)-n.a*(m.b/gcdx);
		m.b=m.b*n.b/gcdx;
		m.deduct();
		return m;
	}
	Rational friend operator* (Rational m,Rational n)
	{
		m.deduct(),n.deduct();
		pcpri::swap(m.a,n.a);
		m.deduct(),n.deduct();
		m.a*=n.a,m.b*=n.b;
		return m;
	}
	Rational friend operator/ (Rational m,Rational n)
	{
		pcpri::swap(m.a,m.b);
		return m*n;
	}
	opeartor bool(){return a!=0;}
	/// @brief Make from number 
	template<typename Tpm>
	Rational(Tpm m){make(m);}
	/// @brief Make from numerator and denominator
	Rational(Tp m,Tp n){a=m,b=n;}
	/// @brief Default to 0
	Rational(){a=0,b=1;}
	template<typename Tpm>
	Rational operator= (Tpm m)
		{make(m);return *this;}
};

#define pcERR_RTN_UNKTYPE "Invalid format specifier for Rational \"%c\""

#ifdef __cpp_lib_format
template<typename Tp>
struct std::formatter<Rational<Tp>,char>
{
	char type='r';
	int acc=pcM_DOUBLEACC;
	constexpr auto parse(std::format_parse_context& ctx)
	{
		auto it=ctx.begin();
		if(it==ctx.end()||*it=='}')
			return it;
		else if(*it=='r')
			return ++it;
		else if(*it=='.')
		{
			++it;acc=0;
			while(it!=ctx.end()&&isdigit(*it))
				acc=acc*10+(*it-'0'),++it;
			if(*it=='f')
			{
				type='f';
				return ++it;
			}
			return it;
		}
		else
			return ctx.end();
	}
	auto format(const Rational<Tp>& r,std::format_context& ctx) const
	{
		if(type=='f')
			return std::format_to(ctx.out(),"{0:.{1}f}",r.To<double>(),acc);
		else
			return std::format_to(ctx.out(),"{}/{}",r.Numerator(),r.Denominator());
	}
};
#endif

/// @brief Rational. Numerator and denominator are stored with `int`
typedef Rational<int> Fraction;
/// @brief Rational. Numerator and denominator are stored with `long long`
typedef Rational<long long> Long_Fraction;

#include"../Multinclude.hpp"