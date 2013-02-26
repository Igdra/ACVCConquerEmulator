#include "StdAfx.h"
#include "AuthCryptographer.h"

AuthCryptographer::AuthCryptographer(void)
{
	EncryptCounter = gcnew Counter();
    DecryptCounter = gcnew Counter();
	FirstKey = gcnew array<unsigned char>(0x100);
	SecondKey = gcnew array<unsigned char>(0x100);
	unsigned char FirstByte = 0x9D;
	unsigned char SecondByte = 0x62;
	for(int i = 0; i < 0x100; i++)
	{
		FirstKey[i] = FirstByte;
		SecondKey[i] = SecondByte;
		FirstByte = (unsigned char)((0x0F + (unsigned char)(FirstByte * 0xFA)) * FirstByte + 0x13);
        SecondByte = (unsigned char)((0x79 - (unsigned char)(SecondByte * 0x5C)) * SecondByte + 0x6D);
	}
}


void AuthCryptographer::Increment(unsigned short Val)
{
	EncryptCounter->Increment(Val);
}
void AuthCryptographer::Encode(array<unsigned char>^ *buffer)
{
	for(int i = 0; i < (*buffer)->Length; i++)
	{
		(*buffer)[i] ^= (unsigned char) 0xAB;
		(*buffer)[i] = (unsigned char) ( (*buffer)[i] >> 4 | (*buffer)[i] << 4 );
		(*buffer)[i] ^= (unsigned char) ( FirstKey[EncryptCounter->FirstKey()] ^  
			SecondKey[EncryptCounter->SecondKey()]  );
		EncryptCounter->Increment(1);
	}
}
void AuthCryptographer::Decode(array<unsigned char>^ *buffer)
{
	for(int i = 0; i < (*buffer)->Length; i++)
	{
		(*buffer)[i] ^= (unsigned char) 0xAB;
		(*buffer)[i] = (unsigned char) ( (*buffer)[i] >> 4 | (*buffer)[i] << 4 );
		(*buffer)[i] ^= (unsigned char) ( FirstKey[EncryptCounter->FirstKey()] ^  
			SecondKey[EncryptCounter->SecondKey()]  );
		DecryptCounter->Increment(1);
	}
}