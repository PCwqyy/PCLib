#ifndef PCL_CONSCR
#define PCL_CONSCR

#include"PCLib/Console.hpp"
#include<vector>
using std::vector;

namespace pcpri
{
	bool isbt(short a,short l,short r)	{return l<=a&&a<=r;}
	template<typename Tp>
	Tp** NewRect(COORD size)
	{
		Tp** temp;
		temp=new Tp*[size.X];
		for(int i=0;i<size.X;i++)
			temp[i]=new Tp[size.y];
		return temp;
	}
	template<typename Tp>
	void FreeRect(Tp** tar,COORD size)
	{
		for(int i=0;i<size.X;i++)
			delete tar[i];
		delete tar;
		return;
	}
}

#define PCcs_MAX_ID_LEN 100
#define PCcs_MAX_TEXT_LEN 1000
#define PCcs_UDF_NODE "undefined"
#define PCcs_TAB 4

#define tPCML 0x01
#define tTEXTBOX 0x02
#define tBUTTON 0x03
#define tRECT 0x04

namespace ConScr
{
	struct Dot
	{
		int col=ConDefaultColor;
		char ch;
	};
	class Node
	{
		protected:
			int Type;
			short ChCount=0;
			Node* Father;
			vector<Node*> Children;
			Dot** Buffer=NULL;
			char Id[PCcs_MAX_ID_LEN];
			COORD Size,Pos;
			char Text[PCcs_MAX_TEXT_LEN];
			void PrintToFather()
			{
				if(Buffer==NULL)	return;
				for(auto i:Children)
					i->PrintToFather();
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						Father->Buffer[Pos.X+i][Pos.Y+j]=Buffer[i][j];
			}
			void RenderText()
			{
				for(int j=0;j<Size.Y;j++)
					for(int i=0;i<Size.X;i++)
						Buffer[i][j].ch=' ';
				int now=0,i=0,j=0;
				while(Text[now]!='\0'&&j<Size.Y)
				{
					if(Text[now]=='\n')
						{j++,i=0,now++;continue;}
					else if(Text[now]=='\t')
						do	Buffer[i][j].ch=' ',i++;
						while(i%PCcs_TAB!=0&&i<Size.X);
					else
						Buffer[i][j].ch=Text[now],i++;
					now++;
					if(i==Size.X)	j++,i=0;
				}
			}
		public:
			Node(int tp){strcpy(Id,PCcs_UDF_NODE);Type=tp;}
			short GetChildrenCount(){return ChCount;}
			int AddChild(Node* tar)
			{
				Children.push_back(tar);
				ChCount++;
				return 0;
			}
			void SetId(char* tar){strcpy(Id,tar);return;}
			void GetId(char* tar){strcpy(tar,Id);return;}
			void SetSize(COORD tar)
			{
				if(Buffer!=NULL)	pcpri::FreeRect(Buffer,Size);
				Size=tar;
				Buffer=pcpri::NewRect<Dot>(tar);
			}
			COORD GetSize(){return Size;}
			COORD GetPos(){return Pos;}
			void SetText(char* tar){strcpy(Text,tar);return;}
			void GetText(char* tar){strcpy(tar,Text);return;}
			bool Hover()
			{
				return pcpri::isbt(GetMousexy().X,Pos.X,Pos.X+Size.X)&&
					   pcpri::isbt(GetMousexy().Y,Pos.Y,Pos.Y+Size.Y);
			};

	};
};

#endif