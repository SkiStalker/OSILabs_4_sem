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
	map<string, PData> vars;
	vector<int> boolTable;
	PData data = new Data[1024];
public:
	Expression(const string& str)
	{
		int len;
		topostfix((char*)str.c_str(), data, &len);

		for (int i = 0; i < len; i++)
		{
			if (vars.find(data->name) == vars.end())
			{
				vars[data->name] = data;
			}
		}

		for (int i = 0; i < (vars.size() << 1); i++)
		{
			int k = 0;
			for (auto& var : vars)
			{
				var.second->value = (i & (k << 1)) >> k;
				if (var.second->is_negative)
					var.second->value = !var.second->value;
				k++;
			}

			int res;
			count(data, len, &res);
			boolTable.push_back(res);
		}

	}

	bool operator==(const Expression& other) const
	{
		return boolTable == other.boolTable;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "Wrong args" << endl;
		return -1;
	}

	string str1 = argv[1];
	string str2 = argv[2];

	cout << (Expression(str1) == Expression(str2)) << endl;

	getchar();
	return 0;
}