#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>

int DEBUG = 0;

using namespace std;

using uchar = unsigned char;

struct Bit
{
	Bit(int val)
	{
		i = val;
	}
	unsigned i : 1;
};

void printBits(const vector<vector<Bit>>& bits);

int findAdditonalBitsCount(int len)
{
	int res = 1;
	int pow = 1;
	while ((res << pow) < len + pow + 1)
	{
		pow++;
	}
	return pow;
}

vector<int> findChangeAddBit(const vector<Bit>& arr, int addCount)
{
	vector<int> res;
	for (int i = 0; i < addCount; i++)
	{
		int pow = 1 << i;
		int ind = pow;
		int cnt = 0;
		while (ind < arr.size())
		{
			for (int k = ind; k < ind + pow && k < arr.size(); k++)
			{
				cnt += arr[k].i;
			}
			ind += (pow << 1);
		}
		if (cnt % 2)
			res.push_back(pow);
	}
	return res;
}

vector<vector<Bit>> readBits(const vector<uchar>& data, int szBlock, bool filling)
{
	vector<vector<Bit>> bits;
	int dataPos = 0;
	int charDataNumber = 0;
	while (dataPos < data.size())
	{
		bits.push_back(vector<Bit>());
		int bitCount = 0;
		while (bitCount < szBlock)
		{
			bits.back().push_back((data[dataPos] & (1 << (7 - charDataNumber))) >> (7 - charDataNumber));
			bitCount++;
			charDataNumber++;
			if (charDataNumber == 8)
			{
				dataPos++;
				charDataNumber = 0;
			}

			if (filling && dataPos >= data.size())
			{
				while (bitCount < szBlock)
				{
					bits.back().push_back(0);
					bitCount++;
				}
			}
		}
		if (!filling && charDataNumber && dataPos == data.size() - 1)
			dataPos++;
	}
	return bits;
}

void printBits(const vector<vector<Bit>>& bits)
{
	for (auto& t : bits)
	{
		for (auto bit : t)
		{
			cout << bit.i << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < bits.back().size(); i++)
		cout << "-";
	cout << endl;
}

bool checkOdd(const vector<Bit>& bits)
{
	int res = 0;
	for (int i = 0; i < bits.size(); i++)
	{
		res += bits[i].i;
	}
	return res % 2;
}

vector<uchar> bitsToChar(const vector<vector<Bit>>& bits, bool filling)
{
	vector<uchar> res;
	int bitPos = 0;
	res.push_back(0);
	for (int k = 0; k < bits.size(); k++)
	{
		for (int i = 0; i < bits[k].size(); i++)
		{
			res.back() |= bits[k][i].i;
			if (bitPos == 7)
			{
				res.push_back(0);
				bitPos = 0;
			}
			else
			{
				bitPos++;
				res.back() <<= 1;
			}
		}
	}
	int bitsOff = (bits.size() * bits.back().size()) % 8;
	if (filling && bitsOff)
	{
		for (int i = 0; i < (8 - bitsOff) - 1; i++)
			res.back() <<= 1;
	}
	return res;
}

vector<uchar> encryptByHemming(const vector<uchar>& data, int szBlock)
{
	int cnt = findAdditonalBitsCount(szBlock);
	vector<vector<Bit>> bits = readBits(data, szBlock + cnt + 1, false);

	if (DEBUG)
	{
		cout << "Crypted bits:" << endl;
		printBits(bits);
	}

	for (int i = 0; i < bits.size(); i++)
	{
		int pos = 0;
		auto pows = findChangeAddBit(bits[i], cnt);

		pos = accumulate(pows.begin(), pows.end(), 0);

		if (pos)
		{
			cout << "Block " << i << " - ";
			if (!checkOdd(bits[i]))
			{
				cout << "Two error bits" << endl;
			}
			else
			{
				cout << "Error bit : " << pos << endl;
				bits[i][pos].i = !bits[i][pos].i;
			}
		}
	}

	vector<vector<Bit>> unformattedBits;

	for (auto& t : bits)
	{
		unformattedBits.push_back(vector<Bit>());
		int pow = 0;
		for (int i = 1; i < t.size(); i++)
		{
			if (i != (1 << pow))
			{
				unformattedBits.back().push_back(t[i]);
			}
			else
			{
				pow++;
			}
		}
	}

	if (DEBUG)
	{
		cout << "Unformatted bits:" << endl;
		printBits(unformattedBits);
	}

	auto res = bitsToChar(unformattedBits, false);

	if (szBlock % 8)
		res.pop_back();

	while (!res.back())
		res.pop_back();

	return res;
}

vector<uchar> cryptByHemming(const vector<uchar>& data, int szBlock)
{
	vector<vector<Bit>> bits = readBits(data, szBlock, true);

	if (DEBUG)
	{
		cout << "Original bits:" << endl;
		printBits(bits);
	}

	int cnt = findAdditonalBitsCount(szBlock);

	vector<vector<Bit>> formattedBits;

	for (auto& t : bits)
	{
		formattedBits.push_back(vector<Bit>());
		formattedBits.back().push_back(0);
		int pow = 0;
		for (int i = 0, j = 0; i < t.size() + cnt; i++)
		{
			if ((i + 1) == (1 << pow))
			{
				pow++;
				formattedBits.back().push_back(0);
			}
			else
			{
				formattedBits.back().push_back(t[j]);
				j++;
			}
		}
		for (auto pos : findChangeAddBit(formattedBits.back(), cnt))
		{
			formattedBits.back()[pos] = 1;
		}

		if (checkOdd(formattedBits.back()))
			formattedBits.back().front() = 1;
	}

	if (DEBUG)
	{
		cout << "Formatted bits:" << endl;
		printBits(formattedBits);
	}

	return bitsToChar(formattedBits, true);
}

void readArgs(int argc, char* argv[], int& szBlock, string& inName, string& outName, bool& crypt)
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "-n")
		{
			i++;
			szBlock = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-i")
		{
			i++;
			inName = argv[i];
		}
		else if (string(argv[i]) == "-o")
		{
			i++;
			outName = argv[i];
		}
		else if (string(argv[i]) == "-c")
		{
			i++;
			crypt = stoi(argv[i]);
		}
	}
}

void readBytes(ifstream& in, vector<uchar>& bytes)
{
	while (!in.eof())
	{
		char t = in.get();
		if (t != EOF)
		{
			bytes.push_back(t);
		}
	}
}

void writeBytes(ofstream& out, const vector<uchar> bytes)
{
	for (auto& byte : bytes)
	{
		out.put(byte);
	}
}

int run(const string& inName, const string& outName, const bool crypt, const int& szBlock)
{
	ifstream in(inName);

	if (!in.is_open())
	{
		cout << "File not exist" << endl;
		return -1;
	}

	ofstream out(outName);

	if (!out.is_open())
	{
		cout << "Can not open out file" << endl;
		return -1;
	}

	vector<uchar> buf;
	readBytes(in, buf);
	if (crypt)
	{
		writeBytes(out, cryptByHemming(buf, szBlock));
	}
	else
	{
		writeBytes(out, encryptByHemming(buf, szBlock));
	}
	in.close();
	out.close();
}

int main(int argc, char* argv[])
{

	if (DEBUG)
	{
		vector<uchar> buf;
		ifstream in("in.txt");
		readBytes(in, buf);

		auto res = cryptByHemming(buf, 13);
		res.front() -= 1;
		res[res.size() - 2] += 1;

		res = encryptByHemming(res, 13);


		getchar();
		in.close();
	}
	else
	{
		int szBlock;
		string outName;
		string inName;
		bool crypt;

		readArgs(argc, argv, szBlock, inName, outName, crypt);
		run(inName, outName, crypt, szBlock);
		
	}
	return 0;
}

