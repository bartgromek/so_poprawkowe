#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main() {
    char buf[28];
    char msg[] = "No co tam kurwa";
    
    int fd = open("/dev/clipboard", O_RDWR,  "r+");

    int size = (int)strlen(msg);
    int id = write(fd, msg, size);
    size = read(fd, buf, 10);
    printf("size %d\n", size);
    buf[size] = '\0';
    printf("id: %d buf: %s\n", id, buf);
    
    close(fd);
    
    return 0;
}

