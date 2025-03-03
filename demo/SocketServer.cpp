#include"../src/Socket.hpp"
char msg[1000],in[1000];
char names[16][100];
int main()
{
	ServerSocket server;
	unsigned short port;
	printf("Input port\n");
	scanf("%ud",&port);
	if(!server.BindPort(port))
	{
		printf("Err\n");
		return errno;
	}
	printf("done!\n");
	while(true)
	{
		if(server.Listen())
		{
			int acc=server.Accept();
			if(acc!=INVALID_SOCKET)
				printf("Client #%d connected.\n",acc),
				server.Send(acc,"Hello client #%d!",acc);
		}
		for(int i=0;i<server.GetConnctedCnt();i++)
			if(server.Receive(i,msg)!=-1)
			{
				printf("%d>%s\n",i,msg);
				for(int j=0;j<server.GetConnctedCnt();j++)
					server.Send(j,"%d>%s",i,msg);
			}
		Sleep(50);
	}
	return 0;
}

	