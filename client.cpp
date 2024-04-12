// demostrates the use of stream socket for IPC on the same machine

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

const char socket_path[] = "tmp/tmp1";
const int SIZE=30;

char buffer[] = "this sentence looks ok";

void CreateSocket(int &rc, int &fd, sockaddr_un &addr)
{
    memset(&addr, 0, sizeof(addr));
    if( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
    {
        std::cerr << "CLIENT: CreateSocket() failed...\n";
        exit(EXIT_FAILURE);
    }
}

void SetSocketPath(struct sockaddr_un &addr)
{
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) -1);
}

void ClientConnect(int &fd, struct sockaddr_un &addr)
{
    if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 )
    {
        std::cerr << "CLIENT: ClientConnect() failed...\n";
        exit(EXIT_FAILURE);       
    }
}

void Write(int &rc, int &fd)
{
    int length = strlen(buffer) + 1;
    rc = write(fd, buffer, length);
    if(rc < 0)
    {
        std::cerr << "CLIENT: Write() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);           
    }
}

void Read(int &rc, int &fd)
{
    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer));
    rc = read(fd, buffer, SIZE);
    if(rc < 0)
    {
        std::cerr << "CLIENT: Read() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);           
    }

    std::cout << "Client received -> " << buffer << std::endl;
}

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int fd;
    int rc;

    CreateSocket(rc, fd, addr);
    SetSocketPath(addr);
    ClientConnect(fd, addr);
    Write(rc, fd);
    Read(rc, fd);

    close(fd);
}