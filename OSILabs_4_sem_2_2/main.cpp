#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <sys/wait.h>
#include <iomanip>
#include <numeric>

#define _DEBUG_

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

int tryEncryptByHemming(const vector<uchar>& data, int szBlock)
{
	int cnt = findAdditonalBitsCount(szBlock);
	vector<vector<Bit>> bits = readBits(data, szBlock + cnt + 1, false);
	int res = 0;

	for (auto& t : bits)
	{
		int pos = 0;
		auto pows = findChangeAddBit(t, cnt);

		pos = accumulate(pows.begin(), pows.end(), 0);

		if (pos)
		{
			if (!checkOdd(t))
			{
				res += 2;
			}
			else
			{
				res += 1;
			}
		}
	}
	return 0;
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

int checkFile(const string& name, int szBlock)
{
	ifstream in(name);
	vector<uchar> buf;
	readBytes(in, buf);
	int res = tryEncryptByHemming(buf, szBlock);
	in.close();
	return res;
}

void runDaemon(const int& szBlock, const string& dirName, const int& period, const string& logName)
{
	cout << "Daemon started" << endl;
	ofstream logFile(logName);

	if (!logFile.is_open())
	{
		std::cout << "Can not open log file" << endl;
		return;
	}

	while (true)
	{
		DIR* dir = opendir(dirName.c_str());

		if (!dir)
		{
			std::cout << "Error open dir" << endl;
			return;
		}

		dirent* f_cur = nullptr;

		while (f_cur = readdir(dir))
		{
			if (f_cur->d_type != DT_DIR)
			{
				if (int cnt = checkFile(dirName + "/" + f_cur->d_name, szBlock))
				{
					time_t t = time(0);
					tm* local = localtime(&t);
					logFile << "[" << put_time(local, "%c") << "]{" << dirName + "/" + f_cur->d_name << "} - " << cnt << endl;
				}
			}
		}
		closedir(dir);
		sleep(period);
	}

	cout << "Daemon finished" << endl;
}

void readArgs(int argc, char* argv[], int& szBlock, string& dirName, int& period, string& logName)
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "-n")
		{
			i++;
			szBlock = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-d")
		{
			i++;
			dirName = argv[i];
		}
		else if (string(argv[i]) == "-t")
		{
			i++;
			period = stoi(argv[i]);
		}
		else if (string(argv[i]) == "-l")
		{
			i++;
			logName = argv[i];
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 9)
	{
		cout << "Wrong args" << endl;
		return -1;
	}
	int szBlock;
	string dirName;
	string logName;
	int period;

	readArgs(argc, argv, szBlock, dirName, period, logName);
	int pid;


#ifndef _DEBUG_
	switch (pid = fork())
	{
	case 0:
	{
		setsid();
		runDaemon(szBlock, dirName, period, logName);
		return 0;
		break;
	}
	case -1:
	{
		cout << "Can not create deamon" << endl;
		return -1;
		break;
	}
	}
	cout << "Create daemon, id = " << pid << endl;
#else
	runDaemon(szBlock, dirName, period, logName);
#endif // _DEBUG_


	getchar();
	return 0;
}