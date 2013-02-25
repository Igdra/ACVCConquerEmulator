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

	}
	void RecieveConnection( IAsyncResult^ Result)
	{
		SyncObj^ Mediator = (SyncObj^)Result->AsyncState;
		Mediator->Client = Listener->EndAccept(Result);
		Mediator->Buffer = gcnew array<unsigned char>(65535);

		Listener->BeginReceive(Mediator->Buffer, 0, 65535, SocketFlags::None,
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