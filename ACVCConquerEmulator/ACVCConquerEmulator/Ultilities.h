#pragma once

public ref class Ultilities sealed abstract
{
public:
	static Random^ RGen = gcnew Random();
	static unsigned int RandomUInt32();
	static String^ ReadString(array<unsigned char>^ buffer, int Offset, int Length)
	{
		return System::Text::Encoding::ASCII->GetString(buffer, Offset, Length);
	}
};
