#ifndef PCL_CONSCR
#define PCL_CONSCR

#include"PCLib/Console.hpp"
#include<vector>
using std::vector;

#define PCcs_TAB 4
#define PCcs_MAX_ID_LEN 100
#define PCcs_MAX_TEXT_LEN 1000
#define PCcs_UDF_NODE "undefined"
#define PCcs_PCML_ID "pcml"

#define tNODE 0x00
#define tPCML 0x01
#define tTEXTBOX 0x02
#define tBUTTON 0x03
#define tRECT 0x04

namespace pcpri
{
	bool isbt(short a,short l,short r)
		{return l<=a&&a<=r;}
	template<typename Tp>
	void NewRect(COORD size,Tp** &Dest)
	{
		Dest=new Tp*[size.X];
		for(int i=0;i<size.X;i++)
			Dest[i]=new Tp[size.Y];
		return;
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

namespace ConScr
{
	struct Dot
	{
		int col=ConDefaultColor;
		char ch;
		bool friend operator!=(Dot A,Dot B)
			{return A.col!=B.col||A.ch!=B.ch;}
	};
	void PrintDot(Dot A,COORD pos)
	{
		ColorPrintf(pos.X,pos.Y,A.col,"%c",A.ch);
		return;
	}
	class Node
	{
		protected:
			int Type;
			short ChCount=0;
			Node* Father=NULL;
			vector<Node*> Children;
			bool NoRender;
			Dot** Buffer=NULL;
			char Id[PCcs_MAX_ID_LEN];
			COORD Size,Pos;
			int Color;
			void PrintToFather()
			{
				if(!NoRender&&ChCount!=0)
					for(auto i:Children)
						i->PrintToFather();
				if(Buffer==NULL)	return;
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						Father->Buffer[Pos.X+i][Pos.Y+j]=Buffer[i][j];
				NoRender=true;
			}
			void BuildFromStr()
			{
				return;
			}
			void Init(int tp)
			{
				strcpy(Id,PCcs_UDF_NODE);
				BuildFromStr();
				NoRender=false;
				pcpri::NewRect(Size,Buffer);
				return;
			}
		public:
			Node(){Init(tNODE);}
			void Rerender(){NoRender=false;return;}
			short GetChildrenCount(){return ChCount;}
			int AddChild(Node* tar)
			{
				if(Father!=NULL)
					Father->Rerender();
				Children.push_back(tar);
				tar->Father=this;
				ChCount++;
				return 0;
			}
			void SetId(char* tar){strcpy(Id,tar);return;}
			void GetId(char* tar){strcpy(tar,Id);return;}
			void SetSize(COORD tar)
			{
				if(Buffer!=NULL)
					pcpri::FreeRect(Buffer,Size);
				pcpri::NewRect<Dot>(tar,Buffer);
				Father->Rerender();
				Size=tar;
				return;
			}
			COORD GetSize(){return Size;}
			void SetPos(COORD tar)
			{
				if(Father!=NULL)
					Father->Rerender();
				Pos=tar;
				return;
			}
			COORD GetPos(){return Pos;}
			void SetColor(int col)
			{
				Color=col;
				for(int j=0;j<Size.Y;j++)
					for(int i=0;i<Size.X;i++)
						Buffer[i][j].col=Color;
				if(Father!=NULL)
					Father->Rerender();
				return;
			}
			int GetColor(){return Color;}
			bool Hover()
			{
				return pcpri::isbt(GetMousexy().X,Pos.X,Pos.X+Size.X)&&
					   pcpri::isbt(GetMousexy().Y,Pos.Y,Pos.Y+Size.Y);
			};
			bool Clicked(){return Hover()&&MouseLClick();}
	};
	class PCML:public Node
	{
		protected:
			Dot** Screen;
			void Init(int tp)
			{
				Node::Init(tp);
				pcpri::NewRect<Dot>(Size,Screen);
				strcpy(Id,PCcs_PCML_ID);
				return;
			}
		public:
			PCML(){Init(tPCML);}
			void Flush()
			{
				if(NoRender)	return;
				PrintToFather();
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						if(Buffer[i][j]!=Screen[i][j])
							Screen[i][j]=Buffer[i][j],
							PrintDot(Buffer[i][j],(COORD){i,j});
				return;
			}
	};
	class TextBox:Node
	{
		protected:
			char Text[PCcs_MAX_TEXT_LEN];
			void RenderText()
			{
				if(Father!=NULL)
					Father->Rerender();
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
			void Init(int tp)
			{
				Node::Init(tp);
				RenderText();
			}
		public:
			TextBox(){TextBox::Init(tTEXTBOX);}
			void SetText(char* tar)
			{
				strcpy(Text,tar);
				Father->Rerender();
				return;
			}
			void GetText(char* tar){strcpy(tar,Text);return;}
			void Debug()
			{
				Init(tTEXTBOX);
				SetText("Ha\tHa\nHa");
				SetPos((COORD){1,1});
				SetSize((COORD){7,2});
				SetColor(0x70);
			}		
	};
};

#endif