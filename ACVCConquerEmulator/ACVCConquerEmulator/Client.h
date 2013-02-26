#pragma once

#include "AuthCryptographer.h"

ref class Client
{
public:
	Client(void);
	AuthCryptographer^ AuthCrypt;
	Socket^ InnerClient;
	void SendAuth(void* Addr, int Count);
};
