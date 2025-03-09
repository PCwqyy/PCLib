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

char MyName[100],IpInput[100],Msg[1000],InStr[1000],Command[1000];
vector<string> DisplayNames(100,""),MustNotMatches,RuleBackUps[100];
vector<regex> Rules[100];
map<string,int> NameMap;
int NowPlayer,Port;
int LimitRule=10,LimitPlayer=16;
bool Alive[50];
FileOp flProfile;

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
		MustNotMatches.push_back(InStr);
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
				{
					strcpy(InStr,Msg+8);
					OnScreen(0x07,0x0B,"Server>%s\n",InStr);
				}
				else if(strcmp(InStr,"Error")==0)
				{
					strcpy(InStr,Msg+7);
					OnScreen(0x0C,0x0B,"[Error]%s\n",InStr);
				}
				else if(strcmp(InStr,"Blocked")==0)
				{
					sscanf(Msg+9,"%s",InStr);
					strcpy(Command,Msg+10+strlen(InStr));
					OnScreen(0x0A,0x0B,"~Blocked from %s:\n",InStr);
					OnScreen(0x0E,0x0B,"~%s\n",Command);
				}
				else if(strcmp(InStr,"CMD")==0)
				{
					sscanf(Msg+5,"%s",InStr);
					strcpy(Command,Msg+6+strlen(InStr));
					OnScreen(0x0C,0x0B,"~Attack from %s:\n",InStr);
					OnScreen(0x0E,0x0B,"~%s\n",Command);
					CursorGoto(Out.Query());
					int ret=system(Command);
					Out.Goto(0,GetCursorxy().Y);
					In.Goto(-1,GetCursorxy().Y);
					OnScreen(0x0E,0x0B,"Command returns %d(%#x).\n",ret,ret);
					client.Send("&Return %s %d",InStr,ret);
				}
				else if(strcmp(InStr,"Info")==0)
				{
					strcpy(InStr,Msg+6);
					OnScreen(0x06,0x0B,"%s\n",InStr);
				}
				else if(strcmp(InStr,"Return")==0)
				{
					strcpy(InStr,Msg+8);
					OnScreen(0x0E,0x0B,"%s\n",InStr);
				}
				else
					OnScreen(0x0C,0x0B,"Unknown - \"%s\"",Msg);
			}
			else
				OnScreen(0x07,0x0B,"%s\n",Msg);
		}
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
	NameMap["Server"]=-1;
	if(!server.BindPort(Port))
	{
		ColorPrintf(0x0C,"Can not bind port %d\n",Port);
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
				DisplayNames[id]=InStr;
				NameMap[DisplayNames[id]]=id;
				Alive[id]=true;
				NowPlayer++,AliveCount=40;
				OnScreen(0x06,0x0E,"Client #%d connected.\n",id);
				server.Send(id,"$Server You are #%d!",id);
				server.SendToAll("$Info #%d Joined",id);
			}
		}
		for(int i=0;i<server.GetConnectedCnt();i++)
			if(Alive[i]&&server.Receive(i,Msg)!=SOCKET_ERROR)
				if(Msg[0]=='$')
				{
					sscanf(Msg+1,"%s",InStr);
					if(strcmp(InStr,"Name")==0)
					{
						sscanf(Msg+6,"%s",InStr);
						if(InStr[0]=='#')
							server.Send(i,"$Error Not a creative name \"%s\"",InStr);
						else if(NameMap.find(InStr)!=NameMap.end())
							server.Send(i,"$Error Invalid Name \"%s\"",InStr);
						else
						{
							OnScreen(0x08,0x0E,"[%s=>%s]\n",DisplayNames[i].c_str(),InStr);
							server.SendToAll("$Info %s renamed %s",DisplayNames[i].c_str(),InStr);
							DisplayNames[i]=InStr;
							NameMap[DisplayNames[i]]=i;
						}
					}
					else if(strcmp(InStr,"Rule")==0)
					{
						int num;
						if(sscanf(Msg+6,"%d",&num)==1)
							if(num>Rules[i].size()-1)
								server.Send(i,"$Error No such rule");
							else
							{
								strcpy(InStr,Msg+8);
								if(InvalidRule(InStr))
									server.Send(i,"$Error Invalid rule");
								else
									server.Send(i,"$Return %s -> %s",RuleBackUps[i][num].c_str(),InStr),
									Rules[i][num]=regex(InStr),
									RuleBackUps[i][num]=InStr;
							}
						else
						{
							strcpy(InStr,Msg+6);
							if(Rules[i].size()>=LimitRule)
								server.Send(i,"$Error Too many rules");
							else if(InvalidRule(InStr))
								server.Send(i,"$Error Invalid rule");
							else
								Rules[i].push_back(regex(InStr)),
								RuleBackUps[i].push_back(InStr);
						}
					}
					else if(strcmp(InStr,"MyRule")==0)
					{
						string res="$Return Your rules are:\n";
						int j=0;
						for(auto k:RuleBackUps[i])
							sprintf(Msg,"%d: %s\n",j,k.c_str()),
							j++,res+=Msg;
						server.Send(i,res.c_str());
					}
					else if(strcmp(InStr,"Alias")==0)
					{
						sscanf(Msg+7,"%s",InStr);
						auto it=NameMap.find(InStr);
						if(it==NameMap.end())
							server.Send(i,"Error No such player.");
						else
						{
							int tar=it->second;
							sprintf(Msg,"Alias to player %s:\n",InStr);
							string res=Msg;
							for(auto j:NameMap)
								if(j.second==tar)
									res+="- "+j.first+'\n';
							server.Send(i,"$Return %s",res.c_str());
						}
					}
					else if(strcmp(InStr,"Attack")==0)
					{
						sscanf(Msg+8,"%s",InStr);
						if(strcmp(InStr,"Server")==0)
							server.Send(i,"$Error Hey! You can't attack server! X(");
						if(NameMap.find(InStr)==NameMap.end())
							server.Send(i,"$Error Player \"%s\" not found!",InStr);
						else
						{
							int tar=NameMap[InStr];
							strcpy(InStr,Msg+9+strlen(InStr));
							if(MatchRule(tar,InStr))
								server.Send(tar,"$Blocked %s %s",DisplayNames[i].c_str(),InStr),
								server.Send(i,"$Return Command is blocked!");
							else
								server.Send(tar,"$CMD %s %s",DisplayNames[i].c_str(),InStr);
						}
					}
					else
						server.Send(i,"$Error Unknown - \"%s\"",Msg);
				}
				else if(Msg[0]=='&')
				{
					sscanf(Msg+1,"%s",InStr);
					if(strcmp(InStr,"Return")==0)
					{
						int ret;
						sscanf(Msg+8,"%s %d",InStr,&ret);
						server.Send(NameMap[InStr],
							"$Return Command to %s returns %d(%#x)",
							DisplayNames[i].c_str(),ret,ret);
					}
				}
				else
				{
					OnScreen(0x07,0x0E,"%s>%s\n",DisplayNames[i].c_str(),Msg);
					for(int j=0;j<server.GetConnectedCnt();j++)
						if(Alive[j]&&j!=i)
							server.Send(j,"%s>%s",DisplayNames[i].c_str(),Msg);
				}
		if(KeyDown(VK_RETURN)&&strlen(In.GetScan())!=0)
		{
			strcpy(InStr,In.GetScan());
			In.ClearScan();
			OnScreen(0x0E,0x0E,"<%s\n",InStr);
			if(InStr[0]!='$'&&InStr[0]!='&')
				sprintf(Msg,"$Server %s",InStr);
			else strcpy(Msg,InStr);
			for(int i=0;i<server.GetConnectedCnt();i++)
				if(Alive[i])
					server.Send(i,"%s",Msg);
		}
		if(AliveCount==0)
		{
			AliveCount=40;
			for(int i=0;i<server.GetConnectedCnt();i++)
				if(Alive[i]&&!server.CheckAlive(i))
					OnScreen(0x06,0x0E,"%s disconnect.\n",DisplayNames[i].c_str()),
					server.SendToAll("$Info %s loses",DisplayNames[i].c_str()),
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
	printf(
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
	goto StartInput;
	return 0;
}