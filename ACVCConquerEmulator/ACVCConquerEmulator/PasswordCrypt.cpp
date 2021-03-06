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


ref class PasswordCryptopgraphy
{
private:
	array<unsigned char>^ Key;
	static array<unsigned char>^ scanCodeToVirtualKeyMap =  
	{ 
            0, 0x1b, 0x31, 50, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0xbd, 0xbb, 8, 9, 
            0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4f, 80, 0xdb, 0xdd, 13, 0x11, 0x41, 0x53, 
            0x44, 70, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0xba, 0xc0, 0xdf, 0x10, 0xde, 90, 0x58, 0x43, 0x56, 
            0x42, 0x4e, 0x4d, 0xbc, 190, 0xbf, 0x10, 0x6a, 0x12, 0x20, 20, 0x70, 0x71, 0x72, 0x73, 0x74, 
            0x75, 0x76, 0x77, 120, 0x79, 0x90, 0x91, 0x24, 0x26, 0x21, 0x6d, 0x25, 12, 0x27, 0x6b, 0x23, 
            40, 0x22, 0x2d, 0x2e, 0x2c, 0, 220, 0x7a, 0x7b, 12, 0xee, 0xf1, 0xea, 0xf9, 0xf5, 0xf3, 
            0, 0, 0xfb, 0x2f, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 130, 0x83, 0x84, 0x85, 0x86, 0xed, 
            0, 0xe9, 0, 0xc1, 0, 0, 0x87, 0, 0, 0, 0, 0xeb, 9, 0, 0xc2, 0
	};
	static array<unsigned char>^ virtualKeyToScanCodeMap =
	{
		 0, 0, 0, 70, 0, 0, 0, 0, 14, 15, 0, 0, 0x4c, 0x1c, 0, 0, 
            0x2a, 0x1d, 0x38, 0, 0x3a, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
            0x39, 0x49, 0x51, 0x4f, 0x47, 0x4b, 0x48, 0x4d, 80, 0, 0, 0, 0x54, 0x52, 0x53, 0x63, 
            11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0, 
            0, 30, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 50, 0x31, 0x18, 
            0x19, 0x10, 0x13, 0x1f, 20, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c, 0x5b, 0x5c, 0x5d, 0, 0x5f, 
            0x52, 0x4f, 80, 0x51, 0x4b, 0x4c, 0x4d, 0x47, 0x48, 0x49, 0x37, 0x4e, 0, 0x4a, 0x53, 0x35, 
            0x3b, 60, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x57, 0x58, 100, 0x65, 0x66, 0x67, 
            0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 110, 0x76, 0, 0, 0, 0, 0, 0, 0, 0, 
            0x45, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0x2a, 0x36, 0x1d, 0x1d, 0x38, 0x38, 0x6a, 0x69, 0x67, 0x68, 0x65, 0x66, 50, 0x20, 0x2e, 0x30, 
            0x19, 0x10, 0x24, 0x22, 0x6c, 0x6d, 0x6b, 0x21, 0, 0, 0x27, 13, 0x33, 12, 0x34, 0x35, 
            40, 0x73, 0x7e, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1a, 0x56, 0x1b, 0x2b, 0x29, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0x71, 0x5c, 0x7b, 0, 0x6f, 90, 0, 
            0, 0x5b, 0, 0x5f, 0, 0x5e, 0, 0, 0, 0x5d, 0, 0x62, 0, 0, 0, 0
	};

public:
	PasswordCryptopgraphy(String^ UserName)
	{
		Key = gcnew array<unsigned char>(0x200);
		int seed = 0;
		for(int a = 0; a < UserName->Length; a++)
			seed += (int)UserName[a];
		msvcrt::msvcrt::srand(seed);
		array<unsigned char>^ buffer = gcnew array<unsigned char>(0x10);
		for(int i = 0; i < 0x10; i++)
			buffer[i] = (unsigned char)msvcrt::msvcrt::rand();

		for(int j = 1; j < 0x100; j++)
		{
			Key[j * 2] = (unsigned char)j;
			Key[2*j + 1] = (unsigned char)(j ^ buffer[j & 15]);
		}
		
		for(int k = 1; k < 0x100; k++)
		{
			for(int m = 1 + k; m < 0x100; m++)
			{
				if( Key[(k * 2) + 1] < Key[(m * 2) + 1])
				{
					 Key[k * 2] = (unsigned char)(Key[k * 2] ^ Key[m * 2]);
                     Key[m * 2] = (unsigned char)(Key[m * 2] ^ Key[k * 2]);
                     Key[k * 2] = (unsigned char)(Key[k * 2] ^ Key[m * 2]);
                     Key[(k * 2) + 1] = (unsigned char)(Key[(k * 2) + 1] ^ Key[(m * 2) + 1]);
                     Key[(m * 2) + 1] = (unsigned char)(Key[(m * 2) + 1] ^ Key[(k * 2) + 1]);
                     Key[(k * 2) + 1] = (unsigned char)(Key[(k * 2) + 1] ^ Key[(m * 2) + 1]);
				}
			}
		}
	}


	void Decrypt(array<unsigned char>^ *Data, int Length)
	{
		array<unsigned char>^ buffer = gcnew array<unsigned char>(Length);
		for(int i = 0; i < Length; i++)
		{
			bool flag = false;
			if( (*Data)[i] == 0)
				goto end;

			unsigned char index = (unsigned char)Key[ (*Data)[i] * 2];
			if( index >= 0x80)
			{
				index = (unsigned char)( Key[ ((*Data)[i] * 2)] - 0x80 );
				flag = true;
			}
			
			buffer[i] = (unsigned char)( buffer[i] + scanCodeToVirtualKeyMap[index] );
			if ((!flag && (buffer[i] >= 0x41)) && (buffer[i] <= 90))
				buffer[i] = (unsigned char)(buffer[i] + 0x20);
		}
end:
		*Data = buffer;
	}
};