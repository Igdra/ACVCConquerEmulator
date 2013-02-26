#include "stdafx.h"
#include "Client.h"

Client::Client(void)
{
}

void Client::SendAuth(void* Addr, int Count)
{
	array<unsigned char>^ Buffer = gcnew array<unsigned char>(Count);
	for(int a = 0; a < Count; a++)
	{
		Buffer[a] = *( ((unsigned char*)Addr) + a );
	}
	AuthCrypt->Encode(&Buffer);
	InnerClient->Send(Buffer);

}