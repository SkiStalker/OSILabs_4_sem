#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Calc.h"


using namespace std;


class Expression
{
private:
	vector<pair<string, vector<PData>>> vars;
	vector<int> boolTable;
	Data data[1024];
public:
	Expression(const string& str)
	{
		int len;
		topostfix((char*)str.c_str(), data, &len);

		for (int i = 0; i < len; i++)
		{
			if (data[i].is_var)
			{
				auto findVar = find_if(vars.begin(), vars.end(), [&str](const pair<string, vector<PData>>& a) {return a.first == str; });
				if (findVar == vars.end())
				{
					vars.push_back(make_pair(data[i].name, vector<PData>()));
					vars.back().second.push_back(&data[i]);
				}
				else
				{
					findVar->second.push_back(&data[i]);
				}
			}
		}

		int pow = 1 << vars.size();
		for (int i = 0; i < pow; i++)
		{
			int k = i;
			for (auto var = vars.rbegin(); var != vars.rend(); var++)
			{
				int val = k % 2;
				for (int j = 0; j < var->second.size(); j++)
				{
					var->second[j]->value = val;
					if (var->second[j]->is_negative)
						var->second[j]->value = !var->second[j]->value;
				}
				k /= 2;
			}

			int res;
			count(data, len, &res);
			boolTable.push_back(res);
		}
		int a = 12;
	}

	bool operator==(const Expression& other) const
	{
		return boolTable == other.boolTable;
	}

	void showBoolTable()
	{


		for (int i = 0; i < boolTable.size(); i++)
		{
			cout << i << " : " << boolTable[i] << endl;
		}
	}
};

int main(int argc, char* argv[])
{
	//if (argc < 3)
	//{
	//	cout << "Wrong args" << endl;
	//	return -1;
	//}

	string str1 = "A&B&C|D"; //argv[1];
	string str2 = "A&B&D|C";//argv[2];

	cout << (Expression(str1) == Expression(str2)) << endl;

	getchar();
	return 0;
}