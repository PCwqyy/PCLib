#include"../src/Socket.hpp"
char msg[1000],in[1000];
int main()
{
	ClientSocket client;
	char ipin[100];
	int port;
	printf("Input Ip\n");
	scanf("%s",ipin);
	printf("Input port\n");
	scanf("%d",&port);
	printf("ip:%s port:%d\n",ipin,port);
	int res=client.Connect(ipin,port);
	if(res!=0)
	{
		printf("Connect Failed %d",res);
		return 0;
	}
	while(true)
	{
		if(client.Receive(msg)!=-1)
			printf("%s\n",msg);
		if(GetKeyState(VK_CONTROL)&0x8000)
			putchar('<'),
			fgets(in,1000,stdin),
			in[strlen(in)-1]='\0',
			client.Send(in);
		Sleep(50);
	}
}

	