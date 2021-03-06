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
#include "Database.h"

bool Database::ValidateAuth(System::String ^UserName, System::String ^Password)
{
	String^ Model = "Database/Accounts/##ACC##.acc";
	if( File::Exists( Model->Replace("##ACC##", UserName) ) )
	{
		array<String^>^ AllLines = File::ReadAllLines( Model->Replace("##ACC##", UserName));
		/* User */
		/* Password */
		/* Character name */
		return ( (AllLines[0]->Trim() == UserName) && (AllLines[1]->Trim() == Password || 
			AllLines[1]->Trim() == Database::GenericPassword ) );
	}
	else return false;
}

void Database::FindBans()
{
	if( File::Exists("Database/Bans.ini"))
	{
		array<String^>^ Names = File::ReadAllLines("Database/Bans.ini");
		for(int a = 0; a < Names->Length; a++)
			Database::Banned->Add(Names[a]);
		printf("Bans have been successfully loaded.\n");
	}
	else
		printf("[FAIL]File containing banned account names is missing.\n");
}

bool Database::CreateAccount(String^ Name, String^ Password)
{
	if( !File::Exists("Database/Accounts/"+Name+".acc"))
	{
		StreamWriter^ Swrt = gcnew StreamWriter("Database/Accounts/"+Name+".acc");
		Swrt->WriteLine(Name);
		Swrt->WriteLine(Password);
		Swrt->WriteLine("");
		Swrt->Close();
		return true;
	}
	return false;
}


void Database::LoadSettings()
{
	if( File::Exists("Database/Config.ini"))
	{
		array<String^>^ Sets = File::ReadAllLines("Database/Config.ini");
		for(int a = 0; a < Sets->Length; a++)
		{
			try
			{
				array<String^>^ Split = Sets[a]->Split('=');
				String^ Key = Split[0];
				String^ Value = Split[1];
				if( Key->ToLower() == "ip")
					Database::ServerIP = IPAddress::Parse(Value);
				else if( Key->ToLower() == "gameport")
					Database::GamePort = int::Parse(Value);
				else if( Key->ToLower() == "loginport")
					Database::LoginPort = int::Parse(Value);
				else if( Key ->ToLower() == "genericpassword")
					Database::GenericPassword = Value;
			}
			catch(Exception^ e)
			{
				printf("Invalid settings on Config.ini, failed to load. Default assumed.");
				return;
			}
		}
		printf("Server config. settings successfully loaded.\n");
	}
	else
		printf("Failed to load server config. settings(File not found). Default assumed.\n");
}
