/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of ACVCConquerEmulator
    Copyright 2013 - ACVC Team
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------

Author : ACVC
*/

// ACVCConquerEmulator.cpp : main project file.

#include "stdafx.h"
#include "Database.h"
#include "Server.cpp"
#include "Ultilities.h"
#include "AuthCryptographer.h"
#include "Client.h"
#include "Packets.cpp"
#include "PasswordCrypt.cpp"
#include "RC5.cpp"

#define DEBUG
#define CLIENT_CREATE_ACCOUNT

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

					PasswordCryptopgraphy^ PassCrypt_ = gcnew PasswordCryptopgraphy(User);
					RC5^ RC5Crypt = gcnew RC5(rc5Key);
					RawPassword = RC5Crypt->Decrypt(RawPassword);
					PassCrypt_->Decrypt( &RawPassword, 16) ;
					String^ Password = System::Text::ASCIIEncoding::ASCII
						->GetString(RawPassword)->Trim('\0');
#ifdef DEBUG
					printf("[DEBUG]Password found : "+ Password);
					/* PHEW! Finally done! */
#endif

					if( Database::ValidateAuth(User, Password) )
					{

					}
					else
					{
#ifdef CLIENT_CREATE_ACCOUNT
					/* Create account should login fail. */
#endif
					}
					break;
				}
		}
	}
};







int main()
{
	printf("Starting auth server...");
	try
	{
		Server^ LoginServer = gcnew Server(Database::ServerIP, Database::LoginPort);
		LoginServer->Start();
		printf(" server started! ("+Database::ServerIP->ToString()+":"+Database::LoginPort+")\n");
		LoginServer->OnConnect += gcnew Action<SyncObj^>(&Handler::AuthConnectAction);
		LoginServer->OnRecieve += gcnew Server::DoubleAction(&Handler::AuthRecieveAction);
	}
	catch (Exception^ e)
	{
		printf(" server could not be started. Invalid IP Address/Port. \nPress any key to exit.\n");
		getchar();
		return 0;
	}

	for(;;)
	{

	}
    return 0;
}