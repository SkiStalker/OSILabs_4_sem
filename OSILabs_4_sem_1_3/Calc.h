#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <string>
#include <sstream>

using namespace std;

typedef struct _Data
{
	string name = "";
	int value = -1;
	char operand = 0;
	int is_var = 0;
	bool is_negative = false;
} Data, * PData;


void removespace(char* str, int len);

int count(PData data, int len, int* res);

char* topostfix(char* str, PData res, int* len);