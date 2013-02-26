#pragma once

#include "stdafx.h"
#include "Client.h"

ref class Database sealed abstract
{
public:
	static IPAddress^ ServerIP = IPAddress::Parse("192.168.0.185");
	static int LoginPort = 9958;
	static int GamePort = 5816;
	static Dictionary<Socket^, Client^>^ ConnectedClients = gcnew Dictionary<Socket^, Client^>();
};
