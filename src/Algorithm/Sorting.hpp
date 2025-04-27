#pragma once
#define PCL_SORTING 

template<typename Tp>
void Swap(Tp&a,Tp&b)
{
	const Tp c=a;
	a=b,b=c;
	return;
}
template<typename Tp>
void Swap(Tp*a,Tp*b)
{
	const Tp c=*a;
	*a=*b,*b=c;
	return;
}
template<typename Tp>
void BucketSort(Tp*a,int size)
{
	int maxi=0;
	for(int i=0;i<size;i++)
		if(a[i]>a[maxi])
			maxi=i;
	int A[maxi+10]={0},G=0;
	for(int i=0;i<size;i++)
		A[a[i]]++;
	for(int i=0;i<maxi;i++)
		while(A[i]--)
			a[G++]=i;
	delete []A;
	return;
}
template<typename Tp>
void SelectionSort(Tp*a,int size)
{
	for(int i=0;i<size;i++)
	{
		int minj=i;
		for(int j=i;j<size;j++)
			if(a[j]<a[minj])
				minj=j;
		Swap(a[i],a[minj]);
	}
	return;
}
template<typename Tp>
void BubbleSort(Tp*a,int size)
{
	for(int i=0;i<size-1;i++)
	{
		bool finish=true;
		for(int j=0;j<size-1;j++)
			if(a[j]>a[j+1])
				Swap(a[j],a[j+1]),
				finish=false;
		if(finish)  break;
	}
	return;
}
template<typename Tp>
void InsertionSort(Tp*a,int size)
{
	for(int i=1;i<size;i++)
	{
		const Tp o=a[i];
		int j=i-1;
		while(j>=0&&a[j]>o)
			a[j+1]=a[j],j--;
		a[j+1]=o;
	}
	return;
}
template<typename Tp>
void QuickSort(Tp*a,int size)
{
	if(size<=1) return;
	size-=1;
	Tp*i=a,*j=a+size,base=*a;
	while(i<j)
	{
		while(*j>=base&&i<j)	j--;
		while(*i<=base&&i<j)	i++;
		if(i<j)				 Swap(i,j);
	}
	Swap(a,i);
	QuickSort(a,i-a);
	QuickSort(i+1,size-(i-a));
	return;
}
template<typename Tp>
void MergeSort(Tp*a,int size)
{
	if(size<=1) return;
	int msize=size/2;
	MergeSort(a,msize);
	MergeSort(a+msize,size-msize);
	Tp*i=a,*j=a+msize,*A=new Tp[size+2];
	int G=0;
	while(G<size)
		if(*i<=*j&&i<a+msize||j>=a+size)
			A[G++]=*i,i++;
		else
			A[G++]=*j,j++;
	for(int k=0;k<size;k++)
		*(a+k)=A[k];
	delete []A;
	return;
}
namespace pcpri
{
template<typename Tp>
void preheapsort(Tp*a,int s,int e)
{
	int f=s,c=s*2+1;
	while(c<=e)
	{
		if(c+1<=e&&a[c]<a[c+1]) c++;
		if(a[f]>=a[c])  return;
		Swap(a[f],a[c]);
		f=c,c=c*2+1;
	}
	return;
}
}//namespace
template<typename Tp>
void HeapSort(Tp*a,int size)
{
	for(int i=(size-2)/2;i>=0;i--)
		pcpri::preheapsort(a,i,size-1);
	for(int i=size-1;i>0;i--)
		Swap(a[0],a[i]),
		pcpri::preheapsort(a,0,i-1);
	return;
}
namespace pcpri
{
template<typename Tp>
void preinversions(Tp*a,int size,int&ans)
{
	if(size<=1) return;
	int msize=size/2;
	preinversions(a,msize,ans);
	preinversions(a+msize,size-msize,ans);
	Tp*i=a,*j=a+msize,*A=new Tp[size+2];
	int G=0;
	while(G<size)
		if(*i<=*j&&i<a+msize||j>=a+size)
			A[G++]=*i,i++;
		else
			A[G++]=*j,j++,ans+=msize-(i-a);
	for(int k=0;k<size;k++)
		*(a+k)=A[k];
	delete []A;
	return;
}
}//namespace
template<typename Tp>
int Inversions(Tp *a,int size)
{
	Tp*A=new Tp[size+2];
	int ans=0;
	for(int i=0;i<size;i++)
		A[i]=a[i];
	pcpri::preinversions(A,size,ans);
	delete []A;
	return ans;
}

#include"../Multinclude.hpp"