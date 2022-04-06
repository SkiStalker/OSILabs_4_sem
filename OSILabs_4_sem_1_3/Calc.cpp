#include "Calc.h"

#pragma warning (disable:4996)

using namespace std;

// �������� �����
void CreateVar(PData data, string name)
{
	data->name = name;
	data->is_var = 1;
}

// �������� ��������
void CreateOperation(PData data, char val)
{
	data->operand = val;
	data->is_var = 0;
}

// �������� �� �������������� � ������ ��� ���������
// (������� � ��� ��� ��� �������� ����� == -1, � ��� ����� ��������)
int isAction(Data data)
{
	return !data.is_var;
}

// ������ ������
int getData(char** str, PData data)
{
	char temp[1024];
	int ind = 0;
	// ���� ����� ������ ���� �� ������ �� ������
	if (isalpha(**str))
	{
		while (isalpha(**str))
		{
			temp[ind] = **str;
			ind++;
			(*str)++;
		}
		temp[ind] = 0;
		CreateVar(data, temp);
		return 0;
	}
	else
	{
		// ���� �������� ������ ������ ���������
		if (**str == '(' || **str == ')' || **str == '&' || **str == '|' || **str == '!')
		{
			CreateOperation(data, **str);
			(*str)++;
			return 0;
		}
		else
		{
			// ���� �� �������� ������
			return -1;
		}
	}
}

// ������� ��� ��������� ���������� ��������
int getPrior(char c)
{
	switch (c)
	{
	case '(': return 1; break;
	case ')': return 2; break;
	case '!': return 3; break;
	case '|': return 4; break;
	case '&':return 5; break;
	default: return -1;
	}
}


// ���������� � ����������� �����
char* topostfix(char* str, PData res, int* len)
{
	int i = 0;

	// ���� �������������� � ������ ��� ��������� ���������� ��������
	int flag = -1;

	// ��������� �� ���� ������ ��� ��� ����
	stack<Data> _stack;

	Data temp;

	while (*str)
	{
		temp.name = "";
		temp.operand = 0;
		temp.is_var = 0;

		// ���� ������ ������ ��������� ���, �� ��� �� ����� � �� ��������
		if (getData(&str, &temp))
		{
			return ++str;
		}

		// ������ ����������
		int resAct = isAction(temp);

		// ���� ��� ��������
		if (resAct)
		{
			// ���� ��� ������ ������
			if (flag == -1)
			{
				if (temp.operand != '(')
				{
					if (temp.operand == '!')
					{
						getData(&str, &temp);
						if (resAct = isAction(temp))
							return str;
						temp.is_negative = true;
						flag = resAct;
					}
					else
					{
						return str;
					}
				}
			}
			// ���� ���������� �������� ���� ��������
			else if (flag == resAct)
			{
				if (temp.operand == '!')
				{
					getData(&str, &temp);
					// ����� ��������� �� �������� �� ������
					if (resAct = isAction(temp))
						return str;
					temp.is_negative = true;
					// ��������� ����
					flag = resAct;
				}
				// ���� �������� �� ������, �� ��� ������
				else if (temp.operand != '(' && temp.operand != ')')
				{
					return str;
				}
			}
			// ���� �� ����� ���� ����� � ������� �� ������ ��������� ����
			else if (temp.operand != '(' && temp.operand != ')')
			{
				flag = resAct;
			}
			// � ���� ����� ����� ����� ����������� ������, �� ������
			else if (temp.operand == '(')
				return str;
		}
		else
		{
			// ���� ��� ����� ������ ��������� ���� ���������
			flag = resAct;
		}

		// ���� "temp" �����
		if (!resAct)
		{
			res[i++] = temp;
		}
		else
		{
			// ���� ���
			if (temp.operand == '(')
			{
				_stack.push(temp);
			}
			else if (temp.operand == ')')
			{
				// C������ ������� �����
				if (_stack.size())
				{
					temp = _stack.top();
					// ���� ����� ����������� ������ - �������
					if (*(str - 2) == '(')
						return str;
					// �� ���������
					while (temp.operand != '(')
					{
						temp = _stack.top();
						_stack.pop();
						res[i++] = temp;
						if (!_stack.size())
							return str;
						temp = _stack.top();
					}

					if (_stack.size())
						_stack.pop();
				}
				else
				{
					// ���� ������ ����, �� ���������
					return str;
				}
			}
			else
			{
				// �� ��������� ��������� ����
				int prior = getPrior(temp.operand);
				Data old = temp;
				if (_stack.size())
				{
					temp = _stack.top();
					while (!_stack.empty() && (prior <= getPrior(temp.operand)))
					{
						temp = _stack.top();
						_stack.pop();
						res[i++] = temp;
						if (!_stack.empty())
							temp = _stack.top();
					}
				}
				_stack.push(old);
			}
		}
	}
	// ���� ��� �� � ����� �������� ���������
	if (!_stack.empty())
	{
		while (_stack.size())
		{
			temp = _stack.top();
			_stack.pop();
			// ������ ���� �� ������
			if (temp.operand == '(')
				return str;
			res[i++] = temp;
		}
	}

	// ������ ������ ������� � ����������
	*len = i;
	while (!_stack.empty())
		_stack.pop();

	return 0;
}

// ������� ����������� ��� ��������
int calc(int a, int b, char act)
{
	switch (act)
	{
	case '&': return a & b; break;
	case '|': return a | b;  break;
	}
	return -1;
}

// ��������� �������� ������� � ����������� �����
// ������ ������� �� ���������
int count(PData data, int len, int* res)
{
	int i = 0;
	int a = 0, b = 0;
	stack<Data> _stack;
	while (i < len)
	{
		if (isAction(data[i]))
		{
			Data a;
			Data b;
			a = _stack.top();
			_stack.pop();
			b = _stack.top();
			_stack.pop();
			CreateVar(&a, a.name);
			a.value = calc(a.value, b.value, data[i++].operand);
			_stack.push(a);
		}
		else
		{
			_stack.push(data[i++]);
		}
	}
	Data _res = _stack.top();
	*res = _res.value;

	while (!_stack.empty())
		_stack.pop();
	return 0;
}

// �������� �������� �� ������
void removespace(char* str, int len)
{
	char temp[1024];
	int i = 0, ii = 0;
	for (i = 0; i < len; i++)
		if (str[i] != ' ')
			temp[ii++] = str[i];
	for (i = 0; i < ii; i++)
		str[i] = temp[i];
	str[i] = '\0';
}


