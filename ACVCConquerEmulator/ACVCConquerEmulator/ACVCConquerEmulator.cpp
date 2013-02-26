// ACVCConquerEmulator.cpp : main project file.

#include "stdafx.h"
#include "Database.h"
#include "Server.cpp"
#include "Ultilities.h"
#include "AuthCryptographer.h"
#include "Client.h"
#include "Packets.cpp"

#define DEBUG

public ref class Handler
{
public:
	static void ConnectAction(SyncObj^ Mediator)
	{
#ifdef DEBUG
		printf("Connection recieved : "+Mediator->Client->RemoteEndPoint->ToString()+"\n");
		printf("Sending seed..\n");
#endif
		unsigned int Seed = Ultilities::RandomUInt32();
		Client^ ConnectingClient = gcnew Client();
		ConnectingClient->InnerClient = Mediator->Client;
		ConnectingClient->AuthCrypt = gcnew AuthCryptographer();
		PasswordSeed* InitialSeed = new PasswordSeed(Seed);
		ConnectingClient->SendAuth((void*)InitialSeed, InitialSeed->Length);
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
	printf(" server started!\n");
	LoginServer->OnConnect += gcnew Action<SyncObj^>(&Handler::ConnectAction);
	LoginServer->OnRecieve += gcnew Server::DoubleAction(&Handler::RecieveAction);
	for(;;)
	{ }
    return 0;
}