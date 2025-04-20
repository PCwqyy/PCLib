#pragma once
#define PCL_LARGE

#include<cstdio>
#include<bitset>
#include<cmath>
#include<algorithm>
using std::min;

#define pcLG_ERR_OF "Large overflowed!"
#define pcLG_ERR_DIV0 "Cannot div by 0!"
#define pcLG_ERR_INDEX "Not a Hex num!"

namespace pcpri
{
	int ctoi(char c)
	{
		if(isdigit(c))return c-'0';
		if(isupper(c))return c-'A'+10;
		if(islower(c))return c-'a'+10;
		return -1;
	}
}
template<int Length>
class Large{
	protected:
		std::bitset<Length> bin;
		char conr[Length/2];
		Large ModFC(int b,Large &Div)
		{
			Large divisor(b);
			Div=*this/divisor;
			Large result=Div*divisor;
			return *this-result;
		}
	public:
	/* ------Functions------- */
		size_t GetSize(){return Length;}
		bool GetLast(){return bin[0];}
		char* ToString()
		{
			Large Div;
			for(int i=0;i<Length/2;i++)
				conr[i]='\0';
			char* tmp=new char[Length/2];
			int now=0,i;
			while(Boolize())
			{
				tmp[now]=ModFC(10,Div).Convert<int>()+'0';
				*this=Div;now++;
			}
			now--;
			for(i=0;now>=0;i++,now--)
				conr[i]=tmp[now];
			for(;i<Length/2;i++)
				conr[i]='\0';
			return conr;
		}
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
			return Length;
		}
		/// @brief Get pos highest `1` in bin
		int GetHighest()
		{
			int cnt=0;
			for(int i=Length-1;i>=0;i--,cnt++)
				if(bin[i]==1)
					return i;
			return Length;
		}
		bool Parity(){return PopCount()&1;}
		//Init
		void Init(){bin=0;return;}
		void Init(bool a[])
		{
			bin=0;
			for(unsigned i=0;i<Length;i++)
				bin[i]=a[i];
			return;
		}
		template<typename Tp>
		void Init(Tp a)
		{
			bin=0;
			if(a<0)	a=-a;
			for(int i=0;i<min(32,Length);i++)
				bin[i]=a&(1<<i);
		}
		//Constuctruer
		~Large()
		{
			bin=0;
		}
		Large(){bin=0;}
		Large(const Large &a){bin=a.bin;}
		Large(bool a[]){Init(a);}
		template<typename Tp>
			Large(Tp a){Init(a);}

	/* ------Operators------- */
		//Operator=
		Large operator=(const char* str)
		{
			bin=0;
			for(unsigned i=0;str[i];i++)
				if(str[i]!=' '&&pcpri::ctoi(str[i])>=10){
					fprintf(stderr,pcLG_ERR_INDEX);
				}
			for(unsigned i=0;str[i];i++)
				if(str[i]!=' ')
					*this=*this*10+pcpri::ctoi(str[i]);
			return*this;
		}
		template<typename Tp>
		Large operator=(Tp a)
		{
			bin=0;
			if(a<0)	a=-a;
			for(int i=0;i<min(32,Length);i++)
				bin[i]=a&(1<<i);
			return *this;
		}
		//Operator[]
		template<typename Tp>
		bool operator[](Tp a)
		{
			if(a>=Length||a<0)
				fprintf(stderr,pcLG_ERR_OF);
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
			for(int i=0;i<Length;i++)c.bin[i]=a.bin[i]&b.bin[i];
			return c;
		}
		template<typename Tp>
		Large friend operator&(Large a,Tp d)
		{
			Large c,b(d);
			for(unsigned i=0;i<Length;i++)
			c.bin[i]=a.bin[i]&b.bin[i];
			return c;
		}
		//Operator^
		Large friend operator^(Large a,Large b)
		{
			Large c;
			for(unsigned i=0;i<Length;i++)
				c.bin[i]=a.bin[i]^b.bin[i];
			return c;
		}
		template<typename Tp>
		Large friend operator^(Large a,Tp d)
		{
			Large c,b(d);
			for(unsigned i=0;i<Length;i++)
				c.bin[i]=a.bin[i]^b.bin[i];
			return c;
		}
		//Operator|
		Large friend operator|(Large a,Large b)
		{
			Large c;
			for(unsigned i=0;i<Length;i++)
				c.bin[i]=a.bin[i]|b.bin[i];
			return c;
		}
		template <typename Tp>
		Large friend operator|(Large a,Tp d)
		{
			Large c,b(d);
			for(unsigned i=0;i<Length;i++)
				c.bin[i]=a.bin[i]|b.bin[i];
			return c;
		}
		//Operator<<
		Large friend operator<<(Large a,int len)
		{
			if(len>Length||len<=0)
				return a;
			for(int i=Length-1-len;i>=0;i--)
				a.bin[i+len]=a.bin[i];
			for(int i=0;i<len;i++)
				a.bin[i]=0;
			return a;
		}
		//Operator>>
		Large friend operator>>(Large a,int len)
		{
			if(len>Length||len<=0)
				return a;
			for(unsigned i=len;i<=Length-1;i++)
				a.bin[i-len]=a.bin[i];
			for(int i=Length-1;i>=Length-len;i--)
				a.bin[i]=0;
			return a;
		}
		bool Boolize(){return !!*this;}
		//Comparing Operators
		bool friend operator<(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 0;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 1;
			return 0;
		}
		bool friend operator>(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 1;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 0;
			return 0;
		}
		bool friend operator<=(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 0;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 1;
			return 1;
		}
		bool friend operator>=(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 1;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 0;
			return 1;
		}
		bool friend operator!=(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]!=b.bin[i])
					return 1;
			return 0;
		}
		bool friend operator==(Large a,Large b)
		{
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]!=b.bin[i])
					return 0;
			return 1;
		}
		template<typename Tp>
		bool friend operator<(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 0;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 1;
			return 0;
		}
		template<typename Tp>
		bool friend operator>(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 1;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 0;
			return 0;
		}
		template<typename Tp>
		bool friend operator<=(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 0;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 1;
			return 1;
		}
		template<typename Tp>
		bool friend operator>=(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]==1&&b.bin[i]==0)
					return 1;
				else if(a.bin[i]==0&&b.bin[i]==1)
					return 0;
			return 1;
		}
		template<typename Tp>
		bool friend operator!=(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]!=b.bin[i])
					return 1;
			return 0;
		}
		template<typename Tp>
		bool friend operator==(Large a,Tp c)
		{
			Large b(c);
			for(int i=Length-1;i>=0;i--)
				if(a.bin[i]!=b.bin[i])
					return 0;
			return 1;
		}
		//Math Operators
		Large friend operator+(Large a,Large b)
		{
			Large sum,carry;
			while(b.Boolize())
			{
				sum=a^b,carry=(a&b)<<1;
				a=sum,b=carry;
			}
			return a;
		}
		template<typename Tp>
		Large friend operator+(Large a,Tp c)
		{
			Large sum,carry,b(c);
			while(b.Boolize())
			{
				sum=a^b,carry=(a&b)<<1;
				a=sum,b=carry;
			}
			return a;
		}
		Large operator~()
		{
			Large a=*this;
			for(unsigned i=0;i<Length;i++)
				a.bin[i]=1-a.bin[i];
			return a;
		}
		Large friend operator-(Large x,Large y)
		{
			Large c(1);
			return((x+(~y))+c);
		}
		template<typename Tp>
		Large friend operator-(Large x,Tp q)
		{
			Large c(1),y(q);
			return((x+(~y))+c);
		}
		Large friend operator*(Large a,Large b)
		{
			int i=0;
			Large res;
			while(b.Boolize())
			{
				if(b.bin[0])
					res=(res+(a<<i));
				b>>=1,i++;
			}
			return res;
		}
		template<typename Tp>
		Large friend operator*(Large a,Tp c)
		{
			int i=0;
			Large res,b(c);
			while(b.Boolize())
			{
				if(b.bin[0])
					res=(res+(a<<i));
				b>>=1,i++;
			}
			return res;
		}
		Large friend operator/(Large dividend,Large divisor)
		{
			Large result(0);
			if(dividend==0)
				{return result;}
			if(divisor==0)
				fprintf(stderr,pcLG_ERR_DIV0);
			Large multiple(1),initDivisor(divisor);
			while(dividend>=divisor)
			{
				dividend=dividend-divisor;
				result=result+multiple;
				divisor<<=1,multiple<<=1;
				while(dividend<divisor)
					if(divisor>=initDivisor)
						divisor>>=1,multiple>>=1;
					else return result;
			}
			return result;
		}
		Large friend operator%(Large dividend,Large divisor)
		{
			Large result=dividend/divisor*divisor;
			return dividend-result;
		}
		template<typename Tp>
		Large friend operator/(Large dividend,Tp b)
		{
			Large result(0),divisor(b);
			if(dividend==0)
				return result;
			if(divisor==0)
				fprintf(stderr,pcLG_ERR_DIV0);
			Large multiple(1),initDivisor(divisor);
			while(dividend>=divisor)
			{
				dividend=dividend-divisor;
				result=result+multiple;
				divisor<<=1,multiple<<=1;
				while(dividend<divisor)
					if(divisor>=initDivisor)
						divisor>>=1,multiple>>=1;
					else return result;
			}
			return result;
		}
		template<typename Tp>
		Large friend operator%(Large dividend,Tp b)
		{
			Large divisor(b);
			Large result=dividend/divisor*divisor;
			return dividend-result;
		}
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