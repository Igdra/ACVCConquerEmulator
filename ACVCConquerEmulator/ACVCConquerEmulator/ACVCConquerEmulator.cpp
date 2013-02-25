// ACVCConquerEmulator.cpp : main project file.

#include "stdafx.h"
#include "Database.h"
#include "Server.cpp"

void ConnectAction(SyncObj^ Mediator);

int main()
{
	printf("Starting server...");
	Server^ LoginServer = gcnew Server(Database::ServerIP, Database::LoginPort);
	LoginServer->Start();
	printf(" server started!");
	//LoginServer->OnConnect = &ConnectAction;
	for(;;)
	{ }
    return 0;
}

void ConnectAction(SyncObj^ Mediator)
{

}