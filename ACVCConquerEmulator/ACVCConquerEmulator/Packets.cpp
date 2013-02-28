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
#include "Enums.h"

public ref class Packet
{
public:
	array<unsigned char>^ InnerPacket;
	Packet(int Size)
	{
		InnerPacket = gcnew array<unsigned char>(Size);
	}
	void Write( void* Data, int size, int Offset)
	{
		for(int a = Offset; a < Offset + size; a++)
		{
			InnerPacket[a] = *(((unsigned char*)Data) + (a - Offset) );
		}
	}
	void WriteString( String^ Data, int Offset)
	{
		array<unsigned char>^ StrToArray = Text::Encoding::ASCII->GetBytes(Data);
		for(int a = Offset; a < Offset + Data->Length; a++)
			InnerPacket[a] = StrToArray[a - Offset];
	}
	void WriteStamp()
	{
		WriteString("TQServer", InnerPacket->Length - 9);
	}
};


public ref class Packets sealed abstract
{
public:
		static array<unsigned char>^ AuthResponse(String^ IP, unsigned short Port,	unsigned int Token, AuthResponses Response)
{
	Packet^ P = gcnew Packet(52);
	unsigned short _Len = 52;
	unsigned short _Type = 1055;
	unsigned int _Token = Token;
	unsigned int _Response = (unsigned int)Response;
	unsigned int _Port = Port;
	unsigned short _A = 42508;
	unsigned short _B = 67;
	P->Write(&_Len, sizeof(_Len), 0);
	P->Write(&_Type, sizeof(_Type), 2);
	P->Write(&_Token, sizeof(_Token), 4);
	P->Write(&_Response, sizeof(_Response), 8);
	P->Write(&_Port, sizeof(_Port), 12);
	P->Write(&_A, sizeof(_A), 16);
    P->Write(&_B, sizeof(_B), 18);
	P->WriteString(IP, 20 );
	return P->InnerPacket;
}
};


public class PasswordSeed
	{
	public:
		unsigned short Length;
		unsigned short Type;
		unsigned int Seed;

	public:
		PasswordSeed(unsigned int seed)
		{
			Length = 8;
		    Type = 1059;
			Seed = seed;
		}
	};










