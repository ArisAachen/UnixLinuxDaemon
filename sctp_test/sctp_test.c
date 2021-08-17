//
// Created by aris on 2021/8/15.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>


void sctp_test() {

    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (sctp_bindx(fd, NULL, 0)) {

    }


}


int main() {
    return 1;
}