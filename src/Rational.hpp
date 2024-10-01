#ifndef PCL_RATIONAL
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

template<typename Tp=int>
class rational
{
	private:
		Tp a=0,b=1;
		short doubleacc=5;
		template<typename Tpm>
			Tpm acc(int accuracy)
			{
				Tp ans=1;
				while(accuracy--)   ans*=10;
				return ans;
			}
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
			if(n==0)	n=1/0;
			a=m,b=n;
			reduct();
		}
#ifdef _INC_STDIO
		void Print()
		{
			reduct();
			printf("%d",a);
			if(b!=1)	printf("/%d",b);
		}
#endif
		template<typename Tpm>
			void Compute(Tpm&m){m=1.0*a/b;}

		rational operator+ (rational m)
		{
			const Tp gcdx=gcd(b,m.b);
			m.a=m.a*(b/gcdx)+a*(m.b/gcdx);
			m.b=m.b/gcdx*b;
			m.reduct();
			return m;
		}
		rational operator- (rational m)
		{
			const Tp gcdx=gcd(b,m.b);
			m.a=a*(m.b/gcdx)-m.a*(b/gcdx);
			m.b=m.b/gcdx*b;
			m.reduct();
			return m;
		}
		rational operator* (rational m)
		{
			reduct(),m.reduct();
			Swap(a,m.a);
			reduct(),m.reduct();
			m.a*=a,m.b*=b;
			m.reduct();
			return m;
		}
		rational operator/ (rational m)
		{
			Swap(m.a,m.b);
			return operator*(m);
		}
		void operator= (Tp m){a=m,b=1;}
		void operator= (double m){b=acc(doubleacc),a=m*b;reduct();}
		void operator= (rational m){a=m.a,b=m.b;reduct();}
};

#endif
