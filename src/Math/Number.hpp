/// @warning Unfinished! Do not use!!

#pragma once
#define PCL_NUMBER

#define _USE_MATH_DEFINES

#include<cmath>
#include<vector>
#include"Fraction.hpp"
#include"../Container/String.hpp"
#include"../Exception.hpp"

#define pcOP_OUTTYPE_FUNCTION 0
#define pcOP_OUTTYPE_ALPHABET 1
struct Operation
{
	String expr;const int outType;
	double (*func)(double);
	Operation(String e,double(*f)(double),int outType=pcOP_OUTTYPE_FUNCTION)
		:expr(e),func(f),outType(outType){}
	inline String toString(const String& arg) const
	{
		if(outType==pcOP_OUTTYPE_FUNCTION)
			return expr+'('+arg+')';
		else if(outType==pcOP_OUTTYPE_ALPHABET)
			return arg+expr;
	}
};
namespace pcmath_op
{
	const Operation Sqrt(u"√",sqrt);
	const Operation Sin("sin",sin);
	const Operation Cos("cos",cos);
	const Operation Tan("tan",tan);
	const Operation PI(u"π",[](double a){return M_PI*a;},pcOP_OUTTYPE_ALPHABET);
}

class Number
{
private:
	struct child
	{
		Fraction coef;
		Operation* op;
		Number* value;
		String toString()
		{
			String frac=std::format("{}",coef);
			if(op==nullptr)
				return frac;
			else
				return frac+op->toString(value->toString(true));
		}
	};
	std::vector<child> children;
public:
	String toString(bool recursive=false)
	{
		if(children.size()==0)
			return String("0");
		else if(children.size()==1&&children[0].op==nullptr)
			return std::format("{}",children[0].coef);
		else
		{
			String result;int size=children.size();
			for(int i=0;i<size;i++)
			{
				result+=children[i].toString();
				if(i!=size-1)	result+='+';
			}
			if(recursive)
				return "("+result+")";
			else
				return result;
		}
	}
	void addTerm(Fraction coef,const Operation* op=nullptr,Number* value=nullptr)
	{
		child c;c.coef=coef,c.op=const_cast<Operation*>(op),c.value=value;
		children.push_back(c);
	}
};