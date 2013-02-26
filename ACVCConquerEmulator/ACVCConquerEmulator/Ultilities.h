#pragma once

public ref class Ultilities sealed abstract
{
public:
	static Random^ RGen = gcnew Random();
	static unsigned int RandomUInt32();
};
