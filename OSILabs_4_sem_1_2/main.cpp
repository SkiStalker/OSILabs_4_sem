#include <iostream>
#include <vector>

using namespace std;

class Block
{
public:
	vector<char> data;
};

struct Bit
{
	Bit(int val)
	{
		i = val;
	}
	unsigned i : 1;
};

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


void calcAddBits(vector<Bit>& arr, int addCount)
{
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
			ind += pow << 1;
		}
		if (cnt % 2)
			arr[pow] = 1;
	}
}

vector<char> cryptByHemming(vector<char> data, int szBlock)
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
			if (!(charDataNumber % 8))
			{
				dataPos++;
				charDataNumber = 0;
			}

			if (dataPos >= data.size())
			{
				while (bitCount < szBlock)
				{
					bits.back().push_back(0);
					bitCount++;
				}
			}
		}
	}

	int cnt = findAdditonalBitsCount(szBlock);

	vector<vector<Bit>> formattedBits;

	for (auto& t : bits)
	{
		formattedBits.push_back(vector<Bit>());
		int pow = 0;
		for (int i = 0, j = 0; i < t.size() + cnt; i++)
		{
			if (i == 1 << pow)
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
		calcAddBits(formattedBits.back(), cnt);
	}

	vector<char> res;

	int bitPos = 0;
	for (auto& t : formattedBits)
	{
		res.push_back(0);
		for (int i = 0 ; i < t.size(); i++)
		{
			res.back() << 1;
			res.back() |= t[i].i;
			if (bitPos == 8)
			{
				res.push_back(0);
				bitPos = 0;
			}
			else
			{
				bitPos++;
			}
		}
	}

	return res;
}


int main()
{
	auto res = cryptByHemming({ 1, 1, 1, 1 }, 13);
	getchar();
	getchar();
	return 0;
}

