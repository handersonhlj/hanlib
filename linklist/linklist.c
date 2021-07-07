#include "linklist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

LinkList firstNode;
LinkList endNode;

unsigned long int listLength = 0;

static pthread_mutex_t mute;

void creatList(void) {
    firstNode = (LinkList)malloc(sizeof(LNode));
    memset(firstNode->data,0,MAX_LENGTH);
    firstNode->next = NULL;
    endNode = firstNode;
    pthread_mutex_init(&mute, NULL);
    printf("create linklist\n");
    listLength = 0;
}



unsigned int getListLength() {
    return listLength;
};

void emptyList(void) {
    LinkList p = firstNode->next;
    LinkList tmp;
    int i = 0;
    while (p != NULL) {
        tmp = p;
        p = p->next;
        free(tmp);
        tmp = NULL;
    }
    endNode = firstNode;
    firstNode->next = NULL;
    listLength = 0;
}

void insertListBuf(unsigned char *data,int len) {

    if (listLength > 1024) emptyList();

    LinkList dataP = (LinkList)malloc(sizeof(LNode));
    memcpy(dataP->data,data,len);
    dataP->length =len;
    dataP->next = NULL;

    endNode->next = dataP;
    endNode = dataP;

    pthread_mutex_lock(&mute);//lock
    listLength++;
    pthread_mutex_unlock(&mute);
    printf("insert buf = %x,len = %d\n",endNode,listLength);
    // for(int i=0;i<len;i++){
    //     printf("%2x ",dataP->data[i]);
    // }
    // printf("\n");

};

LinkList getFirstData(void){
    LinkList p = firstNode->next;
    if(firstNode == endNode || p == NULL || listLength < 2) return 0;
    printf("getFirstDatan= %X\n",p);
    return p;
}

void delFirstData(void){

    LinkList p = firstNode->next;
    if (firstNode == endNode || p == NULL || listLength < 2){
        // printf("p = %x\n",p);
        return -1;
    }
    printf("del first= %x len = %d\n",p,listLength);
    firstNode->next = p->next;
    free(p);
    p = NULL;

    pthread_mutex_lock(&mute);//lock
    listLength--;
    pthread_mutex_unlock(&mute);


}

int getListData(unsigned char *buf) {
    
    LinkList p = firstNode->next;
    int len = 0;
    if(firstNode == endNode || p == NULL) return -1;
    len = p->length;
    memcpy(buf,p->data,p->length);
    firstNode->next = p->next;
    free(p);
    p = NULL;
    listLength--;
    return len;
};

