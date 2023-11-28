// tcp_server.c
// Simple TCP server for CS5600
// author: Hank Bao

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

const int BUFFER_SIZE = 1024;

void unblock_fd(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void completion_callback(union sigval sigval) {
    struct aiocb *req;
    int ret;

    // make sure the request is valid
    req = (struct aiocb *)sigval.sival_ptr;
    if (aio_error(req) == 0) {
        ret = aio_return(req);
        write(req->aio_fildes, req->aio_buf, ret);
    }

    close(req->aio_fildes);
    free(req->aio_buf);
    free(req);
}

void start_read_async(const char *filename, int client_fd) {
    struct aiocb *cb;
    int fd;

    cb = malloc(sizeof(struct aiocb));
    if (!cb) {
        perror("malloc");
        return;
    }

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        free(cb);
        return;
    }

    cb->aio_buf = malloc(BUFFER_SIZE);
    if (!cb->aio_buf) {
        perror("malloc");
        close(fd);
        free(cb);
        return;
    }

    memset(cb, 0, sizeof(struct aiocb));
    cb->aio_fildes = client_fd;  // set client_fd to send data back to client
    cb->aio_nbytes = BUFFER_SIZE;
    cb->aio_offset = 0;

    cb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    cb->aio_sigevent.sigev_notify_function = completion_callback;
    cb->aio_sigevent.sigev_notify_attributes = NULL;
    cb->aio_sigevent.sigev_value.sival_ptr = cb;

    if (aio_read(cb) == -1) {
        perror("aio_read");
        close(fd);
        free((void *)cb->aio_buf);
        free(cb);
        return;
    }
}

void handle_incoming_tcp(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    int read_bytes = read(client_fd, buffer, BUFFER_SIZE);

    if (read_bytes < 0) {
        perror("read");
        return;
    }

    // read the filename from request
    buffer[read_bytes] = '\0';
    start_read_async(buffer, client_fd);
}

int main(int argc, char *argv[]) {
    const int PORT = 2046;
    const int MAX_CONNECTIONS = 10;
    int server_fd, incoming_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    unblock_fd(server_fd);

    // Set server socket to allow multiple connections
    // int opt = 1;
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);

    while (1) {
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == server_fd) {
                    if ((incoming_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }

                    // unblock_fd(incoming_fd);

                    FD_SET(incoming_fd, &current_sockets);
                } else {
                    handle_incoming_tcp(i);
                    close(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }

    return 0;
}
