#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>



int main()
{
    int fd;
    fd = open("/dev/console", O_RDWR);
    ioctl(fd, KDSETLED, 7);
    return 0;
}