#pragma once
#define PCL_LARGE

#include<cstdio>
#include<bitset>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<stdexcept>
using std::min;

#define pcLG_ERR_OF "Large overflowed!"
#define pcLG_ERR_DIV0 "Cannot div by 0!"
#define pcLG_ERR_INDEX "Not a Hex num!"

namespace pcpri
{
	int ctoi(char c)
	{
		if(isdigit(c))	return c-'0';
		if(isupper(c))	return c-'A'+10;
		if(islower(c))	return c-'a'+10;
		return -1;
	}
}
template<int Length>
class Large
{
protected:
	std::bitset<Length> bin;
public:
/* ------Functions------- */
	/// @brief Get max size of `Large`
	size_t GetSize(){return Length;}
	/// @brief Get the lowest digit in bin. 
	bool GetLast(){return bin[0];}
public:
	/// @brief Convert to string (`char*`)
	void ToString(char* tmp,int base=10) const
	{
		int now=0;
		Large n=*this,res;
		while(n>0)
			std::tie(n,res)=divideOp(n,base),
			tmp[now++]=res.Convert<int>()+'0';
		tmp[now]='\0';
	}
	/**
	 * @brief Convert to target number type.
	 * @tparam Tp The target type.
	 */
	template<typename Tp>
	Tp Convert()
	{
		Tp ans=0;
		for(int i=0;i<min((int)sizeof(Tp)*8,Length);i++)
			if(bin[i])
				ans+=(1ull<<i);
		return ans;
	}
	/// @brief Count `1` in bin
	int PopCount()
	{
		int ans=0;
		for(int i=0;i<Length;i++)
			if(bin[i]==1)
				ans++;
		return ans;
	}
	/// @brief Get pos of lowest `1` in bin
	int GetLowest()
	{
		for(int i=0;i<Length;i++)
			if(bin[i]==1)
				return i;
		return 0;
	}
	/// @brief Get pos highest `1` in bin
	int GetHighest()
	{
		int cnt=0;
		for(int i=Length-1;i>=0;i--,cnt++)
			if(bin[i]==1)
				return i;
		return 0;
	}
	/**
	 * @brief Wheater the count of `1` in bin is odd or even
	 * @return `true` when odd \
	 * @return `false` when even
	 */
	bool Parity(){return PopCount()&1;}
	//SetVal

	/// @brief Set value to 0
	void SetVal(){bin=0;return;}
	/// @brief Set value with bin
	void SetVal(bool a[])
	{
		bin=0;
		for(int i=0;i<Length;i++)
			bin[i]=a[i];
		return;
	}
	/// @brief Set value with bin
	void SetVal(std::bitset<Length> a){bin=a;}
	/// @brief Set value to number string
	void SetVal(const char* a,int base=10)
	{
		bin=0;
		for(int i=0;a[i];i++)
		{
			if(a[i]!=' '&&pcpri::ctoi(a[i])>=base)
				throw std::runtime_error(pcLG_ERR_INDEX);
			*this=*this*base+pcpri::ctoi(a[i]);
		}
		return;
	}
	/// @brief Set value to number
	template<typename Tp>
	void SetVal(Tp a)
	{
		if(a<0)	a=-a;
		bin=a;
	}
	//Constuctruer
	~Large(){bin=0;}
	Large(){bin=0;}
	Large(const Large &a){bin=a.bin;}
	Large(bool a[]){SetVal(a);}
	Large(const char* a,int base=10){SetVal(a,base);}
	template<typename Tp>
		Large(Tp a){SetVal(a);}

/* ------Operators------- */
	//Operator=
	template<typename Tp>
	Large operator=(Tp a)
	{
		SetVal(a);
		return *this;
	}
	//Operator[]
	template<typename Tp>
	auto operator[](Tp a)
	{
		if(a>=Length||a<0)
			throw std::overflow_error(pcLG_ERR_OF);
		return bin[a];
	}
	//Operator!
	bool operator!()
	{
		for(int i=0;i<Length;i++)
			if(bin[i])
				return 0;
		return 1;
	}
	//Operator&
	Large friend operator&(Large a,Large b){
		Large c;
		for(int i=0;i<Length;i++)c[i]=a[i]&b[i];
		return c;
	}
	template<typename Tp>
	Large friend operator&(Large a,Tp d)
	{
		Large c,b(d);
		for(int i=0;i<Length;i++)
		c[i]=a[i]&b[i];
		return c;
	}
	//Operator^
	Large friend operator^(Large a,Large b)
	{
		Large c;
		for(int i=0;i<Length;i++)
			c[i]=a[i]^b[i];
		return c;
	}
	template<typename Tp>
	Large friend operator^(Large a,Tp d)
	{
		Large c,b(d);
		for(int i=0;i<Length;i++)
			c[i]=a[i]^b[i];
		return c;
	}
	//Operator|
	Large friend operator|(Large a,Large b)
	{
		Large c;
		for(int i=0;i<Length;i++)
			c[i]=a[i]|b[i];
		return c;
	}
	template <typename Tp>
	Large friend operator|(Large a,Tp d)
	{
		Large c,b(d);
		for(int i=0;i<Length;i++)
			c[i]=a[i]|b[i];
		return c;
	}
	//Operator<<
	Large friend operator<<(Large a,int len)
	{
		if(len>Length||len<=0)
			return a;
		for(int i=Length-1-len;i>=0;i--)
			a[i+len]=a[i];
		for(int i=0;i<len;i++)
			a[i]=0;
		return a;
	}
	//Operator>>
	Large friend operator>>(Large a,int len)
	{
		if(len>Length||len<=0)
			return a;
		for(unsigned i=len;i<=Length-1;i++)
			a[i-len]=a[i];
		for(int i=Length-1;i>=Length-len;i--)
			a[i]=0;
		return a;
	}
	bool Boolize()
	{
		for(int i=0;i<Length;i++)
			if(bin[i])
				return 1;
		return 0;
	}
	//Comparing Operators
	bool friend operator<(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 0;
			else if(a[i]==0&&b[i]==1)
				return 1;
		return 0;
	}
	bool friend operator>(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 1;
			else if(a[i]==0&&b[i]==1)
				return 0;
		return 0;
	}
	bool friend operator<=(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 0;
			else if(a[i]==0&&b[i]==1)
				return 1;
		return 1;
	}
	bool friend operator>=(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 1;
			else if(a[i]==0&&b[i]==1)
				return 0;
		return 1;
	}
	bool friend operator!=(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]!=b[i])
				return 1;
		return 0;
	}
	bool friend operator==(Large a,Large b)
	{
		for(int i=Length-1;i>=0;i--)
			if(a[i]!=b[i])
				return 0;
		return 1;
	}
	template<typename Tp>
	bool friend operator<(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 0;
			else if(a[i]==0&&b[i]==1)
				return 1;
		return 0;
	}
	template<typename Tp>
	bool friend operator>(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 1;
			else if(a[i]==0&&b[i]==1)
				return 0;
		return 0;
	}
	template<typename Tp>
	bool friend operator<=(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 0;
			else if(a[i]==0&&b[i]==1)
				return 1;
		return 1;
	}
	template<typename Tp>
	bool friend operator>=(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return 1;
			else if(a[i]==0&&b[i]==1)
				return 0;
		return 1;
	}
	template<typename Tp>
	bool friend operator!=(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]!=b[i])
				return 1;
		return 0;
	}
	template<typename Tp>
	bool friend operator==(Large a,Tp c)
	{
		Large b(c);
		for(int i=Length-1;i>=0;i--)
			if(a[i]!=b[i])
				return 0;
		return 1;
	}
	//Math Operators
	Large operator~()
	{
		Large a=*this;
		for(int i=0;i<Length;i++)
			a[i]=!a[i];
		return a;
	}
	Large friend operator+(Large a,Large b)
	{
		Large<Length> sum;Large<Length+1> carry;
		int h=std::max(a.GetHighest(),b.GetHighest());
		for(int i=0;i<=h||carry[i];i++)
			carry[i+1]=a[i]&b[i]|a[i]&carry[i]|b[i]&carry[i],
			sum[i]=a[i]^b[i]^carry[i];
		return sum;
	}
	template<typename Tp>
	Large friend operator+(Large a,Tp b)
		{return a+Large(b);}
	Large friend operator-(Large a,Large b)
	{
		Large<Length> diff;Large<Length+1> carry;
		int h=std::max(a.GetHighest(),b.GetHighest());
		for(int i=0;i<=h||carry[i];i++)
			carry[i+1]=a[i]&b[i]&carry[i]|(!a[i])&(b[i]|carry[i]),
			diff[i]=a[i]^b[i]^carry[i];
		return diff;
	}
	template<typename Tp>
	Large friend operator-(Large a,Tp b)
		{return a-Large(b);}
	Large friend operator*(Large a,Large b)
	{
		int i=0;
		Large res;
		while(b.Boolize())
		{
			if(b[0])
				res=(res+(a<<i));
			b>>=1,i++;
		}
		return res;
	}
	template<typename Tp>
	Large friend operator*(Large a,Tp c)
		{return a*Large(c);}
protected:
	std::tuple<Large,Large> friend divideOp(Large dividend,Large divisor)
	{
		if(divisor==0)
			throw std::runtime_error(pcLG_ERR_DIV0);
		if(dividend==0)
			return std::make_tuple(0,0);
		Large result=0;
		Large nowQ=1,nowD=divisor;
		while(dividend>=nowD)
			nowD<<=1,nowQ<<=1;
		while(dividend>=divisor)
		{
			if(dividend>=nowD)
				dividend-=nowD,
				result+=nowQ;
			nowD>>=1,nowQ>>=1;
		}
		return std::make_tuple(result,dividend);
	}
public:
	Large friend operator/(Large dividend,Large divisor)
		{return std::get<0>(divideOp(dividend,divisor));}
	Large friend operator%(Large dividend,Large divisor)
		{return std::get<1>(divideOp(dividend,divisor));}
	template<typename Tp>
	Large friend operator/(Large dividend,Tp b)
		{return dividend/Large(b);}
	template<typename Tp>
	Large friend operator%(Large dividend,Tp b)
		{return dividend%Large(b);}
	//Fast Operator Zone
	Large operator++()
		{*this=*this+1;return*this;}
	Large operator++(int)
		{Large a=*this;++*this;return a;}
	Large operator--()
		{*this=*this-1;return*this;}
	Large operator--(int)
		{Large a=*this;--*this;return a;}
	template<typename Tp>
	Large operator+=(Tp a)
		{*this=*this+a;return*this;}
	template<typename Tp>
	Large operator-=(Tp a)
		{*this=*this-a;return*this;}
	Large operator<<=(int a)
		{*this=*this<<a;return*this;}
	Large operator>>=(int a)
		{*this=*this>>a;return*this;}
	template <typename Tp>
	Large operator*=(Tp a)
		{*this=*this*a;return*this;}
	template <typename Tp>
	Large operator|=(Tp a)
		{*this=*this|a;return*this;}
	template <typename Tp>
	Large operator^=(Tp a)
		{*this=*this^a;return*this;}
	template <typename Tp>
	Large operator&=(Tp a)
		{*this=*this&a;return*this;}
	template<typename Tp>
	Large operator/=(Tp a){*this=*this/a;return *this;}
	template<typename Tp>
	Large operator%=(Tp a){*this=*this%a;return *this;}
};

#ifdef _GLIBCXX_IOSTREAM
template<int Length>
std::ostream& operator<<(std::ostream& os,const Large<Length>& obj)
{
	char a[Length]={0};
	obj.ToString(a);
	os<<strrev(a);
	return os;
}
template<int Length>
std::istream& operator>>(std::istream& is, Large<Length>& obj)
{
	char input[Length/2];
	is>>input;
	obj.SetVal(input);
	return is;
}
#endif

#ifdef PCL_IO
template<int Length>
int ssprintpc(char* Dest,Large<Length> Th)
{
	strcpy(pcpri::temp,Th.ToString());
	int tlen=strlen(pcpri::temp);
	pcpri::temp[tlen++]=pcPF_SPACE;
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	return tlen;
}
#endif

#include"../Multinclude.hpp"