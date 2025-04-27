#pragma once
#define PCL_LARGE

#include<cstdio>
#include<bitset>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<stdexcept>
using std::min;

#include"../Exception.hpp"

#define pcLG_ERR_OF "Large overflowed, length=%d, query %d"
#define pcLG_ERR_DIV0 "Large divided by zero"
#define pcLG_ERR_SYNTAX "Invalid convention from \"%s\" to Large"

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

/**
 * @brief Big int stored in bin.
 * @tparam Length Max length of the number in bin
 * @warning We did **not** use complement code when storing this.
 * Instead, an extra sign bool is used to represent the plus or minus,
 * so the bit arithmetics only works on unsigned part of the number.
 */
template<int Length>
class Large
{
protected:
	bool sign=false;
	std::bitset<Length> bin;
public:
/* ------Functions------- */
	/// @brief Get max size of `Large`
	size_t GetSize(){return Length;}
	/// @brief Get the lowest digit in bin. 
	bool GetLast(){return bin[0];}
public:
	/// @brief Convert to string (`char*`)
	void ToString(char* buff,int base=10) const
	{
		if(!Bool())
		{
			buff[0]='0',buff[1]='\0';
			return;
		}
		int now=0;
		Large n=this->Abs(),res;
		while(n>0)
		std::tie(n,res)=divideOp(n,base),
		buff[now++]=res.Convert<int>()+'0';
		if(sign)	buff[now++]='-';
		buff[now]='\0';
		strcpy(buff,strrev(buff));
	}
	/**
	 * @brief Convert to target number type.
	 * @tparam Tp The target type.
	 */
	template<typename Tp>
	Tp Convert() const
	{
		Tp ans=0;
		for(int i=0;i<min((int)sizeof(Tp)*8,Length);i++)
			if(bin[i])
				ans+=(1ull<<i);
		if(sign)	ans=-ans;
		return ans;
	}
	/// @brief Count `1` in bin
	int PopCount() const
	{
		int ans=0;
		for(int i=0;i<Length;i++)
			if(bin[i]==1)
				ans++;
		return ans;
	}
	/// @brief Get pos of lowest `1` in bin
	int GetLowest() const
	{
		for(int i=0;i<Length;i++)
			if(bin[i]==1)
				return i;
		return 0;
	}
	/// @brief Get pos highest `1` in bin
	int GetHighest() const
	{
		int cnt=0;
		for(int i=Length-1;i>=0;i--,cnt++)
			if(bin[i]==1)
				return i;
		return 0;
	}
	/**
	 * @brief Whether the count of `1` in bin is odd or even
	 * @return `true` when odd \
	 * @return `false` when even
	 */
	bool Parity() const {return PopCount()&1;}
	Large Abs() const
		{return sign?operator-():*this;}
	//SetVal
	/// @brief Set value to 0
	void SetVal(){bin=0;sign=false;return;}
	/// @brief Set value with bin
	void SetVal(bool a[])
	{
		bin=0;
		for(int i=0;i<Length;i++)
			bin[i]=a[i];
		return;
	}
	/// @brief Set value with bin
	void SetVal(std::bitset<Length> a)
	{
		if(a<0)	sign=true,bin=-a;
		else	sign=false,bin=a;
		return;
	}
	/// @brief Set value to number string
	void SetVal(const char* a,int base=10)
	{
		bin=0;int i=0;
		if(a[0]=='-')	i=1;
		for(;a[i];i++)
		{
			if(a[i]!=' '&&pcpri::ctoi(a[i])>=base)
				throw pc::Exception(pcLG_ERR_SYNTAX,a);
			*this=*this*base+pcpri::ctoi(a[i]);
		}
		if(a[0]=='-')	sign=true;
		return;
	}
	/// @brief Set value to number
	template<typename Tp>
	void SetVal(Tp a)
	{
		if(a<0)	a=-a;
		bin=a;
	}
	//Constructor
	~Large(){bin=0;}
	Large(){bin=0;}
	Large(const Large &a){bin=a.bin;sign=a.sign;}
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
	auto operator[](size_t a)
	{
		if(a>=Length||a<0)
			throw pc::Exception(pcLG_ERR_OF,Length,a);
		return bin[a];
	}
	//Operator!
	bool operator!() const
	{
		for(int i=0;i<Length;i++)
			if(bin[i])
				return 0;
		return 1;
	}
	//Operator&
	Large friend operator&(Large a,Large b)
	{
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
	bool Bool() const
	{
		for(int i=0;i<Length;i++)
			if(bin[i])
				return 1;
		return 0;
	}
	//Comparing Operators
protected:
	int friend compare(Large a,Large b)
	{
		if(!a.Bool()&&a.sign)	a.sign=false;
		if(!b.Bool()&&b.sign)	b.sign=false;
		if(!a.sign&&b.sign)	return 1;
		if(a.sign&&!b.sign)	return -1;
		for(int i=Length-1;i>=0;i--)
			if(a[i]==1&&b[i]==0)
				return a.sign?-1:1;
			else if(a[i]==0&&b[i]==1)
				return a.sign?1:-1;
		return 0;
	}
public:
	bool friend operator<(Large a,Large b)
		{return compare(a,b)<0;}
	bool friend operator>(Large a,Large b)
		{return compare(a,b)>0;}
	bool friend operator<=(Large a,Large b)
		{return compare(a,b)<=0;}
	bool friend operator>=(Large a,Large b)
		{return compare(a,b)>=0;}
	bool friend operator!=(Large a,Large b)
		{return compare(a,b)!=0;}
	bool friend operator==(Large a,Large b)
		{return compare(a,b)==0;}
	template<typename Tp>
	bool friend operator<(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)<0;
	}
	template<typename Tp>
	bool friend operator>(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)>0;
	}
	template<typename Tp>
	bool friend operator<=(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)<=0;
	}
	template<typename Tp>
	bool friend operator>=(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)>=0;
	}
	template<typename Tp>
	bool friend operator!=(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)!=0;
	}
	template<typename Tp>
	bool friend operator==(Large a,Tp c)
	{
		Large b(c);
		return compare(a,b)==0;
	}
	//Math Operators
	Large operator~() const
	{
		Large a=*this;
		for(int i=0;i<Length;i++)
			a[i]=!a[i];
		return a;
	}
	Large operator-() const
	{
		Large a=*this;
		a.sign=!sign;
		return a;
	}
	Large friend operator+(Large a,Large b)
	{
		if(b>a)	std::swap(a,b);
		if(b.sign)	return a-(-b);
		Large<Length> sum;Large<Length+1> carry;
		int h=std::max(a.GetHighest(),b.GetHighest());
		for(int i=0;i<=h||carry[i];i++)
			carry[i+1]=a[i]&b[i]|a[i]&carry[i]|b[i]&carry[i],
			sum[i]=a[i]^b[i]^carry[i];
		sum.sign=a.sign;
		return sum;
	}
	template<typename Tp>
	Large friend operator+(Large a,Tp b)
		{return a+Large(b);}
	Large friend operator-(Large a,Large b)
	{
		bool swaped=b>a;
		if(swaped)	std::swap(a,b);
		if(b.sign)	return swaped?-(a+(-b)):a+(-b);
		Large<Length> diff;Large<Length+1> carry;
		int h=std::max(a.GetHighest(),b.GetHighest());
		for(int i=0;i<=h||carry[i];i++)
			carry[i+1]=a[i]&b[i]&carry[i]|(!a[i])&(b[i]|carry[i]),
			diff[i]=a[i]^b[i]^carry[i];
		diff.sign=swaped;
		return diff;
	}
	template<typename Tp>
	Large friend operator-(Large a,Tp b)
		{return a-Large(b);}
	Large friend operator*(Large a,Large b)
	{
		int i=0;Large res;
		while(b.Bool())
		{
			if(b[0])	res=(res+(a<<i));
			b>>=1,i++;
		}
		res.sign=a.sign^b.sign;
		return res;
	}
	template<typename Tp>
	Large friend operator*(Large a,Tp c)
		{return a*Large(c);}
protected:
	std::tuple<Large,Large> friend divideOp(Large dividend,Large divisor)
	{
		bool neg=dividend.sign!=divisor.sign;
		dividend.sign=false,divisor.sign=false;
		if(divisor==0)
			throw pc::Exception(pcLG_ERR_DIV0);
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
		if(neg)	return std::make_tuple(-result,-dividend);
		else	return std::make_tuple(result,dividend);
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
	obj.ToString(a,10);
	os<<a;
	return os;
}
template<int Length>
std::istream& operator>>(std::istream& is, Large<Length>& obj)
{
	char input[Length/2];
	is>>input;
	obj.SetVal(input,10);
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