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
		printf("[DEBUG]Connection recieved : "+Mediator->Client->RemoteEndPoint->ToString()+".\n");
		printf("[DEBUG]Sending seed.. ");
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
					printf("[DEBUG]Login requested.\n");
#endif
					String^ User = Ultilities::ReadString(buffer, 4, 16)->TrimEnd('\0');
#ifdef DEBUG
					printf("[DEBUG]User name found : '"+ User+"'.\n");
#endif
					array<unsigned char>^ RawPassword = gcnew array<unsigned char>(16);
					Buffer::BlockCopy(buffer, 132, RawPassword, 0, 16);
					msvcrt::msvcrt::srand((int)MyClient->PasswordSeed);
					array<unsigned char>^ rc5Key = gcnew array<unsigned char>(16);

					for (int i = 0; i < 16; i++)
						rc5Key[i] = (unsigned char)msvcrt::msvcrt::rand();

					/* TODO : Add password crypt. support.. (Prepared for hell?) */
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