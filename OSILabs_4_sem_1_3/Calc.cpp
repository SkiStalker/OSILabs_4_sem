#include "Calc.h"

#pragma warning (disable:4996)

using namespace std;

// Создание числа
void CreateVar(PData data, string name)
{
	data->name = name;
	data->is_var = 1;
}

// Создание операции
void CreateOperation(PData data, char val)
{
	data->operand = val;
	data->is_var = 0;
}

// Проверка на принадлежность к числам или опреациям
// (Костыль с тем что для операций число == -1, а для чисел операция)
int isAction(Data data)
{
	return !data.is_var;
}

// Парсер строки
int getData(char** str, PData data)
{
	char temp[1024];
	int ind = 0;
	// Если число читаем пока не станет не числом
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
		// Если опреация просто символ считываем
		if (**str == '(' || **str == ')' || **str == '&' || **str == '|' || **str == '!')
		{
			CreateOperation(data, **str);
			(*str)++;
			return 0;
		}
		else
		{
			// Если не операция ошибка
			return -1;
		}
	}
}

// Функция для получения приоритета операций
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


// Приведение в постфиксную форму
char* topostfix(char* str, PData res, int* len)
{
	int i = 0;

	// Флаг принадлежности к цифрам или опреациям предыдущей операции
	int flag = -1;

	// Указатель на стек потому что так надо
	stack<Data> _stack;

	Data temp;

	while (*str)
	{
		temp.name = "";
		temp.operand = 0;
		temp.is_var = 0;

		// Если парсер вернул ненулевой код, то это не число и не операция
		if (getData(&str, &temp))
		{
			return ++str;
		}

		// Просто переменная
		int resAct = isAction(temp);

		// Если это действие
		if (resAct)
		{
			// Если это начало строки
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
			// Если предыдущая операция тоже действие
			else if (flag == resAct)
			{
				if (temp.operand == '!')
				{
					getData(&str, &temp);
					// Сразу проверяем не операция ли дальше
					if (resAct = isAction(temp))
						return str;
					temp.is_negative = true;
					// Обновляем флаг
					flag = resAct;
				}
				// Если операция не скобки, то это ошибка
				else if (temp.operand != '(' && temp.operand != ')')
				{
					return str;
				}
			}
			// Если до этого было число и текущая не скобки обновляем флаг
			else if (temp.operand != '(' && temp.operand != ')')
			{
				flag = resAct;
			}
			// А если после числа стоит открывающая скобка, то ошибка
			else if (temp.operand == '(')
				return str;
		}
		else
		{
			// Если это число просто обновляем флаг состояния
			flag = resAct;
		}

		// Если "temp" число
		if (!resAct)
		{
			res[i++] = temp;
		}
		else
		{
			// Если нет
			if (temp.operand == '(')
			{
				_stack.push(temp);
			}
			else if (temp.operand == ')')
			{
				// Cмотрим вершину стека
				if (_stack.size())
				{
					temp = _stack.top();
					// Если сразу открывающая скобка - возврат
					if (*(str - 2) == '(')
						return str;
					// По алгоритму
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
					// Если пустой стэк, то непорядок
					return str;
				}
			}
			else
			{
				// По алгоритму выгружаем стек
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
	// Если что то в стеке осталось выгружаем
	if (!_stack.empty())
	{
		while (_stack.size())
		{
			temp = _stack.top();
			_stack.pop();
			// Скобок быть не должно
			if (temp.operand == '(')
				return str;
			res[i++] = temp;
		}
	}

	// Длинна нашего массива с элементами
	*len = i;
	while (!_stack.empty())
		_stack.pop();

	return 0;
}

// Простой калькулятор для удобства
int calc(int a, int b, char act)
{
	switch (act)
	{
	case '&': return a & b; break;
	case '|': return a | b;  break;
	}
	return -1;
}

// Процедура подсчета массива в постфиксной форме
// Просто перебор по алгоритму
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

// Удаление пробелов из строки
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


