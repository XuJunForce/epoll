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

void ToUpper(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main(){

    int lfd = socket(AF_INET, SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        return -1;
    }
    int flag = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR,&flag,sizeof(flag));
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(8888);
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        return -1;
    }
    ret = listen(lfd, 128);
    if(ret == -1){
        perror("listen");
        return -1;
    }

    //这里的ev 仅用来初始化lfd,使得能够enfp能够监听
    int epfd = epoll_create(100);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    int res = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(struct epoll_event);
    while(1){
        int num = epoll_wait(epfd, evs, size, -1);
        if(num <0){
            perror("epoll_wait");
            break;
        }
        else if(num == 0){
            continue;
        }
        else if(num > 0){
            for(int i = 0 ;i<num;i++){
                if(lfd == evs[i].data.fd){
                    struct sockaddr_in caddr;
                    socklen_t len = sizeof(caddr);
                    int cfd = accept(lfd, (struct sockaddr*)&caddr, &len);
                    ev.events = EPOLLIN;
                    ev.data.fd = cfd;
                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                    if(ret == -1){
                        perror("epoll_ctl_accept");
                        exit(-1);
                    }

                }

                else{
                    char buff[1024];
                    memset(buff, 0, sizeof(buff));
                    int len = recv(evs[i].data.fd, buff, sizeof(buff), 0);
                    if(len == 0){
                        printf("client disconnected\n");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, NULL);
                        continue;
                    }
                    else if(len < 0){
                        perror("server-client recv error");
                    }
                    else if( len > 0){
                        printf("client says:%s\n",buff);
                        ToUpper(buff);
                        send(evs->data.fd, buff,strlen(buff)+1, 0);
                    }
                }

            }
        }


    }






    return 0;
}