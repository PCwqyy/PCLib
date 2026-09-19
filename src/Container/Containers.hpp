#pragma once
#define PCL_CONTINERS

/// @brief Do you know what are Disjoint Sets?
template<int maxlength>
class DisjointSets
{
	private:
		int Fa[maxlength+5];
	public:
		DisjointSets()
		{
			for(int i=0;i<maxlength;i++)
				Fa[i]=i;
		}
		DisjointSets(DisjointSets<maxlength>&m)
		{
			for(int i=0;i<maxlength;i++)
				Fa[i]=m.Fa[i];
		}
		~DisjointSets(){}
		int Find(int n)
		{
			if(n<0||n>=maxlength)
				return -1;
			if(Fa[n]!=n)
				Fa[n]=Find(Fa[n]);
			return Fa[n];
		}
		void Merge(int a,int b)
		{
			if(a<0||a>=maxlength||b<0||b>=maxlength)
				return;
			Fa[Find(b)]=Find(a);
			return;
		}
		bool Judge(int a,int b)
		{
			if(a<0||a>=maxlength||b<0||b>=maxlength)
				return false;
			return Find(a)==Find(b);
		}
		void operator=(DisjointSets m)
		{
			for(int i=0;i<maxlength;i++)
				Fa[i]=m.Fa[i];
		}
		int& operator[](int i){return Fa[i];}
};

#define EMPTY 0
/// @brief This Trie Tree is not just for chars
template<char schar,int length,typename endTp>
class TrieTree
{
	private:
		TrieTree* Fa=EMPTY;
		TrieTree* Ch[length]={EMPTY};
	public:
		endTp EndWord;
		TrieTree(){}
		~TrieTree(){}
		TrieTree* Pa(){return Fa;}
		TrieTree* Insert(char* s)
		{
			if(*s=='\0')
				{EndWord++;return this;}
			const int o=(*s)-schar;
			if(Ch[o]==EMPTY)
				Ch[o]=new TrieTree,
				Ch[o]->Fa=this;
			return Ch[o]->Insert(s+1);
		}
		TrieTree* Locate(char* s)
		{
			if(*s=='\0')	return this;
			const int o=(*s)-schar;
			if(Ch[o]==EMPTY)	return EMPTY;
			return Ch[o]->Locate(s+1);
		}
		TrieTree* InsLoc(char* s)
		{
			if(*s=='\0')	return this;
			const int o=(*s)-schar;
			if(Ch[o]==EMPTY)
				Ch[o]=new TrieTree,
				Ch[o]->Fa=this;
			return Ch[o]->Insert(s+1);
		}
};
/// @brief This Trie Tree is just for [a~z]
typedef TrieTree<'a',26,bool> Trie;

/// @brief Binary indexed trees which named IndexedTrees
template<typename Tp,int maxlength>
class IndexedTrees
{
	private:
		Tp data[maxlength+5];
		Tp sum[maxlength+5];
		int length=0;
		int lowbit(int a){return a&-a;}
	public:
		IndexedTrees(){}
		~IndexedTrees(){}
		Tp PerSum(int p)
		{
			Tp ans=0;p++;
			while(p)
				ans+=sum[p],
				p-=lowbit(p);
			return ans;
		}
		void Plus(int p,Tp a)
		{
			p++,data[p]+=a;
			while(p<=length)
				sum[p]+=a,
				p+=lowbit(p);
			return;
		}
		void Build(Tp* m,int l)
		{
			length=l;
			for(int i=1;i<=length;i++)
				data[i]=m[i];
			for(int i=1;i<=length;i++)
				Plus(i,data[i]);
		}
		void Build(int l)
		{
			length=l;
			for(int i=1;i<=length;i++)
				Plus(i,data[i]);
		}
		Tp operator()(int l,int r)
			{return PerSum(r+1)-PerSum(l);}
		Tp &operator[](int p){return data[p+1];}
		IndexedTrees(IndexedTrees& m)
		{
			for(int i=1;i<=m.length;i++)
				data[i]=m.data[i],sum[i]=m.sum[i];
			length=m.length;
		}
};

/// @brief Binary indexed trees, but this time 2D
template<typename Tp,int maxX,int maxY>
class IndexedRect
{
	private:
		int lowbit(int a){return a&-a;}
		Tp data[maxX+5][maxY+5];
		Tp sum[maxX+5][maxY+5];
		int lx,ly;
		Tp initvar;
	public:
		Tp PreSum(int px,int py)
		{
			Tp ans=initvar;
			for(int i=px+1;i>0;i-=lowbit(i))
				for(int j=py+1;j>0;j-=lowbit(j))
					ans+=sum[i][j];
			return ans;
		}
		void Plus(int px,int py,Tp a)
		{
			for(int i=px+1;i<=lx;i+=lowbit(i))
				for(int j=py+1;j<=ly;j+=lowbit(j))
					sum[i][j]+=a;
			return;
		}
		void Build(int x,int y,Tp InitVar)
		{
			lx=x,ly=y,initvar=InitVar;
			for(int i=1;i<=lx;i++)
				for(int j=1;j<=ly;j++)
					Plus(i,j,data[i][j]);
			return;
		}
		void Build(Tp** m,int x,int y,Tp InitVar)
		{
			for(int i=1;i<=lx;i++)
				for(int j=1;j<=ly;j++)
					data[i][j]=m[i-1][j-1];
			Build(x,y,InitVar);
			return;
		}
		Tp operator()(int x1,int y1,int x2,int y2)
			{return PreSum(x2,y2)-PreSum(x1-1,y2)
				-PreSum(x2,y1-1)+PreSum(x1-1,y1-1);}
		Tp* operator[](int x){return data[x];}
		IndexedRect(){}
		~IndexedRect(){}
		IndexedRect(IndexedRect& m)
		{
			for(int i=1;i<=lx;i++)
				for(int j=1;j<=ly;j++)
					data[i][j]=m.data[i][j],
					sum[i][j]=m.sum[i][j];
			lx=m.lx,ly=m.ly;
		}
};

template<typename Tp>
struct stsum
	{Tp operator()(Tp a,Tp b){return a+b;}};
template<typename Tp>
struct stsum_
	{Tp operator()(Tp a,Tp b){return a*b;}};
template<typename Tp>
struct stmax
	{Tp operator()(Tp a,Tp b){return a>b?a:b;}};
template<typename Tp>
struct stmax_
	{Tp operator()(Tp a,Tp b){return a+b;}};
template<typename Tp>
struct stmin
	{Tp operator()(Tp a,Tp b){return a<b?a:b;}};
template<typename Tp>
struct stmin_
	{Tp operator()(Tp a,Tp b){return a+b;}};


/// @brief Segment tree without lazy delete. \n
/// @brief The last template is operation struct with operator()
template< typename Tp,int maxlength,typename op=stsum<Tp> >
class SegmentTree
{
	private:
		struct point{int l,r;Tp data;};
		point tr[maxlength*4+5];
		op Op;
		Tp initvar;
		int lch(int p){return (p<<1)+1;}
		int rch(int p){return (p<<1)+2;}
		void build(int p,int l,int r,Tp *from)
		{
			tr[p].l=l,tr[p].r=r;
			if(l==r)	{tr[p].data=from[l];return;}
			build(lch(p),l,l+r>>1,from);
			build(rch(p),(l+r>>1)+1,r,from);
			tr[p].data=Op(tr[lch(p)].data,tr[rch(p)].data);
			return;
		}
		Tp locate(int p,int target)
		{
			if(tr[p].l==tr[p].r)	{return tr[p].data;}
			if(target<=(tr[p].l+tr[p].r)>>1)
				return locate(lch(p),target);
			else return locate(rch(p),target);
		}
		void edit(int p,int target,Tp var)
		{
			if(tr[p].l==tr[p].r)
				{tr[p].data=var;return;}
			if(target<=(tr[p].l+tr[p].r)>>1)
				edit(lch(p),target,var);
			else edit(rch(p),target,var);
			tr[p].data=Op(tr[lch(p)].data,tr[rch(p)].data);
			return;
		}
		Tp ask(int p,int l,int r)
		{
			if(l<=tr[p].l&&tr[p].r<=r)
				return tr[p].data;
			Tp var=initvar;
			if(l<=(tr[p].l+tr[p].r)>>1)
				var=Op(var,ask(lch(p),l,r));
			if(r>(tr[p].l+tr[p].r)>>1)
				var=Op(var,ask(rch(p),l,r));
			return var;
		}
	public:
		void BuildOn(Tp *from,int l){build(0,0,l-1,from);return;}
		void Edit(int tar,Tp var){edit(0,tar,var);return;}
		Tp operator[](int tar){return locate(0,tar);}
		Tp operator()(int l,int r){return ask(0,l,r);}
		Tp GetData(int p){return tr[p].data;}
		void SetInitVar(Tp InitVar){initvar=InitVar;}
};

/// @brief Segment tree with lazy delete. \n
/// @note To affrot the lazy delete, you should give the fast operation struct which is op_
template< typename Tp,int maxlength,typename op=stsum<Tp>,typename op_=stsum_<Tp> >
class LazySegmentTree
{
	private:
		struct point{int l,r;Tp data,lazy;};
		point tr[maxlength*4+5];
		op Op;op_ Op_;
		Tp initvar;
		int lch(int p){return (p<<1)+1;}
		int rch(int p){return (p<<1)+2;}
		void build(int p,int l,int r,Tp *from)
		{
			tr[p].l=l,tr[p].r=r;
			if(l==r)	{tr[p].data=from[l];return;}
			build(lch(p),l,l+r>>1,from);
			build(rch(p),(l+r>>1)+1,r,from);
			tr[p].data=Op(tr[lch(p)].data,tr[rch(p)].data);
			return;
		}
		void lazy(int p)
		{
			if(!tr[p].lazy)	return;
			tr[lch(p)].data=Op(tr[lch(p)].data,
				tr[p].lazy*(tr[lch(p)].r-tr[lch(p)].l+1));
			tr[rch(p)].data=Op(tr[rch(p)].data,
				tr[p].lazy*(tr[rch(p)].r-tr[rch(p)].l+1));
			tr[lch(p)].lazy+=tr[p].lazy;
			tr[rch(p)].lazy+=tr[p].lazy;
			tr[p].lazy=initvar;
			return;
		}
		Tp locate(int p,int target)
		{
			if(tr[p].l==tr[p].r)	{return tr[p].data;}
			if(target<=(tr[p].l+tr[p].r)>>1)
				return locate(lch(p),target);
			else return locate(rch(p),target);
		}
		void edit(int p,int target,Tp var)
		{
			if(tr[p].l==tr[p].r)
				{tr[p].data+=var;return;}
			if(target<=(tr[p].l+tr[p].r)>>1)
				edit(lch(p),target,var);
			else edit(rch(p),target,var);
			tr[p].data=Op(tr[lch(p)].data,tr[rch(p)].data);
			return;
		}
		void edit(int p,int l,int r,Tp var)
		{
			if(l<=tr[p].l&&tr[p].r<=r)
			{
				tr[p].data=Op(tr[p].data,Op_(tr[p].r-tr[p].l+1,var));
				tr[p].lazy+=var;
				return;
			}
			lazy(p);
			if(l<=(tr[p].l+tr[p].r)>>1)
				edit(lch(p),l,r,var);
			if(r>(tr[p].l+tr[p].r)>>1)
				edit(rch(p),l,r,var);
			tr[p].data=Op(tr[lch(p)].data,tr[rch(p)].data);
		}
		Tp ask(int p,int l,int r)
		{
			if(l<=tr[p].l&&tr[p].r<=r)
				return tr[p].data;
			lazy(p);
			Tp var=initvar;
			if(l<=(tr[p].l+tr[p].r)>>1)
				var=Op(var,ask(lch(p),l,r));
			if(r>(tr[p].l+tr[p].r)>>1)
				var=Op(var,ask(rch(p),l,r));
			return var;
		}
	public:
		void BuildOn(Tp *from,int l){build(0,0,l-1,from);return;}
		void Edit(int tar,Tp var){edit(0,tar,var);return;}
		void Edit(int l,int r,Tp var){edit(0,l,r,var);}
		Tp operator[](int tar){return locate(0,tar);}
		Tp operator()(int l,int r){return ask(0,l,r);}
		void SetInitVar(Tp InitVar){initvar=InitVar;}
};

/// @brief A tree-heap with memory recycle, called Treap
/// @note Remember to give the INF and -INF(_INF) of Tp in constructer!
template<typename Tp,int maxlength>
class Treap
{
	private:
		int newodd=0,root=0,size=0;
		Tp INF,_INF;
		struct point
		{
			int l,r,data,count,total;
			Tp sign;
		}tr[maxlength+5];
		Stack<int,maxlength> bin;
		unsigned int naxtrand=1;
		int rand()
		{
			naxtrand=naxtrand*1103515245+12345;
			return((unsigned int)(naxtrand/65536)%32768);
		}
		int create(Tp var)
		{
			int thisodd;
			if(!bin.Empty())
				thisodd=bin.Top(),
				bin.Pop();
			else thisodd=newodd++;
			tr[thisodd]={-1,-1,rand(),1,1,var};
			return thisodd;
		}
		void caltot(int p)
		{
			tr[p].total=0;
			if(tr[p].l!=-1)	tr[p].total+=tr[tr[p].l].total;
			if(tr[p].r!=-1)	tr[p].total+=tr[tr[p].r].total;
			tr[p].total+=tr[p].count;
			return;
		}
		void build()
		{
			create(_INF),create(INF);
			root=0,tr[0].r=1;
			caltot(0);
		}
		void lturn(int &p)
		{
			const int rt=tr[p].r;
			tr[p].r=tr[rt].l,tr[rt].l=p,p=rt;
			caltot(tr[p].l),caltot(p);
			return;
		}
		void rturn(int &p)
		{
			const int lt=tr[p].l;
			tr[p].l=tr[lt].r,tr[lt].r=p,p=lt;
			caltot(tr[p].r),caltot(p);
			return;
		}
		void insert(int &p,Tp var)
		{
			if(p==-1){p=create(var);return;}
			if(var==tr[p].sign){tr[p].count++,caltot(p);return;}
			if(var<tr[p].sign)
				insert(tr[p].l,var),
				tr[p].data<tr[tr[p].l].data?rturn(p):(void)0;
			else
				insert(tr[p].r,var),
				tr[p].data<tr[tr[p].r].data?lturn(p):(void)0;
			caltot(p);
			return;
		}
		void remove(int &p,Tp tar)
		{
			if(p==-1)	return;
			if(tar==tr[p].sign)
			{
				if(tr[p].count>1){tr[p].count--;caltot(p);}
				else if(tr[p].l!=-1||tr[p].r!=-1)
					if(tr[p].r==-1||(tr[p].l!=-1?tr[tr[p].l].data:0)
						>(tr[p].r!=-1?tr[tr[p].r].data:0))
						rturn(p),remove(tr[p].r,tar),caltot(p);
					else lturn(p),remove(tr[p].l,tar),caltot(p);
				else bin.Push(p),p=-1;
				return;
			}
			if(tr[p].sign<tar)	remove(tr[p].r,tar);
			else	remove(tr[p].l,tar);
			caltot(p);
			return;
		}
		Tp prev(Tp var)
		{
			int ans=0,p=root;
			while(p!=-1)
				if(tr[p].sign<var)
					ans=p,p=tr[p].r;
				else p=tr[p].l;
			return tr[ans].sign;
		}
		Tp next(Tp var)
		{
			int ans=1,p=root;
			while(p!=-1)
				if(tr[p].sign>var)
					ans=p,p=tr[p].l;
				else p=tr[p].r;
			return tr[ans].sign;
		}
		int rank(int p,Tp tar)
		{
			if(p==-1)	return -1;
			if(tr[p].sign==tar)	return (tr[p].l!=-1?tr[tr[p].l].total:0)+1;
			if(tr[p].sign>tar)	return rank(tr[p].l,tar);
			return rank(tr[p].r,tar)+
				(tr[p].l!=-1?tr[tr[p].l].total:0)+tr[p].count;
		}
		Tp tar(int p,int rank)
		{
			if(p==-1)	return _INF;
			if(tr[p].l!=-1&&tr[tr[p].l].total>=rank)
				return tar(tr[p].l,rank);
			if((tr[p].l!=-1?tr[tr[p].l].total:0)+tr[p].count>=rank)
				return tr[p].sign;
			return tar(tr[p].r,rank-
				(tr[p].l!=-1?tr[tr[p].l].total:0)-tr[p].count);	
		}
	public:
		Treap(Tp inf,Tp _inf){INF=inf,_INF=_inf;build();}
		void Insert(Tp var){insert(root,var);}
		void Delete(Tp var){remove(root,var);}
		Tp Prev(Tp var){return prev(var);}
		Tp Next(Tp var){return next(var);}
		int Rank(Tp var){return rank(root,var)-1;}
		Tp Var(int rank){return tar(root,rank+1);}
};

/// @brief A tree-heap without truning.
/// @note Memory recycle added
template<typename Tp,int maxlength>
class NoTurnTreap
{
	private:
		int root=-1,newodd=0;
		struct point{int l,r,data,total;Tp sign;}tr[maxlength+5];
		Stack<int,maxlength> bin;
		unsigned int naxtrand=1;
		int rand()
		{
			naxtrand=naxtrand*1103515245+12345;
			return((unsigned int)(naxtrand/65536)%32768);
		}
		int create(Tp var)
		{
			int thisodd;
			if(!bin.Empty())
				thisodd=bin.Top(),
				bin.Pop();
			else thisodd=newodd++;
			tr[thisodd]={-1,-1,rand(),1,var};
			return thisodd-1;
		}
		void caltot(int p)
		{
			tr[p].total=1;
			if(tr[p].l!=-1)	tr[p].total+=tr[tr[p].l].total;
			if(tr[p].r!=-1)	tr[p].total+=tr[tr[p].r].total;
		}
		int total(int p){return p!=-1?tr[p].total:0;}
		void splitvarL(int p,int &lp,int &rp,Tp var)
		{
			if(p==-1)	{lp=-1,rp=-1;return;}
			if(tr[p].sign>var)
				rp=p,splitvarL(tr[p].l,lp,tr[rp].l,var);
			else lp=p,splitvarL(tr[p].r,tr[lp].r,rp,var);
			caltot(p),caltot(lp),caltot(rp);
			return;
		}
		void splitvarR(int p,int &lp,int &rp,Tp var)
		{
			if(p==-1)	{lp=-1,rp=-1;return;}
			if(tr[p].sign<var)
				lp=p,splitvarR(tr[p].r,tr[lp].r,rp,var);
			else rp=p,splitvarR(tr[p].l,lp,tr[rp].l,var);
			caltot(p),caltot(lp),caltot(rp);
			return;
		}
		void splitrank(int p,int &lp,int &rp,int rank)
		{
			if(p==-1)	{lp=-1,rp=-1;return;}
			if(total(tr[p].l)+1<=rank)
				lp=p,rank-=total(tr[p].l)+1,
				splitrank(tr[p].r,tr[lp].r,rp,rank);
			else rp=p,splitrank(tr[p].l,lp,tr[rp].l,rank);
			caltot(p),caltot(lp),caltot(rp);
			return;
		}
		int merge(int lp,int rp)
		{
			if(lp==-1)	return rp;
			if(rp==-1)	return lp;
			int re;
			if(tr[lp].data>tr[rp].data)
				re=lp,tr[lp].r=merge(tr[lp].r,rp);
			else re=rp,tr[rp].l=merge(lp,tr[rp].l);
			caltot(re);
			return re;
		}
	public:
		void Insert(Tp var)
		{
			int t1=-1,t2=-1;
			splitvarL(root,t1,t2,var);
			root=merge(merge(t1,create(var)),t2);
			return;
		}
		void Delete(Tp var)
		{
			int t1=-1,t2=-1,tar=-1;
			splitvarL(root,t1,t2,var);
			splitrank(t1,t1,tar,total(t1)-1);
			bin.Push(tar);
			root=merge(t1,t2);
		}
		int Rank(int var)
		{
			int t1=-1,t2=-1;
			splitvarR(root,t1,t2,var);
			const int ans=total(t1)+1;
			root=merge(t1,t2);
			return ans;
		}
		Tp Var(int rank)
		{
			int t1=-1,t2=-1,tar=-1;
			splitrank(root,t1,t2,rank);
			splitrank(t1,t1,tar,rank-1);
			const Tp ans=tr[tar].sign;
			root=merge(merge(t1,tar),t2);
			return ans;
		}
		Tp Prev(Tp var)
		{
			int t1=-1,t2=-1,tar=-1;
			splitvarR(root,t1,t2,var);
			splitrank(t1,t1,tar,total(t1)-1);
			const Tp ans=tr[tar].sign;
			root=merge(merge(t1,tar),t2);
			return ans;
		}
		Tp Next(Tp var)
		{
			int t1=-1,t2=-1,tar=-1; 
			splitvarL(root,t1,t2,var);
			splitrank(t2,tar,t2,1);
			const Tp ans=tr[tar].sign;
			root=merge(t1,merge(tar,t2));
			return ans;
		}
};

