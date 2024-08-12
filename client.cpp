#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

int main(){

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("connet");
        return -1;
    }
    int number = 0;
    char buff[1024];
    memset(buff, 0, 1024);
    while(1){
        if(number == 20){
            sprintf(buff, "exit");
            exit(0);
        }
        sprintf(buff, "hello how are you %d",number++);
        send(fd, buff, sizeof(buff), 0);
        int num = recv(fd, buff, sizeof(buff), 0);
        if(num > 0){
            printf("server says:%s\n",buff);
        }
        else if(num == 0){
            printf("server disconnected\n");
        }
        else if(num< 0){
            perror("client could recv server");
            exit(-1);
        }
        sleep(1);
    }


    



    return 0;
}