#include <iostream>

using namespace std;

using uchar = unsigned char;


void ALD(bool f0, bool f1, bool a, bool b, bool ena, bool enb, bool inva, bool inShift, bool& out, bool& outShift)
{
	bool resolLine1 = !f0 && !f1;
	bool resolLine2 = !f0 && f1;
	bool resolLine3 = f0 && !f1;
	bool resolLine4 = f0 && f1;
	bool inA = inva ^ (a && ena);
	bool inB = b && enb;
	bool aAndB = inA && inB;
	bool aAndBResolve = aAndB && resolLine1;
	bool aOrB = inA || inB;
	bool aOrBResolve = aOrB && resolLine2;
	bool notB = !inB;
	bool notBResolve = notB && resolLine3;
	bool aXorB = inA ^ inB;
	bool aXorBXorInShift = aXorB ^ inShift;
	bool aXorBXorInShiftResolve = aXorBXorInShift && resolLine4;
	bool aPlusBResolve = aAndB && resolLine4;
	bool aXorBAndInShiftResolve = aXorB && inShift && resolLine4;

	outShift = (aPlusBResolve || aXorBAndInShiftResolve);

	out = aAndBResolve || aOrBResolve || notBResolve || aXorBXorInShiftResolve;
}

uchar OR(uchar _right, uchar _left)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(0, 1, _right & 1, _left & 1, 1, 1, 0, 0, out, shift);
		res |= (out << i);
		_right >>= 1;
		_left >>= 1;
	}
	return res;
}

uchar AND(uchar _right, uchar _left)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(0, 0, _right & 1, _left & 1, 1, 1, 0, 0, out, shift);
		res |= (out << i);
		_right >>= 1;
		_left >>= 1;
	}
	return res;
}

uchar NOT(uchar _right)
{
	uchar res = 0;

	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool shift = 0;

		ALD(1, 0, _right & 1, _right & 1, 1, 1, 0, 0, out, shift);
		res |= (out << i);
		_right >>= 1;
	}
	return res;
}

uchar PLUS(uchar _right, uchar _left)
{
	uchar res = 0;

	bool inShift = 0;
	for (int i = 0; i < 8; i++)
	{
		bool out = 0;
		bool outShift;

		ALD(1, 1, _right & 1, _left & 1, 1, 1, 0, inShift, out, outShift);
		res |= (out << i);
		_right >>= 1;
		_left >>= 1;
		inShift = outShift;
	}
	return res;
}

int main()
{
	uchar res = PLUS(5, 2);
	cout << res << endl;
	return 0;
}