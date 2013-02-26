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
		(*buffer)[i] ^= (unsigned char) ( FirstKey[DecryptCounter->FirstKey()] ^  
			SecondKey[DecryptCounter->SecondKey()]  );
		DecryptCounter->Increment(1);
	}
}