#pragma once

ref class Database sealed abstract
{
public:
	static IPAddress^ ServerIP = IPAddress::Parse("192.168.0.185");
	static int LoginPort = 9958;
	static int GamePort = 5816;
};
