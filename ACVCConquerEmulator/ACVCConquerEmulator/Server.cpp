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
#include "stdafx.h"

public ref class SyncObj
{
public:
	Socket^ Client;
	array<unsigned char>^ Buffer;
	Object^ Connector;
};

public ref class Server
{
public:
	delegate void DoubleAction(array<unsigned char>^ buffer, SyncObj^ Mediator);
	event DoubleAction^ OnRecieve;
	event Action<SyncObj^>^ OnConnect;
	IPAddress^ InternalIP;
	int InternalPort;
	Socket^ Listener;
public:
	Server(IPAddress^ ListenIP, int Port)
	{
		InternalIP = ListenIP;
		InternalPort = Port;
		Listener = gcnew Socket(AddressFamily::InterNetwork,
			SocketType::Stream, ProtocolType::Tcp);
		Listener->Bind(gcnew IPEndPoint(InternalIP, InternalPort));
	}
public:
	void RecieveData( IAsyncResult^ Result)
	{
		SyncObj^ Mediator = (SyncObj^)Result->AsyncState;
		SocketError% Error = (SocketError%)SocketError::Disconnecting;
		int Size = Mediator->Client->EndReceive(Result, Error);
		if( Size > 0 && Error == SocketError::Success)
		{
			array<unsigned char>^ buffer = gcnew array<unsigned char>(Size);
			Buffer::BlockCopy(Mediator->Buffer, 0, buffer, 0, Size);
			OnRecieve(buffer, Mediator);
					Mediator->Client->BeginReceive(Mediator->Buffer, 0, 65535, SocketFlags::None,
			gcnew AsyncCallback(this, &Server::RecieveData), Mediator);
		}
		else
		{
			if(Mediator->Client->Connected)
				Mediator->Client->Disconnect(true);
		}
	}
	void RecieveConnection( IAsyncResult^ Result)
	{
		SyncObj^ Mediator = (SyncObj^)Result->AsyncState;
		Mediator->Client = Listener->EndAccept(Result);
		Mediator->Buffer = gcnew array<unsigned char>(65535);

		Mediator->Client->BeginReceive(Mediator->Buffer, 0, 65535, SocketFlags::None,
			gcnew AsyncCallback(this, &Server::RecieveData), Mediator);

		OnConnect(Mediator);
		Listener->BeginAccept(gcnew AsyncCallback(this, &Server::RecieveConnection) , gcnew SyncObj() );
	}
public:
	void Start()
	{
		Listener->Listen(5000);
		Listener->BeginAccept(gcnew AsyncCallback(this, &Server::RecieveConnection) , gcnew SyncObj() );
	}

};