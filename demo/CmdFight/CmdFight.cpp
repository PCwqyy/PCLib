#include"../../src/Socket.hpp"
#include"../../src/Console.hpp"
#include"../../src/File.hpp"
#include<string>
#include<vector>
#include<regex>
#include<map>

using std::string;
using std::vector;
using std::regex;
using std::regex_search;
using std::map;

char MyName[100],IpInput[100],Msg[1000],InStr[1000];
vector<string> Names(100,""),MustNotMatches;
vector<regex> Rules[100];
map<string,int> NameMap;
int NowPlayer,Port;
int LimitRule=10,LimitPlayer=16;
bool Alive[50];
FileOp flProfile;

void SetProfile()
{
	flProfile.open("./profile.txt",OVERWRITE);
	ColorPrintf(0xB0,"Update Profile:\n");
	ColorPrintf(0x0B,"Input Ip\n");
	scanf("%s",IpInput);
	ColorPrintf(0x0B,"Input port\nServer use this port too\n");
	scanf("%d",&Port);
	ColorPrintf(0x0B,"Input Name\n");
	scanf("%s",MyName);
	flProfile.printf("%s\n%d\n%s",IpInput,Port,MyName);
	ColorPrintf(0x0B,"Updated!\n");
	return;
}
void ReadProfile()
{
	flProfile.open("./profile.txt",INSERTWRITE);
	if(flProfile.null())
	{
		SetProfile();
		return;
	}
	flProfile.scanf("%s %d %s",IpInput,&Port,MyName);
	return;
}
bool InvalidRule(const char* rule)
{
	try{regex a(rule);}
	catch(const std::regex_error& e)
		{return true;}
	int len=strlen(rule);
	if(len>10)	return true;
	int invCnt=0;
	for(int i=0;i<len;i++)
		if(rule[i]=='*'||rule[i]=='{'||rule[i]=='+')
			if(i>0&&rule[i-1]!='\\')
				invCnt++;
	if(invCnt>1)	return true;
	regex r(rule);
	for(auto i:MustNotMatches)
		if(regex_search(i.c_str(),r))
			return true;
	return false;
}
bool MatchRule(int id,const char* tar)
{
	for(auto i:Rules[id])
		if(regex_search(tar,i))
			return true;
	return false;
}
void ReadMustNotMatches()
{
	FileOp flRead("./Words.txt",READONLY);
	while(true)
	{
		flRead.getword(InStr);
		MustNotMatches.push_back(string(InStr));
		if(flRead.Eof())	break;
	}
	return;
}

ConCursor In,Out;
template<typename ...types>
void OnScreen(int colOut,int colIn,const char* format,types ...args)
{
	In.ClearScanPrint();
	Out.Printf(colOut,format,args...);
	Out.PrintfNoMove(colIn," %s",In.GetScan());
	In.Goto(-1,Out.Query().Y);
	return;
}
int ClientMain()
{
	ConTitleA("Client");
	ReadProfile();
	ClientSocket client;
	int res=client.Connect(IpInput,Port);
	if(res!=0)
	{
		ColorPrintf(0x0C,"Connect Failed %d\n",res);
		return 0;
	}
	client.Send("$Name %s",MyName);
	system("cls");
	ColorPosPrintf(0x06,0,0,"%s:%d\n",IpInput,Port);
	Out.Goto(0,1);
	In.Goto(1,2);
	In.StartScan(true,0x0B);
	while(true)
	{
		if(client.Receive(Msg)!=SOCKET_ERROR)
		{
			if(Msg[0]=='$')
			{
				sscanf(Msg+1,"%s",InStr);
				if(strcmp(InStr,"Server")==0)
					strcpy(InStr,Msg+8),
					OnScreen(0x07,0x0B,"Server>%s\n",InStr);
				else if(strcmp(InStr,"Error")==0)
					strcpy(InStr,Msg+7),
					OnScreen(0x0C,0x0B,"[Error]%s\n",InStr);
				else if(strcmp(InStr,"Blocked")==0)
					strcpy(InStr,Msg+9),
					OnScreen(0x0A,0x0B,"~Blocked "),
					OnScreen(0x0E,0x0B,"%s\n",InStr);
				else if(strcmp(InStr,"CMD")==0)
					strcpy(InStr,Msg+5),
					OnScreen(0x0C,0x0B,"~Attack "),
					OnScreen(0x0E,0x0B,"%s\n",InStr),
					system(InStr),
					Out.Goto(0,GetCursorxy().Y),
					In.Goto(-1,GetCursorxy().Y),
					OnScreen(0x0E,0x0B,"Command executed.\n");
				else if(strcmp(InStr,"Info")==0)
					strcpy(InStr,Msg+6),
					OnScreen(0x06,0x0B,"%s\n",InStr);
				else
					OnScreen(0x0C,0x0B,"Unknown - \"%s\"",Msg);
			}
			else
				OnScreen(0x07,0x0B,"%s\n",Msg);
		}
		if(KeyDown(VK_RETURN))
			if(strlen(In.GetScan())>0)
				strcpy(Msg,In.GetScan()),
				In.ClearScan(),
				client.Send(Msg),
				OnScreen(0x0B,0x0B,"<%s\n",Msg);
		Sleep(50);
	}
}
int ServerMain()
{
	ConTitleA("Server");
	ServerSocket server;
	ReadProfile();
	ReadMustNotMatches();
	NowPlayer=0;
	int AliveCount=40;
	NameMap[string("Server")]=-1;
	if(!server.BindPort(Port))
	{
		printf("Err\n");
		return errno;
	}
	system("cls");
	Out.Goto(0,0);
	Out.Printf(0x06,"Done!\nServer opened on port %d\n",Port);
	In.Goto(1,2);
	In.StartScan(true,0x0E);
	while(true)
	{
		AliveCount--;
		if(server.Listen()&&NowPlayer<LimitPlayer)
		{
			int id=server.Accept();
			if(id!=INVALID_SOCKET)
			{
				sprintf(InStr,"#%d",id);
				Names[id]=string(InStr);
				NameMap[Names[id]]=id;
				Alive[id]=true;
				NowPlayer++,AliveCount=40;
				OnScreen(0x06,0x0E,"Client #%d connected.\n",id);
				server.Send(id,"$Server You are #%d!",id);
				server.SendToAll("$Info #%d Joined",id);
			}
		}
		if(KeyDown(VK_RETURN)&&strlen(In.GetScan())!=0)
		{
			strcpy(Msg,In.GetScan());
			In.ClearScan();
			OnScreen(0x0E,0x0E,"<%s\n",Msg);
			for(int i=0;i<server.GetConnectedCnt();i++)
				if(Alive[i])
					server.Send(i,"$Server %s",Msg);
		}
		for(int i=0;i<server.GetConnectedCnt();i++)
			if(Alive[i]&&server.Receive(i,Msg)!=SOCKET_ERROR)
				if(Msg[0]=='$')
				{
					sscanf(Msg+1,"%s",InStr);
					if(strcmp(InStr,"Name")==0)
					{
						sscanf(Msg+6,"%s",InStr);
						if(NameMap.find(string(InStr))!=NameMap.end())
							server.Send(i,"$Error Invalid Name \"%s\"",InStr);
						else
						{
							OnScreen(0x08,0x0E,"[%s=>%s]\n",Names[i].c_str(),InStr);
							server.SendToAll("$Info %s renamed %s",Names[i].c_str(),InStr);
							Names[i]=string(InStr);
							NameMap[Names[i]]=i;
						}
					}
					else if(strcmp(InStr,"Rule")==0)
					{
						strcpy(InStr,Msg+6);
						if(Rules[i].size()>=LimitRule)
							server.Send(i,"$Error Too many rules");
						else if(InvalidRule(InStr))
							server.Send(i,"$Error Invalid rule");
						else
							Rules[i].push_back(regex(InStr));
					}
					else if(strcmp(InStr,"Attack")==0)
					{
						sscanf(Msg+8,"%s",InStr);
						if(strcmp(InStr,"Server")==0)
							server.Send(i,"$Error Hey! You can't attack server! X(");
						if(NameMap.find(string(InStr))==NameMap.end())
							server.Send(i,"$Error Player \"%s\" not found!",InStr);
						else
						{
							int tar=NameMap[string(InStr)];
							strcpy(InStr,Msg+9+strlen(InStr));
							server.Send(tar,"$%s %s",MatchRule(tar,InStr)?"Blocked":"CMD",InStr);
						}
					}
					else
						server.Send(i,"$Error Unknown - \"%s\"",Msg);
				}
				else
				{
					OnScreen(0x07,0x0E,"%s>%s\n",Names[i].c_str(),Msg);
					for(int j=0;j<server.GetConnectedCnt();j++)
						if(Alive[j]&&j!=i)
							server.Send(j,"%s>%s",Names[i].c_str(),Msg);
				}
		if(AliveCount==0)
		{
			AliveCount=40;
			for(int i=0;i<server.GetConnectedCnt();i++)
				if(Alive[i]&&!server.CheckAlive(i))
					OnScreen(0x06,0x0E,"%s disconnect.\n",Names[i].c_str()),
					server.SendToAll("$Info %s Loose",Names[i]),
					Alive[i]=false;
		}
		Sleep(50);
	}
	return 0;
}

int main()
{
	ConTitleA("Cmd Fight");
	ReadProfile();
	ColorPrintf(0x07,
		"PClib Cmd Fight\n"
		"PCwqyy(c) All right reserved.\n"
		"Type \"Help\" to get help.\n");
StartInput:
	ColorPrintf(0x0B,">");
	scanf("%s",InStr);
	if(strcmp(InStr,"Server")==0)
		ServerMain();
	else if(strcmp(InStr,"Client")==0)
		ClientMain();
	else if(strcmp(InStr,"Profile")==0)
		SetProfile();
	else if(strcmp(InStr,"Help")==0)
		printf(
			"Server\tStart a server on this computer\n"
			"Client\tJoin other's server\n"
			"Profile\tEdit your Profile\n"
			"Exit\tExit the program\n");
	else if(strcmp(InStr,"Exit")==0)
		return 0;
	goto StartInput;
	return 0;
}