#include "internet.h"
#include <time.h>
#include "timer_task.h"

/**
 * fd :
 * bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));//给服务器socket绑定地址结构（IP+port)
*/
void bind_t(int server_fd,int type,int port){
    int opt = 1;
    time_t now = time(NULL); // 获取当前时间
    struct tm *t = localtime(&now); // 将时间转换为本地时间

    SOCKETADDR_IN serv_addr = {  // 定义服务器地址结构 和 客户端地址结构
        .sin_addr.s_addr =  htonl(INADDR_ANY),
        .sin_family = type,
        .sin_port = htons(port)
    };  

    if(type == IPV4){
        printf("%d-%d-%d  %d:%d:%d  \033[33mINFO\033[0m \033[32m%d\033[0m --- [\t\t %s] \033[34m%s\033[0m\t:Socket file descriptor (%d) Communication IP type (IPV4) %s port (%d)\r\n",  \
        t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec, \
        getpid(),                                                                  \
        __func__,__FILE__                                                          \
        ,server_fd                                                                 \
        ,SERVER_NAME,PORT
        ); \

        
    }else if(type == IPV6){
        NET_LOG("fd = %dtype = IPV6 port = %d  ip = %d\r\n",server_fd,port,htonl(INADDR_ANY));
    }

    //端口复用
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));

    if(bind(server_fd, (SOCKETADDR *)&serv_addr, sizeof(serv_addr)) == -1){//给服务器socket绑定地址结构（IP+port)
        perror("\033[31mbind error\033[0m");
        exit(1);
    }
}

/**
 * domain ---> IPv6或者IPv4
 * type   ---> UDP或者TCP
 * return ---> socket创建的文件描述符
 * fd = socket(AF_INET, SOCK_STREAM, 0);		//创建一个 socket
*/
int socket_t(int domain, int type){
    int server_fd;
    time_t now = time(NULL); // 获取当前时间
    struct tm *t = localtime(&now); // 将时间转换为本地时间
    if((server_fd = socket(domain, type, 0)) == -1){
        perror("socket error\r\n");
        exit(1);
        return -1;
    }else{
        if(type == TCP){
            printf("%d-%d-%d  %d:%d:%d  \033[33mINFO\033[0m \033[32m%d\033[0m --- [\t       %s] \033[34m%s\033[0m\t:Socket file descriptor (%d) TCP socket create success\r\n",  \
            t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec, \
            getpid(),                                                                  \
            __func__,__FILE__                                                          \
            ,server_fd                                                                 \
            ); \
        }else if(type == UDP){
            NET_LOG("fd = %d UDP socket create success\r\n",server_fd);
        }
        return server_fd;
    }
}

/**
 * server_fd    文件描述符
*/
void listen_t(int server_fd){
    if(listen(server_fd,LISTEN_NUM) == -1){
        perror("listen error\r\n");
        exit(1);
    }
}



/**
 * server_fd    服务端文件描述符
 * return       客户端文件描述符
*/
int accept_t(int server_fd){
    socklen_t clit_addr_len =  sizeof(struct sockaddr);
    SOCKETADDR_IN 	clit_addr;
    int client_fd = 0;

    char client_IP[128];
    memset(client_IP,0,sizeof(client_IP));

    client_fd = accept(server_fd, (struct sockaddr *)&clit_addr, &clit_addr_len);

    if(client_fd == -1){
        printf("client ip:%s port:%d\r\n",inet_ntop(IPV4,&clit_addr.sin_addr.s_addr,client_IP,sizeof(client_IP)),ntohs(clit_addr.sin_port));
        perror("accept error:");
        exit(1);
        return -1;
    }else{
        #if 0
            NET_LOG("client ip:%s port:%d\r\n",inet_ntop(IPV4,&clit_addr.sin_addr.s_addr,client_IP,sizeof(client_IP)),ntohs(clit_addr.sin_port));
        #endif
        return client_fd;
    }

}

/**
 * socket连接初始化
 * demain IPV4与IPv6选择
 * type   连接方式  TCP,UDP
 * return 返回的socket文件描述符
*/
int socket_init(int domain, int type){
    int server_fd = socket_t(domain,type);
    bind_t(server_fd,domain,PORT);
    listen_t(server_fd);
    return server_fd;
}


/**
 * epoll初始化函数
 * epoll_def_init   event变量
 * server_fd        socket文件描述符
 * return           epoll_create文件描述符
*/
int epoll_init(EPOLL_DEF *epoll_def_init,int server_fd){
    int epfd=epoll_create(LISTEN_NUM);
    if(epfd == -1){
        perror("epoll_create error:");
        exit(1);
    }
    epoll_def_init->events=EPOLLIN;
    epoll_def_init->data.fd=server_fd;
    //添加socket文件描述符到树中
    epoll_add(epfd,server_fd,epoll_def_init);

    return epfd;
}


/**
 * epoll_add添加文件描述符
 * epfd           epoll_create文件描述符
 * fd             需要添加的文件描述符
 * epoll_def      event变量
*/
void epoll_add(int epfd,int fd,EPOLL_DEF *epoll_def){
    int ep_ctl = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,epoll_def);
     if(ep_ctl == -1){
        perror("epoll_ctl error:");
        exit(1);
    }
}


/**
 * epoll_delete删除文件描述符
 * epfd              epoll_create文件描述符
 * epoll_def         ep[i]变量
 * index             ep的下标
*/
void epoll_delete(int epfd,int clientfd){
    #if 1
        NET_LOG("epoll_delete %d\r\n",clientfd);
    #endif
    epoll_ctl(epfd,EPOLL_CTL_DEL,clientfd,NULL);
    close(clientfd);
   // delete_timer_task(clientfd);
}

/**
 * 添加accept的文件描述符
 * server_fd    socket文件描述符
 * epoll_def    event变量
 * epfd         epoll_create文件描述符
*/
void epoll_accpet(int server_fd,EPOLL_DEF *epoll_def,int epfd){
    int flag = 0;

    int client_fd = accept_t(server_fd);
    //设置io为非阻塞
    flag = fcntl(client_fd,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(client_fd,F_SETFL,flag);

    epoll_def->events=EPOLLIN | EPOLLET;
    epoll_def->data.fd=client_fd;
    epoll_add(epfd,client_fd,epoll_def);

    //添加任务定时
    //add_timer_task(client_fd);
}
