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
	static void AuthConnectAction(SyncObj^ Mediator)
	{
#ifdef DEBUG
		printf("Connection recieved : "+Mediator->Client->RemoteEndPoint->ToString()+"\n");
		printf("Sending seed.. ");
#endif
		unsigned int Seed = Ultilities::RandomUInt32();
		Client^ ConnectingClient = gcnew Client();
		ConnectingClient->InnerClient = Mediator->Client;
		ConnectingClient->AuthCrypt = gcnew AuthCryptographer();
		PasswordSeed* InitialSeed = new PasswordSeed(Seed);
		ConnectingClient->SendAuth( InitialSeed, InitialSeed->Length);
		ConnectingClient->PasswordSeed = Seed;
		Database::ConnectedClients->Add(Mediator->Client, ConnectingClient);
#ifdef DEBUG
		printf("seed has been sent successfully.\n");
#endif
	}
	static void AuthRecieveAction(array<unsigned char>^ buffer, SyncObj^ Mediator)
	{
		Client^ MyClient = Database::ConnectedClients[Mediator->Client];
		MyClient->AuthCrypt->Decode(&buffer);
		unsigned short Size = BitConverter::ToUInt16(buffer, 0);
		unsigned short Type = BitConverter::ToUInt16(buffer, 2);
		switch( Type)
		{
			case 1060:
            case 1086:
				{
#ifdef DEBUG
					printf("Login requested.\n");
#endif

					break;
				}
		}
	}
};




int main()
{
	printf("Starting auth server...");
	Server^ LoginServer = gcnew Server(Database::ServerIP, Database::LoginPort);
	LoginServer->Start();
	printf(" server started! ("+Database::ServerIP->ToString()+":"+Database::LoginPort+")\n");
	LoginServer->OnConnect += gcnew Action<SyncObj^>(&Handler::AuthConnectAction);
	LoginServer->OnRecieve += gcnew Server::DoubleAction(&Handler::AuthRecieveAction);
	for(;;)
	{ }
    return 0;
}