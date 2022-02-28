#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <string>
#include <map>
#include <vector>
#include <ctype.h>

using namespace std;

map<char, vector < int>> alph =
{
{'a', {0, 1}},
{'b', {1, 0, 0, 0}},
{'c', {1, 0, 1, 0}},
{'d', {1, 0, 0}},
{'e', {0}},
{'f', {0, 0, 1, 0}},
{'g', {1, 1, 0}},
{'h', {0, 0, 0, 0}},
{'i', {0, 0}},
{'j', {0, 1, 1, 1}},
{'k', {1, 0, 1}},
{'l', {0, 1, 0, 0}},
{'m', {1, 1}},
{'n', {1, 0}},
{'o', {1, 1, 1}},
{'p', {0, 1, 1, 0}},
{'q', {1, 1, 0, 1}},
{'r', {0, 1, 0}},
{'s', {0, 0 ,0}},
{'t', {1}},
{'u', {0, 0, 1}},
{'v', {0, 0, 0, 1}},
{'w', {0, 1, 1}},
{'x', {1, 0, 0 ,1}},
{'y', {1, 0, 1, 1}},
{'z', {1, 1, 0, 0}},
{'0', {1, 1, 1, 1, 1}},
{'1', {0, 1, 1, 1, 1}},
{'2', {0, 0, 1, 1, 1}},
{'3', {0, 0, 0, 1, 1}},
{'4', {0, 0, 0, 0, 1}},
{'5', {0, 0, 0, 0, 0}},
{'6', {1, 0, 0, 0, 0}},
{'7', {1, 1, 0, 0, 0}},
{'8', {1, 1, 1, 0, 0}},
{'9', {1, 1, 1, 1, 0}}
};


void showMorse(int fd, const string& str)
{
	int delay = 1 * 1000000;
	for (char c : str)
	{
		for (int signal : alph[tolower(c)])
		{
			ioctl(fd, KDSETLED, 4);
			if (signal)
			{
				usleep(0.5 * delay);
			}
			else
			{
				usleep(0.25 * delay);
			}
			ioctl(fd, KDSETLED, 0);
			usleep(0.125 * delay);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		return -1;

	string str(argv[1]);

	int fd;
	fd = open("/dev/console", O_RDWR);
	ioctl(fd, KDSETLED, 0);
	cout << "Input word - " << str << endl;
	showMorse(fd, str);
	cout << "Finished" << endl;
	return 0;
}