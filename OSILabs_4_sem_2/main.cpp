#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <string>
#include <vector>
#include <ctype.h>

using namespace std;

vector<int> leds = { 7, 6,3, 2, 5, 4, 1, 0 };

int main(int argc, char* argv[])
{
	if (argc < 2)
		return -1;
	int fd;
	fd = open("/dev/console", O_RDWR);
	ioctl(fd, KDSETLED, 0);

	for (int i = 0; i < argv[1][0] - '0'; i++)
	{
		ioctl(fd, KDSETLED, leds[i]);
		usleep(1000000);
		ioctl(fd, KDSETLED, 0);
	}
	ioctl(fd, KDSETLED, 0);
	usleep(1000000);
	ioctl(fd, KDSETLED, 7);
	usleep(1000000);
	ioctl(fd, KDSETLED, 0);
	usleep(1000000);
	ioctl(fd, KDSETLED, 7);
	usleep(1000000);
	ioctl(fd, KDSETLED, 0);
	usleep(1000000);
	ioctl(fd, KDSETLED, 7);
	usleep(1000000);
	ioctl(fd, KDSETLED, 0);
	return 0;
}