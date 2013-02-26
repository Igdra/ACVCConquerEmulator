#include "StdAfx.h"
#include "Ultilities.h"

unsigned int Ultilities::RandomUInt32()
{
	return (unsigned int)Ultilities::RGen->Next(0, int::MaxValue);
}