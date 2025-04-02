#pragma once
#define PCL_SOCKET

#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstdio>
#include<vector>
#include<ctime>
using std::vector;

#define pcSK_MAX_BUFF 1024
#define pcSK_PING_INFO "!ping!"
#define pcSK_KICK_INFO "!kick!"
#define WSAEKICKED (WSABASEERR + 9000)

class ServerSocket
{
private:
	struct info
	{
		SOCKET soc;
		bool alive;
	};
	WSADATA wsaData;
	SOCKET listener=INVALID_SOCKET;
	vector<info> sockets;
	char buff[pcSK_MAX_BUFF],buff2[pcSK_MAX_BUFF];
	bool valid(int session)
		{return sockets.size()>session||sockets[session].alive;}
	bool create()
	{
		int opt=1;
		listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));
		unsigned long mode=-1;
		ioctlsocket(listener,FIONBIO,&mode);
		return listener!=INVALID_SOCKET;
	}
public:
	ServerSocket()
	{
		WSAStartup(MAKEWORD(2,2),&wsaData);
		if(!create()) throw("Unable to create socket.");
	}
	~ServerSocket()
	{
		closesocket(listener);
		for(auto i:sockets)
			if(i.soc!=INVALID_SOCKET)
				closesocket(i.soc);
		WSACleanup();
	}
	void Close()
	{
		for(auto i:sockets)
			closesocket(i.soc);
		sockets=vector<info>();
		closesocket(listener);
		listener=INVALID_SOCKET;
		return;
	}
	bool Restart()
	{
		Close();
		return create();
	}
	bool BindPort(unsigned short port)
	{
		sockaddr_in serverAddr;
		serverAddr.sin_family=AF_INET;
		serverAddr.sin_addr.s_addr=INADDR_ANY;
		serverAddr.sin_port=htons(port);
		return bind(listener,(sockaddr*)&serverAddr,sizeof(serverAddr))!=SOCKET_ERROR;
	}
	bool Listen()
		{return listen(listener,SOMAXCONN)!=SOCKET_ERROR;}
	int Accept()
	{
		SOCKET sock=accept(listener,nullptr,nullptr);
		unsigned long mode=-1;
		ioctlsocket(sock,FIONBIO,&mode);
		if(sock==INVALID_SOCKET)	return -1;
		sockets.push_back({sock,true});
		return sockets.size()-1;
	}
	bool Send(int session,const char* data)
	{
		sprintf(buff2,"%d %s",strlen(data),data);
		return send(sockets[session].soc,buff2,strlen(buff2),0)>0;
	}
	template<typename ...types>
	bool Send(int session,const char* data,types ...args)
	{
		if(!valid(session))
			return SOCKET_ERROR;
		sprintf(buff,data,args...);
		return Send(session,buff);
	}
	int SendToAll(const char* data)
	{
		int ret=0;
		int size=sockets.size();
		for(int i=0;i<size;i++)
			if(Send(i,data)>0)
				ret++;
		return ret;
	}
	template<typename ...types>
	int SendToAll(const char* data,types ...args)
	{
		int ret=0;
		int size=sockets.size();
		for(int i=0;i<size;i++)
			if(Send(i,data,args...)>0)
				ret++;
		return ret;
	}
	int Receive(int session,char* dest)
	{
		if(!valid(session))
			return SOCKET_ERROR;
		if(buff[0]=='\0')
		{
			int ret=recv(sockets[session].soc,buff,pcSK_MAX_BUFF,0);
			if(ret<0)	return ret;
			buff[ret]='\0';
		}
		int len,now=0,now2=0;
		if(sscanf(buff,"%d",&len)!=1)
		{
			buff[0]='\0';
			dest[0]='\0';
			return SOCKET_ERROR;
		}
		while(isdigit(buff[now++]));
		for(int i=0;i<len;i++,now++)
			dest[i]=buff[now];
		while(buff[now]!='\0')
			buff[now2++]=buff[now++];
		dest[len]='\0';
		buff[now2]='\0';
		return len;
	}
	int ReceiveFormAll(void (*proc)(int session,const char* msg))
	{
		int ret=0;
		int size=sockets.size();
		for(int i=0;i<size;i++)
			if(Receive(i,buff2)>0)
				ret++,proc(i,buff2);
		return ret;
	}
	int BlockReceive(int session,char* dest,int timeout=1000,int interval=50)
	{
		int ret=SOCKET_ERROR;
		int end=clock()+timeout;
		while(clock()<end&&ret==SOCKET_ERROR)
			ret=Receive(session,dest),
			Sleep(interval);
		return ret;
	}
	int GetConnectedCnt(){return sockets.size();}
	bool CheckAlive(int session)
	{
		int res=Send(session,pcSK_PING_INFO);
		sockets[session].alive=(res>0);
		return sockets[session].alive;
	}
	int GetAliveCnt()
	{
		int ret=0;
		for(auto i:sockets)
			if(i.alive)
				ret++;
		return ret;
	}
	void Kick(int session)
	{
		Send(session,pcSK_KICK_INFO);
		shutdown(sockets[session].soc,SD_BOTH);
		sockets[session].alive=false;
		return;
	}
};


class ClientSocket
{
private:
	WSADATA wsaData;
	SOCKET sock=INVALID_SOCKET;
	char buff[pcSK_MAX_BUFF],buff2[pcSK_MAX_BUFF];
	bool create()
	{
		sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		unsigned long mode=-1;
		ioctlsocket(sock,FIONBIO,&mode);
		return sock!=INVALID_SOCKET;
	}
public:
	ClientSocket()
	{
		WSAStartup(MAKEWORD(2,2),&wsaData);
		sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		unsigned long mode=-1;
		ioctlsocket(sock,FIONBIO,&mode);
	}
	~ClientSocket()
	{
		closesocket(sock);
		WSACleanup();
	}
	/** @return `0` means succeeded, check [MSDN](https://learn.microsoft.com/zh-cn/windows/win32/winsock/windows-sockets-error-codes-2)
	 * ```
	 * WSAEACCES	 访问权限不足
	 * WSAEFAULT	 错误的地址
	 * WSAEINVAL	 无效的参数
	 * WSAEADDRINUSE	 地址已经在使用中
	 * WSAENETDOWN	 网络已关闭
	 * WSAENETUNREACH	 网络不可达
	 * WSAETIMEDOUT	 连接超时
	 * WSAECONNREFUSED 连接被拒绝
	 * WSAEHOSTUNREACH 无法到达主机
	 * WSAECONNABORTED 连接被中止
	 * WSAECONNRESET	 连接被重置
	 * ```
	 */
	int Connect(const char* address,unsigned short port,int time=5)
	{
		sockaddr_in Addr;
		Addr.sin_family=AF_INET;
		Addr.sin_port=htons(port);
		addrinfo hints={0},*res=nullptr;
		hints.ai_family=AF_INET;
		hints.ai_socktype=SOCK_STREAM;
		if(getaddrinfo(address,nullptr,&hints,&res)!=0)
			return WSAGetLastError();
		Addr.sin_addr=((sockaddr_in*)res->ai_addr)->sin_addr;
		freeaddrinfo(res);
		if(connect(sock,(sockaddr*)&Addr,sizeof(Addr))==SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
				return WSAGetLastError();
			fd_set writefds;
			FD_ZERO(&writefds);
			FD_SET(sock,&writefds);
			TIMEVAL timeout={time,0};
			int result=select(0,NULL,&writefds,NULL,&timeout);
			if(result>0&&FD_ISSET(sock,&writefds))	return 0;
			else	return -1;
		}
		else return 0;
	}
	void Disconnect()
	{
		closesocket(sock);
		sock=INVALID_SOCKET;
		create();
	}
	bool Send(const char* data)
	{
		sprintf(buff2,"%d %s",strlen(data),data);
		return send(sock,buff2,strlen(buff2),0)!=SOCKET_ERROR;
	}
	template<typename ...types>
	bool Send(const char* data,types ...args)
	{
		sprintf(buff,data,args...);
		return Send(buff);
	}
	int Receive(char* dest)
	{
		if(buff[0]=='\0')
		{
			int ret=recv(sock,buff,pcSK_MAX_BUFF,0);
			if(ret<0)	return ret;
			buff[ret]='\0';
		}
		int len,now=0,now2=0;
		if(sscanf(buff,"%d",&len)!=1)
		{
			buff[0]='\0';
			dest[0]='\0';
			return SOCKET_ERROR;
		}
		while(isdigit(buff[now++]));
		for(int i=0;i<len;i++,now++)
			dest[i]=buff[now];
		while(buff[now]!='\0')
			buff[now2++]=buff[now++];
		dest[len]='\0';
		buff[now2]='\0';
		if(strcmp(dest,pcSK_PING_INFO)==0)
		{
			dest[0]='\0';
			return SOCKET_ERROR;
		}
		else if(strcmp(dest,pcSK_KICK_INFO)==0)
		{
			dest[0]='\0';
			Disconnect();
			return WSAEKICKED;
		}
		return len;
	}
};

#include"Multinclude.hpp"