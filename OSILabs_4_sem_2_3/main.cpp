#include <iostream>
#include <string>

using namespace std;

void readArgs(int argc, char* argv[], bool& F0, bool& F1, bool& A, bool& B, bool& ENA, bool& ENB)
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "-f0")
		{
			i++;
			F0 = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-f1")
		{
			i++;
			F1 = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-a")
		{
			i++;
			A = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-b")
		{
			i++;
			B = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-ena")
		{
			i++;
			ENA = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-enb")
		{
			i++;
			ENB = stoi(argv[i]);
		}
	}
}

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

int main(int argc, char* argv[])
{
	if (argc < 13)
	{
		cout << "Wrong args" << endl;
		return -1;
	}

	bool f0, f1, a, b, ena, enb;

	readArgs(argc, argv, f0, f1, a, b, ena, enb);


	return 0;
}