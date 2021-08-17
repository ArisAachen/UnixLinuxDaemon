//
// Created by aris on 2021/7/31.
//

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void get_host_by_name_test() {
    struct hostent *ent;
    ent = gethostbyname("www.baidu.com");
    if (ent == NULL) {
        printf("parse host by name failed, err: %s \n", strerror(errno));
        return;
    }

    printf("parse name is %s \n", ent->h_name);

    for (int index = 0; ent->h_aliases[index]; index++) {
        printf("alias name is %s \n", ent->h_aliases[index]);
    }

    char *buf;
    for (int index = 0; ent->h_addr_list[index]; index++) {
        printf("type is %d \n", ent->h_addrtype);
        const char *tmp = inet_ntop(ent->h_addrtype, ent->h_addr_list[index], buf, INET_ADDRSTRLEN);
        if (tmp == NULL) {
            printf("tmp failed, err: %s \n", strerror(errno));
            continue;
        }
        printf("alias name is %s \n", buf);
    }
}

int main() {
    get_host_by_name_test();
    return 1;
}