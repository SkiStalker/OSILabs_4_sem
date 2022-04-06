#include <iostream>

using namespace std;

using uchar = unsigned char;


void ALD(bool f0, bool f1, bool a, bool b, bool ena, bool enb, bool inva, bool inShift, bool& out, bool& outShift)
{
	inline bool resolLine1 = !f0 && !f1;
	inline bool resolLine2 = !f0 && f1;
	inline bool resolLine3 = f0 && !f1;
	inline bool resolLine4 = f0 && f1;
	inline bool inA = inva ^ (a && ena);
	inline bool inB = b && enb;
	inline bool aAndB = inA && inB;
	inline bool aAndBResolve = aAndB && resolLine1;
	inline bool aOrB = inA || inB;
	inline bool aOrBResolve = aOrB && resolLine2;
	inline bool notB = !inB;
	inline bool notBResolve = notB && resolLine3;
	inline bool aXorB = inA ^ inB;
	inline bool aXorBXorInShift = aXorB ^ inShift;
	inline bool aXorBXorInShiftResolve = aXorBXorInShift && resolLine4;
	inline bool aPlusBResolve = aAndB && resolLine4;
	inline bool aXorBResolve = aXorBXorInShift && resolLine4;

	outShift = (aXorBXorInShiftResolve || aXorBResolve);

	out = aAndBResolve || aOrBResolve || notBResolve || aXorBXorInShiftResolve;
}

unsigned char OR(uchar _right, uchar _left)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(0, 1, _right & 1, _left & 1, 1, 1, 0, 0, out, shift);
		res |= out;
		if (i < 7)
		{
			res <<= 1;
			_right <<= 1;
			_left <<= 1;
		}
	}
	return res;
}

unsigned char AND(uchar _right, uchar _left)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(0, 0, _right & 1, _left & 1, 1, 1, 0, 0, out, shift);
		res |= out;
		if (i < 7)
		{
			res <<= 1;
			_right <<= 1;
			_left <<= 1;
		}
	}
	return res;
}

unsigned char NOT(uchar _right)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(1, 0, _right & 1, _right & 1, 1, 1, 0, 0, out, shift);
		res |= out;
		if (i < 7)
		{
			res <<= 1;
			_right <<= 1;
		}
	}
	return res;
}

unsigned char PLUS(uchar _right, uchar _left)
{
	uchar res = 0;

	bool inShift = 0;
	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool outShift;

		ALD(1, 1, _right & 1, _left & 1, 1, 1, 0, inShift, out, outShift);
		res |= out;
		if (i < 7)
		{
			res <<= 1;
			_right <<= 1;
			_left <<= 1;
		}
		inShift = outShift;
	}
	return res;
}

int main()
{

    return 0;
}