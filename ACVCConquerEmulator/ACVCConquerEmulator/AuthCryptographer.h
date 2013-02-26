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
#pragma once



ref class Counter
	{
	private:
		unsigned short KeyCounter; // = 0;
	public:
		Counter(void)
		{
			KeyCounter = 0;
		}
		unsigned char FirstKey()
		{
			return (unsigned char)(KeyCounter & 0xFF);
		}
		unsigned char SecondKey()
		{
			return (unsigned char)(KeyCounter >> 8);
		}
		unsigned short Increment(unsigned short Val)
		{
			KeyCounter += Val;
			return KeyCounter;
		}
	};


ref class AuthCryptographer
{
public:
	AuthCryptographer(void);
	void Increment(unsigned short Val);
	Counter^ EncryptCounter;
	Counter^ DecryptCounter;
	array<unsigned char>^ FirstKey;
	array<unsigned char>^ SecondKey;
	void Encode(array<unsigned char>^ *buffer);
	void Decode(array<unsigned char>^ *buffer);
};
