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

ref class RC5
{
private:
	array<unsigned int>^ _bufKey;
	array<unsigned int>^ _bufSub;
public:
	static unsigned int LeftRotate(unsigned int dwVar, int dwOffset)
        {
            return (dwVar << (dwOffset & 0x1F) | dwVar >> 0x20 - (dwOffset & 0x1F));
        }
    static unsigned int RightRotate(unsigned int dwVar, int dwOffset)
        {
            return (dwVar >> (dwOffset & 0x1F) | dwVar << 0x20 - (dwOffset & 0x1F));
        }

	RC5( array<unsigned char>^ data)
	{
		_bufKey = gcnew array<unsigned int>(4);
		_bufSub = gcnew array<unsigned int>(26);
		if( data->Length != 16)
			throw gcnew Exception("Invalid data length. Must be 16 unsigned chars");

		unsigned int p32 = 0xB7E15163;
		unsigned int q32 = 0x61C88647;

		unsigned int offsetA = 0, offsetB = 0, A = 0, B = 0;
		for (int i = 0; i < 4; i++)
		{
			_bufKey[i] = (unsigned int)(data[i * 4] + (data[i * 4 + 1] << 8)
				+ (data[i * 4 + 2] << 16) + (data[i * 4 + 3] << 24));
		}
		_bufSub[0] = p32;
		for (int i = 1; i < 26; i++)
        {
			_bufSub[i] = _bufSub[i - 1] - q32;
        }
		for (int s = 1; s <= 78; s++)
            {
                _bufSub[offsetA] = LeftRotate(_bufSub[offsetA] + A + B, 3);
                A = _bufSub[offsetA];
                offsetA = (offsetA + 1) % 0x1A;
                _bufKey[offsetB] = LeftRotate(_bufKey[offsetB] + A + B, (int)(A + B));
                B = _bufKey[offsetB];
                offsetB = (offsetB + 1) % 4;
            }
	}
	array<unsigned char>^ Decrypt( array<unsigned char>^ data)
	{
				if (data->Length % 8 != 0) throw gcnew Exception("Invalid password length. Must be multiple of 8");
            int nLen = data->Length / 8 * 8;

            if (nLen <= 0) throw gcnew Exception("Invalid password length. Must be greater than 0 unsigned chars.");
            

			array<unsigned int>^ bufData = gcnew array<unsigned int>( (int)(data->Length / 4) );

            for (int i = 0; i < data->Length / 4; i++)
                bufData[i] = (unsigned int)(data[i * 4] + (data[i * 4 + 1] << 8) + (data[i * 4 + 2] << 16) + (data[i * 4 + 3] << 24));
            
			for (int i = 0; i < nLen / 8; i++)
            {
                unsigned int ld = bufData[2 * i];
                unsigned int rd = bufData[2 * i + 1];
                for (int j = 12; j >= 1; j--)
                {
                    rd = RightRotate((rd - _bufSub[2 * j + 1]), (int)ld) ^ ld;
                    ld = RightRotate((ld - _bufSub[2 * j]), (int)rd) ^ rd;
                }
                unsigned int B = rd - _bufSub[1];
                unsigned int A = ld - _bufSub[0];
                bufData[2 * i] = A;
                bufData[2 * i + 1] = B;
            }
            array<unsigned char>^ result = gcnew array<unsigned char>((int) (bufData->Length * 4));

            for (int i = 0; i < bufData->Length; i++)
            {
                result[i * 4] = (unsigned char)bufData[i];
                result[i * 4 + 1] = (unsigned char)(bufData[i] >> 8);
                result[i * 4 + 2] = (unsigned char)(bufData[i] >> 16);
                result[i * 4 + 3] = (unsigned char)(bufData[i] >> 24);
            }
            return result;
	}
};