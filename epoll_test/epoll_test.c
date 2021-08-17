//
// Created by aris on 2021/7/29.
//

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

struct thread_arg {
    int fd;
};


static void *
work_thread(void *arg) {
    struct thread_arg *handler = (struct thread_arg *) arg;

    while (1) {
        struct epoll_event event;
        int fd = epoll_wait(handler->fd, &event, 5, -1);
        if (fd == -1)
            break;
        if (event.events & EPOLLHUP || event.events & EPOLLRDHUP) {
            printf("rec end event, close fd \n");
            if (!epoll_ctl(handler->fd, EPOLL_CTL_DEL, fd, NULL))
                printf("del fd failed \n");
            break;
        }
        if (event.events & EPOLLIN) {
            printf("rec in event \n");
        }
        if (event.events & EPOLLOUT) {
            printf("rec out event \n");
        }
    }
}

// epoll test
void epoll_test() {
    // create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket failed, err: %s \n", strerror(errno));
        return;
    }
    // set reuse addr
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        printf("set opt failed \n");
        return;
    }

    // create addr
    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof(struct sockaddr_storage));
    struct sockaddr_in *in = (struct sockaddr_in *) &storage;
    in->sin_family = AF_INET;
    in->sin_addr.s_addr = INADDR_ANY;
    in->sin_port = htons(8080);

    // create epoll
    int epoll_fd = epoll_create1(EPOLL_CLOEXEC);

    // listen event
    struct epoll_event event = {
            .events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP,
            .data = NULL,
    };
    // add event
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        printf("create add epoll failed, err: %s \n", strerror(errno));
        return;
    }
    // thread arg
    struct thread_arg arg = {
            .fd = epoll_fd,
    };

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, work_thread, &arg) == -1) {
        printf("create thread failed, err: %s \n", strerror(errno));
        return;
    }

    int result;
    pthread_join(thread_id, (void *) &result);
}

int main() {
    epoll_test();

    return 1;
}