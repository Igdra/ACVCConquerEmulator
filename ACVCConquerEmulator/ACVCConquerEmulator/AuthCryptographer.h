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
