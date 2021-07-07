#ifndef _LINKLIST_H_
#define _LINKLIST_H_
#define MAX_LENGTH 1024*1024*2

typedef struct LNode
{
    unsigned char data[MAX_LENGTH]; //数据域
    struct LNode *next; //指向下一个节点的指针
    unsigned int length;
} LNode, *LinkList;

void creatList(void);
void insertListBuf(unsigned char *data,int length);
unsigned int getListLength();
int getListData(unsigned char *buf);
LinkList getFirstData(void);
void delFirstData(void);

#endif
