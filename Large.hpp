#ifndef PCL_LARGE
#define PCL_LARGE

#define BIGGER 1
#define EQAL 0
#define SMALLER -1

#include<cstdio>
#include<cstring>

short max(short a,short b){return a>b?a:b;}

template<int maxlength>
class Large
{
		private:
		//NumSet
		short a[maxlength+2]={0};
		//Basic Funcs
		void clear()
		{
			for(int i=0;i<maxlength;i++)
				a[i]=0;
			return;
		}
		void carry()
		{
			for(int i=0;i<maxlength;i++)
				if(a[i]>=0)
					a[i+1]+=a[i]/10,a[i]=a[i]%10;
				else if(a[i]%10!=0)
					a[i+1]-=a[i]/10+1,a[i]=10+a[i]%10;
				else
					a[i+1]+=a[i]/10,a[i]=0;
			return;
		}
		short&operator[](short i){if(i<maxlength)   return a[i];}
		int length()
		{
			int len=maxlength;
			while(a[len]==0&&len>1) len--;
			if(len==1&&a[1]==0) return 1;
			return len+1;
		}
		Large copy(Large m)
		{
			for(int i=0;i<m.length();i++)
				a[i]=m[i];
			return m;
		}
		Large copy(char*m)
		{
			for(int i=strlen(m)-1,j=0;i>=0;i--,j++)
				a[j]=m[i]-'0';
		}
		template<typename tp>
		Large copy(tp m)
		{
			int p=0;
			while(m!=0) a[p++]=m%10,m/=10;  
		}
		//Not
		void numbernot()
		{
			Large ans;
			copyto(ans);
			if(ans[0]==0&&ans.length()==1) ans[0]=1;
			else ans.clear();
		}
		//compare
		int compare(Large m)
		{
			if(length()>m.length())
				return BIGGER;
			else if(length()>m.length())
				return SMALLER;
			for(int i=length();i>=0;i--)
				if(a[i]>m[i])	   return BIGGER;
				else if(a[i]<m[i])  return SMALLER;
			return EQAL;
		}
		//plus
		Large plus(Large m)
		{
			const int mL=max(length(),m.length());
			for(int i=0;i<mL;i++)
				m[i]+=a[i];
			m.carry();
			return m;
		}
		template<typename tp>
		Large plus(tp m)
		{
			Large ans;
			ans.copy(*this);
			ans[0]+=m;
			ans.carry();
			return ans;
		}
		void pluss(Large m)
		{
			
		}
		//minus
		Large minus(Large m)
		{
			const int mL=max(length(),m.length());
			for(int i=0;i<mL;i++)
				m[i]=a[i]-m[i];
			m.carry();
			return m;
		}
		template<typename tp>
		Large minus(tp m)
		{
			Large ans;
			ans.copy(*this);
			ans[0]-=m;
			ans.carry();
			return m;
		}
		//times
		Large times(Large m)
		{
			Large ans;
			const int l1=length(),l2=m.length();
			for(int i=0;i<l1;i++)
				for(int j=0;j<l2;j++)
					ans[i+j]+=a[i]*m[j];
			ans.carry();
			return ans;
		}
		template<typename tp>
		Large times(tp m)
		{
			Large ans;
			const int l=length();
			for(int i=0;i<l;i++)
				ans[i]+=a[i]*m;
			ans.carry();
			return ans;
		}
		//divied
		Large divied(Large m)
		{
			Large ans,diver;
			const int l=length()-m.length()-1;
			for(int i=l;i>=0;i--)
			{
				diver.copy(0);
		
			}
		}
		template<typename tp>
		Large divied(tp m)
		{
			Large ans;
			tp diver=0;
		}
		//Tool Funcs
		void scan()
		{
			clear();
			char m[maxlength];
			scanf("%s",m);
			for(int i=strlen(m)-1,j=0;i>=0;i--,j++)
				a[j]=m[i]-'0';
		}
		void print()
		{
			carry();
			for(int i=length()-1;i>=0;i--)
				printf("%d",a[i]);
		}
	public:
		//Constructures/Distructures
		Large(){}
		template<typename tp>Large(tp m){a[0]=m;carry();}
		~Large(){}
		//Operators
		short&	operator[](int i){if(i<maxlength) return a[i];}
		Large	operator= (Large m){return copy(m);}
		Large	operator! ()const{return numbernot();}
		Large	operator+ (Large m){return plus(m);}
		void	operator+=(Large m){pulss(m);}
		Large	operator- (Large m){return minus(m);}
		void	operator-=(Large m){minuss(m);}
		Large	operator* (Large m){return times(m);}
		void	operator*=(Large m){timess(m);}
		Large	operator/ (Large m){return divied(m);}
		void	operator/=(Large m){divieds(m);}
		bool	operator==(Large m)const{return compare(m)==EQAL;}
		bool	operator!=(Large m)const{return compare(m)!=EQAL;}
		bool	operator> (Large m)const{return compare(m)==BIGGER;}
		bool	operator>=(Large m)const{return compare(m)!=SMALLER;}
		bool	operator< (Large m)const{return compare(m)==SMALLER;}
		bool	operator<=(Large m)const{return compare(m)!=BIGGER;}
		template<typename tp>friend Large operator+(Large b,tp m){return b.plus(m);}
		template<typename tp>friend Large operator+(tp m,Large b){return b.plus(m);}
		template<typename tp>friend Large operator-(Large b,tp m){return b.minus(m);}
		template<typename tp>friend Large operator-(tp m,Large b){return b.minus(m);}
		template<typename tp>friend Large operator*(Large b,tp m){return b.times(m);}
		template<typename tp>friend Large operator*(tp m,Large b){return b.times(m);}
		//Functions
		int Length(){return length();}
		void Scan(){return scan();}
		void Print(){return print();}
};

#endif
