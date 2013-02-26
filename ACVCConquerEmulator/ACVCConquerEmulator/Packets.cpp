#include "stdafx.h"


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
