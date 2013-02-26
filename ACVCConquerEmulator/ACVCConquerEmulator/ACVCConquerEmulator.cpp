// ACVCConquerEmulator.cpp : main project file.

#include "stdafx.h"
#include "Database.h"
#include "Server.cpp"



public ref class Handler
{
public:
	static void ConnectAction(SyncObj^ Mediator)
	{

	}
public:
	static void RecieveAction(array<unsigned char>^ buffer, SyncObj^ Mediator)
	{

	}
};




int main()
{
	printf("Starting server...");
	Server^ LoginServer = gcnew Server(Database::ServerIP, Database::LoginPort);
	LoginServer->Start();
	printf(" server started!");
	LoginServer->OnConnect += gcnew Action<SyncObj^>(&Handler::ConnectAction);
	LoginServer->OnRecieve += gcnew Server::DoubleAction(&Handler::RecieveAction);
	for(;;)
	{ }
    return 0;
}