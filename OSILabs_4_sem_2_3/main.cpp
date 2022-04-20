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