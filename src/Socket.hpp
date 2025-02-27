#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstdio>
#include<vector>
using std::vector;

#define SK_MAX_BUFF 1024

class ServerSocket
{
private:
	WSADATA wsaData;
	SOCKET listener=INVALID_SOCKET;
	vector<SOCKET> sockets;
	char buff[SK_MAX_BUFF];
public:
	ServerSocket()
	{
		WSAStartup(MAKEWORD(2,2),&wsaData);
		listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		unsigned long mode=-1;
		ioctlsocket(listener,FIONBIO,&mode);
	}
	~ServerSocket()
	{
		closesocket(listener);
		for(auto i:sockets)
			if(i!=INVALID_SOCKET)
				closesocket(sockets[i]);
		WSACleanup();
	}
	bool Create()
	{
		listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		return listener!=INVALID_SOCKET;
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
		sockets.push_back(sock);
		return sockets.size()-1;
	}
	bool Send(int session,const char *data)
		{return send(sockets[session],data,strlen(data),0)!=SOCKET_ERROR;}
	template<typename ...types>
	bool Send(int session,const char* data,types ...args)
	{
		sprintf(buff,data,args...);
		return send(sockets[session],buff,strlen(buff),0)!=SOCKET_ERROR;
	}
	int Receive(int session,char* dest)
	{
		int ret=recv(sockets[session],buff,SK_MAX_BUFF,0);
		buff[ret]='\0';
		strcpy(dest,buff);
		return ret;
	}
	int GetConnctedCnt(){return sockets.size();}
};


class ClientSocket
{
private:
	WSADATA wsaData;
	SOCKET sock=INVALID_SOCKET;
	char buff[SK_MAX_BUFF];
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
	bool Create()
	{
		sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		unsigned long mode=-1;
		ioctlsocket(sock,FIONBIO,&mode);
		return sock!=INVALID_SOCKET;
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
		Addr.sin_addr.s_addr=inet_addr(address);
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
	bool Send(const char* data)
		{return send(sock,data,strlen(data),0)!=SOCKET_ERROR;}
	template<typename ...types>
	bool Send(const char* data,types ...args)
	{
		sprintf(buff,data,args...);
		return send(sock,buff,strlen(buff),0)!=SOCKET_ERROR;
	}
	int Receive(char* dest)
	{
		int ret=recv(sock,buff,SK_MAX_BUFF,0);
		buff[ret]='\0';
		strcpy(dest,buff);
		return ret;
	}
};