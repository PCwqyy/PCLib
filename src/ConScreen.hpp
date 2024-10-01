#ifndef PCL_CONSCR
#define PCL_CONSCR

#include"Console.hpp"
#include<vector>
using std::vector;

#define PCcs_TAB 4
#define PCcs_MAX_ID_LEN 100
#define PCcs_MAX_TEXT_LEN 1000
#define PCcs_MAX_TYPE_LEN 15
#define PCcs_MAX_ATT_LEN 15
#define PCcs_MAX_ATTVAL_LEN 1000
#define PCcs_UDF_NODE "undefined"
#define PCcs_ERR_UNTYPED "The child node doesn't have a type!"

#define tNODE 0x00
#define tTEXTBOX 0x01
#define tBUTTON 0x02
#define tRECT 0x03

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
	void FreeRect(COORD size,Tp** tar)
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
		ColorPosPrintf(pos.X,pos.Y,A.col,"%c",A.ch);
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
			Dot** Rendered=NULL;
			char Id[PCcs_MAX_ID_LEN];
			COORD Size,Pos;
			int Color;
			void FatherReRender()
			{
				if(Father==NULL)	return;
				Father->Rerender();
				return;
			}
			void PrintToFather()
			{
				if(!NoRender&&ChCount!=0)
					for(auto i:Children)
						i->PrintToFather();
				if(Father==NULL)	return;
				if(Rendered==NULL)	return;
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						Father->Rendered[Pos.X+i][Pos.Y+j]=Rendered[i][j];
				NoRender=true;
			}
			void RenderFromText(char* Text)
			{
				FatherReRender();
				for(int j=0;j<Size.Y;j++)
					for(int i=0;i<Size.X;i++)
						Rendered[i][j].ch=' ';
				int now=0,i=0,j=0;
				while(Text[now]!='\0'&&j<Size.Y)
				{
					if(Text[now]=='\n')
						{j++,i=0,now++;continue;}
					else if(Text[now]=='\t')
						do	Rendered[i][j].ch=' ',i++;
						while(i%PCcs_TAB!=0&&i<Size.X);
					else
						Rendered[i][j].ch=Text[now],i++;
					now++;
					if(i==Size.X)	j++,i=0;
				}
			}
			void Init(int tp)
			{
				strcpy(Id,PCcs_UDF_NODE);
				NoRender=false;
				Type=tp;
				pcpri::NewRect(Size,Rendered);
				return;
			}
		public:
			Node(){Init(tNODE);}
			void Rerender(){NoRender=false;return;}
			void PrintTo(Dot** tar)
			{
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						tar[Pos.X+i][Pos.Y+j]=Rendered[i][j];
				return;
			}
			short GetChildrenCount(){return ChCount;}
			int AddChild(Node* tar)
			{
				FatherReRender();
				Children.push_back(tar);
				tar->Father=this;
				ChCount++;
				return 0;
			}
			void SetId(char* tar){strcpy(Id,tar);return;}
			void GetId(char* tar){strcpy(tar,Id);return;}
			void SetSize(COORD tar)
			{
				if(Rendered!=NULL)
					pcpri::FreeRect(Size,Rendered);
				pcpri::NewRect<Dot>(tar,Rendered);
				FatherReRender();
				Size=tar;
				return;
			}
			COORD GetSize(){return Size;}
			void SetPos(COORD tar)
			{
				FatherReRender();
				Pos=tar;
				return;
			}
			COORD GetPos(){return Pos;}
			void SetColor(int col)
			{
				Color=col;
				for(int j=0;j<Size.Y;j++)
					for(int i=0;i<Size.X;i++)
						Rendered[i][j].col=Color;
				FatherReRender();
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
	class ConScreen
	{
		protected:
			Dot** Screen,**Buffer;
			short ChCount=0;
			Node* Father=NULL;
			vector<Node*> Children;
			COORD Size;
			void Init()
			{
				pcpri::NewRect<Dot>(Size,Screen);
				pcpri::NewRect<Dot>(Size,Buffer);
				return;
			}
		public:
			ConScreen(){Init();}
			void SetSize(COORD tar)
			{
				if(Buffer!=NULL)
					pcpri::FreeRect(Size,Buffer);
				pcpri::NewRect<Dot>(tar,Buffer);
				if(Screen!=NULL)
					pcpri::FreeRect(Size,Screen);
				pcpri::NewRect<Dot>(tar,Screen);
				Size=tar;
				return;
			}
			void Flush()
			{
				if(!ChCount!=0)
					for(auto i:Children)
						i->PrintTo(Buffer);
				for(int i=0;i<Size.X;i++)
					for(int j=0;j<Size.Y;j++)
						if(Buffer[i][j]!=Screen[i][j])
							Screen[i][j]=Buffer[i][j],
							PrintDot(Buffer[i][j],(COORD){i,j});
				return;
			}
	};
	void ReadElement(char* Str,int& now)
	{
		char type[PCcs_MAX_ID_LEN];
		char att[PCcs_MAX_ATT_LEN];
		char val[PCcs_MAX_ATTVAL_LEN]
		while(true)
		{
			if(Str[now]=='\0')
				break;
			if(Str[now]=='#')
				while(Str[++now]=='\n');
			if(Str[now]=='<')
			{
				now++;
				sscanf(Str+now,"%s",type);
				while(Str[now]!='>')
				{
					while(Str[++now]==' ');
					bool f=false;
					int i=0,j=0;
					while(Str[now]!='=')
						att[i++]=Str[now++];
					att[i]='\0';
					while(Str[++now]!='\"');now++;
					while(Str[now]!='\"')
						val[j++]=Str[now++];
					val[j]='\0';
				}
			}
			now++;
		}
		return;
	}
	void Build(char* file)
	{
		FILE *Scr=fopen(file,"r+");
		vector<char> Buff;
		char in,*Res;
		while(true)
		{
			in=fgetc(Scr);
			if(feof(Scr))	break;
			Buff.push_back(in);
		}
		int sz=Buff.size();
		Res=new char[sz+5];
		for(int i=0;i<sz;i++)
			Res[i]=Buff[i];
		Res[sz]='\0';
		ReadElement(Res,0);
	}
};

#endif