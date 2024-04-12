// demostrates the use of stream socket for IPC on the same machine

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

const char socket_path[] = "tmp/tmp1";
const int SIZE=30;

void CreateSocket(struct sockaddr_un &addr, int &fd)
{
    memset(&addr, 0, sizeof(addr));
    if( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
    {
        std::cerr << "SERVER: CreateSocket() failed...\n";
        exit(EXIT_FAILURE);
    }
}

void SetSocketPath(struct sockaddr_un &addr)
{
    addr.sun_family = AF_UNIX;
    strncpy( addr.sun_path, socket_path, sizeof(addr.sun_path)-1 );
    unlink(socket_path);
}

void BindSocket(struct sockaddr_un &addr, int &fd)
{
    if( bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 )
    {
        std::cerr << "SERVER: BindSocket() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void Listen(int &fd)
{
    if( listen(fd, 5) < 0 )
    {
        std::cerr << "SERVER: Listen() failed...\n";
        unlink(socket_path);
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void Accept(int &cl, int &fd)
{
    if( (cl = accept(fd, NULL, NULL)) < 0 )
    {
        std::cerr << "SERVER: Accept() failed...\n";
        unlink(socket_path);
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void Read(int &cl, int &rc, int &fd, char buffer[])
{
    rc = read(cl, buffer, SIZE);
    if(rc < 0)
    {
        std::cerr << "SERVER: Read() failed...\n";
        unlink(socket_path);
        close(fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server received -> " << buffer << std::endl;
}

void Write(int &cl, int &rc, int &fd, char buffer[])
{
    char new_buffer[SIZE];
    
    memset(new_buffer, 0, SIZE);
    
    for(int i=0; i<strlen(buffer); i++)
    {
        new_buffer[i] = toupper(buffer[i]);
    }

    rc = write(cl, new_buffer, strlen(new_buffer));
    if(rc < 0)
    {
        std::cerr << "SERVER: Write() failed...\n";
        unlink(socket_path);
        close(fd);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;            // unix socket address
    char buffer[SIZE];
    int fd;
    int cl;
    int rc;
    bool is_running = true;

    CreateSocket(addr, fd);
    SetSocketPath(addr);
    BindSocket(addr, fd);
    Listen(fd);
    Accept(cl, fd);
    Read(cl, rc, fd, buffer);
    Write(cl, rc, fd, buffer);

    unlink(socket_path);
    close(fd);
    close(cl);
    return EXIT_SUCCESS;
}