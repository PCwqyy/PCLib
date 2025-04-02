#include"../../src/Command.hpp"
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

char MyName[100],IpInput[100],Msg[1000],InStr[1000],CmdToExe[1000];
vector<string> DisplayNames(100,""),MustNotMatches,RuleBackUps[100];
vector<regex> Rules[100];
map<string,int> NameMap;
int NowPlayer,Port,LimitRule=10,LimitPlayer=16;
bool GameStart=false,EscapeFlag=false;
FileOp flProfile;
ClientSocket client;
ServerSocket server;
#define PLAYER_UNKOWN 0
#define PLAYER_LEFT 1
#define PLAYER_DEAD 2
#define PLAYER_OBSERVER 3
#define PLAYER_ALIVE 4
int PlayerStatus[100]={0};
char PlayerStatusText[][16]=
	{"Unkown","Left game","Dead","Observing","Alive"};

void SetProfile()
{
	flProfile.open("./profile.txt",OVERWRITE);
	ColorPrintf(0x70,"Update Profile:\n");
	ColorPrintf(0x07,"Input Ip\n");
	scanf("%s",IpInput);
	ColorPrintf(0x07,"Input port\nServer use this port too\n");
	scanf("%d",&Port);
	ColorPrintf(0x07,"Input Name\n");
	scanf("%s",MyName);
	flProfile.printf("%s\n%d\n%s",IpInput,Port,MyName);
	ColorPrintf(0x07,"Updated!\n");
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
bool InvalidRule(const char* rule,int ses)
{
	try{regex a(rule);}
	catch(const std::regex_error& e)
	{
		server.Send(ses,"&Error Syntax error");
		return true;
	}
	int len=strlen(rule);
	if(len>10)
	{
		server.Send(ses,"&Error Regex too long");
		return true;
	}
	int invCnt=0;
	for(int i=0;i<len;i++)
		if(rule[i]=='*'||rule[i]=='{'||rule[i]=='+')
			if(i>0&&rule[i-1]!='\\')
				invCnt++;
	if(invCnt>1)
	{
		server.Send(ses,"&Error More than one wildcard!");
		return true;
	}
	regex r(rule);
	for(auto i:MustNotMatches)
		if(regex_search(i.c_str(),r))
		{
			server.Send(ses,"&Error Matches: %s",i.c_str());
			return true;
		}
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
		flRead.getline(InStr);
		MustNotMatches.push_back(InStr);
		if(flRead.Eof())	break;
	}
	return;
}
bool InvalidName(const char* name,int ses)
{
	int len=strlen(name);
	if(len>10)
	{
		server.Send(ses,"&Error Name too long");
		return true;
	}
	for(int i=0;i<len;i++)
		if(!isalnum(name[i]))
		{
			server.Send(ses,"&Error Invalid characters");
			return true;
		}
	return false;
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
Commands<> CliCom={{
	{"&Server",0,[](const char* msg)
	{OnScreen(0x07,0x0B,"Server>%s\n",msg);}},
	{"&Error",0,[](const char* msg)
	{OnScreen(0x0C,0x0B,"[Error] %s\n",msg);}},
	{"&Info",0,[](const char* msg)
	{OnScreen(0x06,0x0B,"%s\n",msg);}},
	{"&Return",0,[](const char* msg)
	{OnScreen(0x0E,0x0B,">%s\n",msg);}},
	{"&Blocked",0,[](const char* msg)
	{
		sscanf(msg,"%s",InStr);
		strcpy(CmdToExe,msg+1+strlen(InStr));
		OnScreen(0x0A,0x0B,"~Blocked from %s:\n",InStr);
		OnScreen(0x0E,0x0B,"~%s\n",CmdToExe);
	}},
	{"&CMD",0,[](const char* msg)
	{
		sscanf(msg,"%s",InStr);
		strcpy(CmdToExe,msg+1+strlen(InStr));
		OnScreen(0x0D,0x0B,"~Attack from %s:\n",InStr);
		OnScreen(0x0E,0x0B,"~%s\n",CmdToExe);
		CursorGoto(Out.Query());
		int ret=system(CmdToExe);
		Out.Goto(0,GetCursorxy().Y);
		In.Goto(-1,GetCursorxy().Y);
		OnScreen(0x0E,0x0B,"Command returns %d(%#x).\n",ret,ret);
		client.Send("&Return %s %d",InStr,ret);
	}},
	{"&Broadcast",0,[](const char* msg)
	{
		bool blocked;
		sscanf(msg,"%d %s %s",&blocked,InStr,Msg);
		strcpy(CmdToExe,msg+4+strlen(InStr)+strlen(Msg));
		OnScreen(blocked?0x02:0x05,0x0B,"%%[%s]",blocked?"Blocked":"Unblocked");
		OnScreen(0x03,0x0B,"%s >%c %s\n",InStr,blocked?'(':'>',Msg);
		OnScreen(blocked?0x02:0x05,0x0B,"%%~%s\n",CmdToExe);
	}},
	{"&Kick",0,[](const char* msg)
	{
		OnScreen(0x06,0x0B,"You are kicked form server.\n");
		Sleep(3000);
		client.Disconnect();
		EscapeFlag=true;
	}}
},
	[](const char* msg)
		{OnScreen(0x0C,0x0B,"[Unknown]Server sends \"%s\"\n",msg);},
	[](const char* msg)
		{OnScreen(0x0C,0x0B,"Access denied \"%s\"\n",msg);}
};
void ClientMsgProc(const char* msg)
{
	if(msg[0]=='$'||msg[0]=='&')
		CliCom.Explain(msg,1);
	else
		OnScreen(0x07,0x0B,"%s\n",msg);
	return;
}
void ClientMain()
{
	ConTitleA("Client");
	ReadProfile();
	int res=client.Connect(IpInput,Port);
	if(res!=0)
	{
		ColorPrintf(0x0C,"Connect Failed %d\n",res);
		return;
	}
	system("cls");
	ColorPosPrintf(0x06,0,0,"%s:%d\n",IpInput,Port);
	Out.Goto(0,1);
	In.Goto(1,2);
	In.StartScan(true,0x0B);
	client.Send("%s",MyName);
	EscapeFlag=false;
	int rec;
	while(true)
	{
		rec=client.Receive(Msg);
		if(rec==WSAEKICKED)
			EscapeFlag=true;
		if(rec!=SOCKET_ERROR)
			ClientMsgProc(Msg);
		if(KeyDown(VK_RETURN))
			if(strlen(In.GetScan())>0)
			{
				strcpy(Msg,In.GetScan());
				In.ClearScan();
				if(Msg[0]=='&')
					OnScreen(0x0C,0x0B,"Do not lie to server!\n");
				else
				{
					client.Send(Msg);
					OnScreen(0x0B,0x0B,"<%s\n",Msg);
				}
			}
		if(KeyDown(VK_ESCAPE))
		{
			OnScreen(0x06,0x0B,"Escaping...\n");
			EscapeFlag=true;
		}
		if(EscapeFlag)
			goto ClientEnd;
		Sleep(50);
	}
ClientEnd:
	In.EndScan();
	client.Disconnect();
	return;
}
Commands<int> SerCom={{
	{"$Name",PLAYER_ALIVE,[](const char* msg,int ses)
	{
		sscanf(msg,"%s",InStr);
		if(InStr[0]=='#')
			server.Send(ses,"&Error Not a creative name \"%s\"",InStr);
		else if(InvalidName(InStr,ses))
			return;
		else if(NameMap.find(InStr)!=NameMap.end())
			server.Send(ses,"&Error Someone already called \"%s\"",InStr);
		else
		{
			OnScreen(0x08,0x0E,"[%s=>%s]\n",DisplayNames[ses].c_str(),InStr);
			server.SendToAll("&Info %s renamed %s",DisplayNames[ses].c_str(),InStr);
			DisplayNames[ses]=InStr;
			NameMap[DisplayNames[ses]]=ses;
		}
	}},
	{"$Rule",PLAYER_ALIVE,[](const char* msg,int ses)
	{
		int num;
		if(sscanf(msg,"%d",&num)==1)
			if(num>Rules[ses].size()-1)
				server.Send(ses,"&Error No such rule");
			else
			{
				strcpy(InStr,msg+2);
				if(InvalidRule(InStr,ses))
					return;
				else
					server.Send(ses,"&Return Rule changes %s -> %s",
						RuleBackUps[ses][num].c_str(),InStr),
					Rules[ses][num]=regex(InStr),
					RuleBackUps[ses][num]=InStr;
			}
		else
		{
			if(Rules[ses].size()>=LimitRule)
				server.Send(ses,"&Error Too many rules");
			else if(InvalidRule(msg,ses))
				return;
			else
				Rules[ses].push_back(regex(msg)),
				RuleBackUps[ses].push_back(msg);
		}
	}},
	{"$MyRule",PLAYER_ALIVE,[](const char* msg,int ses)
	{
		string res="Your rules are:\n";
		int j=0;
		for(auto k:RuleBackUps[ses])
			sprintf(Msg,"%d: %s\n",j,k.c_str()),
			j++,res+=Msg;
		server.Send(ses,res.c_str());
	}},
	{"$Alias",PLAYER_OBSERVER,[](const char* msg,int ses)
	{
		sscanf(msg,"%s",InStr);
		auto it=NameMap.find(InStr);
		if(it==NameMap.end())
			server.Send(ses,"Error No such player.");
		else
		{
			int tar=it->second;
			sprintf(Msg,"Alias to player %s:\n",InStr);
			string res=Msg;
			for(auto j:NameMap)
				if(j.second==tar)
					res+="- "+j.first+'\n';
			server.Send(ses,res.c_str());
		}
	}},
	{"$Attack",PLAYER_ALIVE,[](const char* msg,int ses)
	{
		sscanf(msg,"%s",InStr);
		if(strcmp(InStr,"Server")==0)
			server.Send(ses,"&Error Hey! You can't attack server! X(");
		else if(NameMap.find(InStr)==NameMap.end())
			server.Send(ses,"&Error Player \"%s\" not found!",InStr);
		else
		{
			int tar=NameMap[InStr];
			if(PlayerStatus[tar]!=PLAYER_ALIVE)
			{
				server.Send(ses,"&Error %s is not alive!",
					DisplayNames[tar].c_str());
				return;
			}
			strcpy(InStr,msg+1+strlen(InStr));
			bool blocked=MatchRule(tar,InStr);
			if(blocked)
				server.Send(tar,"&Blocked %s %s",DisplayNames[ses].c_str(),InStr),
				server.Send(ses,"&Return Command is blocked!");
			else
				server.Send(tar,"&CMD %s %s",DisplayNames[ses].c_str(),InStr);
			int size=server.GetConnectedCnt();
			for(int i=0;i<size;i++)
				if(i!=tar&&i!=ses)
					server.Send(i,"&Broadcast %d %s %s\n%s",
						blocked,DisplayNames[ses].c_str(),
						DisplayNames[tar].c_str(),InStr);	
		}
	}},
	{"$Player",PLAYER_OBSERVER,[](const char* msg,int ses)
	{
		sprintf(Msg,"Now player left: %d\n",NowPlayer);
		string res=Msg;
		int size=server.GetConnectedCnt();
		for(int i=0;i<size;i++)
			sprintf(Msg,"%12s %s\n",
				DisplayNames[i].c_str(),PlayerStatusText[PlayerStatus[i]]),
			res+=Msg;
		server.Send(ses,res.c_str());
	}},
	{"&Return",PLAYER_ALIVE,[](const char* msg,int ses)
	{
		int ret;
		sscanf(msg,"%s %d",InStr,&ret);
		server.Send(NameMap[InStr],
			"&Return Command to %s returns %d(%#x)",
			DisplayNames[ses].c_str(),ret,ret);
	}},
	{"$Exit",PLAYER_OBSERVER,[](const char* msg,int ses)
		{server.Send(ses,"&Kick");}},
},
	[](const char* msg,int ses)
	{
		server.Send(ses,"&Error Unknown - \"%s\"",msg);
	},
	[](const char* msg,int ses)
	{
		server.Send(ses,"&Error Access denied - \"%s\"",msg);
	}
};
void ServerMsgProc(int ses,const char* msg)
{
	if(GameStart&&(msg[0]=='$'||msg[0]=='&'))
		SerCom.Explain(msg,PlayerStatus[ses],ses);
	else
	{
		OnScreen(0x07,0x0E,"%s>%s\n",DisplayNames[ses].c_str(),msg);
		for(int j=0;j<server.GetConnectedCnt();j++)
			if(j!=ses)
				server.Send(j,"%s>%s",DisplayNames[ses].c_str(),msg);
	}
	return;
}
void ServerMain()
{
	ConTitleA("Server");
	ReadProfile();
	ReadMustNotMatches();
	NowPlayer=0;
	int AliveCount=40;
	NameMap["Server"]=-1;
	GameStart=false;
	if(!server.BindPort(Port))
	{
		ColorPrintf(0x0C,"Can not bind port %d:Err %d\n",Port,errno);
		Sleep(1000);
		return;
	}
	system("cls");
	EscapeFlag=false;
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
				if(server.BlockReceive(id,InStr)==SOCKET_ERROR
				 ||InvalidName(InStr,id))
				{
					server.Send(id,"&Kick");
					goto AcceptEnd;
				}
				if(NameMap.find(InStr)!=NameMap.end())
				{
					server.Send(id,
						"&Error Somebody already named \"%s\".",InStr);
					server.Send(id,"&Kick");
					goto AcceptEnd;
				}
				DisplayNames[id]=InStr;
				NameMap[InStr]=id;
				sprintf(Msg,"#%d",id);
				NameMap[Msg]=id;
				AliveCount=40;
				if(!GameStart)
				{
					NowPlayer++;
					PlayerStatus[id]=PLAYER_ALIVE;
					server.SendToAll("&Info %s(#%d) joined the game",
						DisplayNames[id].c_str(),id);
					OnScreen(0x06,0x0E,"%s(#%d) connected.\n",
						DisplayNames[id].c_str(),id);
				}
				else
				{
					PlayerStatus[id]=PLAYER_OBSERVER;
					server.Send(id,"&Info Game started, now observing.");
					OnScreen(0x08,0x0E,"%s join observing\n",
						DisplayNames[id].c_str());
				}
			AcceptEnd:
			}
		}
		server.ReceiveFormAll(ServerMsgProc);
		if(KeyDown(VK_RETURN)&&strlen(In.GetScan())!=0)
		{
			strcpy(Msg,In.GetScan());
			In.ClearScan();
			OnScreen(0x0E,0x0E,"<%s\n",Msg);
			if(Msg[0]=='$'||Msg[0]=='&')
				server.SendToAll(Msg);
			else if(Msg[0]=='@')
				sscanf(Msg+1,"%s",InStr),
				strcpy(CmdToExe,Msg+2+strlen(InStr)),
				server.Send(NameMap[InStr],CmdToExe);
			else
				server.SendToAll("&Server %s",Msg);
		}
		if(KeyDown(VK_F1))
		{
			GameStart=true,
			server.SendToAll("&Info Game start!");
			OnScreen(0x06,0x0E,"Game start\n");
			In.ClearScan();
		}
		if(KeyDown(VK_ESCAPE))
			EscapeFlag=true;
		if(AliveCount==0)
		{
			AliveCount=40;
			int size=server.GetConnectedCnt();
			for(int i=0;i<size;i++)
				if(PlayerStatus[i]==PLAYER_ALIVE&&!server.CheckAlive(i))
					OnScreen(0x06,0x0E,"%s disconnect.\n",DisplayNames[i].c_str()),
					server.SendToAll("&Info %s dead",DisplayNames[i].c_str()),
					NowPlayer--,PlayerStatus[i]=PLAYER_DEAD;
			if(GameStart&&NowPlayer==1)
				for(int i=0;i<size;i++)
					if(PlayerStatus[i]==PLAYER_ALIVE)
					{
						server.SendToAll("&Info %s WIN!\nGame ends.",
							DisplayNames[i].c_str());
						Sleep(5000);
						EscapeFlag=true;
					}
		}
		if(EscapeFlag)
			goto ServerEnd;
		Sleep(50);
	}
ServerEnd:
	OnScreen(0x06,0x0E,"Escaping...\n");
	server.SendToAll("&Kick");
	server.Restart();
	In.EndScan();
	NameMap.clear();
	DisplayNames.clear();
	Sleep(1000);
	return;
}
int main()
{
	ConTitleA("Cmd Fight");
	ReadProfile();
	printf(
		"PClib Cmd Fight\n"
		"PCwqyy(c) All right reserved.\n"
		"Type \"Help\" to get help.\n");
StartMain:
	ColorPrintf(0x0B,">");
	scanf("%s",InStr);
	if(strcmp(InStr,"Server")==0)
		ServerMain();
	else if(strcmp(InStr,"Client")==0)
		ClientMain();
	else if(strcmp(InStr,"Profile")==0)
		SetProfile();
	else if(strcmp(InStr,"Version")==0)
		printf("Version: PClib 25v3c\n");
	else if(strcmp(InStr,"Help")==0)
		printf(
			"Server\tStart a server on this computer\n"
			"Client\tJoin other's server\n"
			"Profile\tEdit your Profile\n"
			"Version\tView version\n"
			"Exit\tExit the program\n");
	else if(strcmp(InStr,"Exit")==0)
		return 0;
	else
		printf("Unknown command.\n");
	if(EscapeFlag)
	{
		system("cls");
		ColorPrintf(0x06,"Somehow you left the game.\n");
	}
	goto StartMain;
	return 0;
}