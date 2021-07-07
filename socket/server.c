#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define portnumber 8000

int secondsocketfd;
int secondclientfd;
struct sockaddr_in secondclient_addr;
unsigned char tempbuf[64];

void secondSocketInit(void) {
    struct sockaddr_in server_addr;
    int sin_size;
    int nbytes;
    if ((secondsocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) ==
        -1) {  // AF_INET:IPV4;SOCK_STREAM:TCP
        fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
        exit(1);
    }
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;  // Internet
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portnumber);

    if (bind(secondsocketfd, (struct sockaddr *)(&server_addr),
             sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
        exit(1);
    }

    if (listen(secondsocketfd, 5) == -1)  //等待连接
    {
        fprintf(stderr, "Listen error:%s\n\a", strerror(errno));
        exit(1);
    }
}

void secondSocketProcess(void)
{
    FILE *f_pcm = NULL;
    char *p_pcm = NULL;
    unsigned long pcm_count = 0;
    unsigned long pcm_size = 0;
    unsigned long read_size = 0;
    f_pcm = fopen("wav/ddhgdw.pcm", "rb");
    if (NULL == f_pcm)
    {
        printf("\nopen audio file failed! \n");
    }

    fseek(f_pcm, 0, SEEK_END);
    pcm_size = ftell(f_pcm);
    fseek(f_pcm, 0, SEEK_SET);

    p_pcm = (char *)malloc(pcm_size);
    if (NULL == p_pcm)
    {
        printf("\nout of memory! \n");
    }

    read_size = fread((void *)p_pcm, 1, pcm_size, f_pcm);
    if (read_size != pcm_size)
    {
        printf("\nread  error!\n");
    }
    printf("read_size = %d\n",read_size);
    
    while (1)
    {
        int sin_size = sizeof(struct sockaddr_in);
        if ((secondclientfd = accept(secondsocketfd, (struct sockaddr *)(&secondclient_addr),
                                     &sin_size)) == -1)
        {
            fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
            break;
        }
        struct timeval timeout = {0, 100000}; // 3s
        int ret = setsockopt(secondclientfd, SOL_SOCKET, SO_RCVTIMEO,
                             (const char *)&timeout, sizeof(timeout));
        printf("accept %d fd\n",secondclientfd);
        while (1)
        {
            sleep(1);
            printf("start send\n");
            int ret = send(secondclientfd, p_pcm, read_size, 0);
            printf("ret = %d\n", ret);
            memset(tempbuf,0,64);
            int length = recv(secondclientfd,tempbuf,64,0);
            if(length > 0){
                // printf("buf1 = %x,buf2 = %x\n",tempbuf[0],tempbuf[1]);
                int len = tempbuf[2] << 8 | tempbuf[3];
                printf("len = %d\n",len);
                tempbuf[length]='\0';
                printf("text = %s\n",tempbuf+4);
            }
            sleep(10);
        }
        close(secondclientfd);
        secondclientfd = -1;
    }
    close(secondsocketfd);
    exit(0);
}
void main(void)
{
    secondSocketInit();
    secondSocketProcess();
}
