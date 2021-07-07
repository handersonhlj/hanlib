#include<stdio.h>
#include<stdlib.h>


/* 双向链表的结构 */
typedef struct List{
	char cmd[16];
    int pos;		// 	数据域
	struct List *next;		//  向后的指针
	struct List *front;		//  向前的指针
};

typedef struct List* pElem;
typedef struct List eElem;

typedef struct 
{
    LenCmd *head;
    LenCmd *tail;
}LenQueue;


typedef struct {

	int arr[SIZE];
	int head; //记录最前面数字所在的下标
	int tail; //记录最后一个有效数字的下一个坐标
			  //如果队列里一个数都没有的话head=tail
} Queue;

//队列的初始化函数
void queue_init(Queue *p_queue) {
	p_queue->head = 0;
	p_queue->tail = 0;
}

//队列清理函数
void queue_deinit(Queue *p_queue) {
	p_queue->head = 0;
	p_queue->head = 0;
}

//计算数字个数
int queue_size(const Queue *p_queue) {
	return (p_queue->tail - p_queue->head);
}

//判断队列是否为空
int queue_empty(const Queue *p_queue) {
	return !(p_queue->tail - p_queue->head);
}

//判断队列是否满的
int queue_full(const Queue *p_queue) {
	return p_queue->tail >= SIZE;//tail当吧最后一个SIZE-1使用后变为SIZE，为保险要大于
}

//向队列里加入数字
int queue_push(Queue *p_queue, int val) {
	if (queue_full(p_queue)) {
		return 0;
	}
	else {
		p_queue->arr[p_queue->tail] = val;
		p_queue->tail++;
		return 1;//表示将数字加进去了
	}
}

//从队列里获得数字的（会把数字从队列里删除）
int queue_pop(Queue *p_queue, int *p_num) {
	if (queue_empty(p_queue)) {
		return 0;
	}
	else {
		*p_num = p_queue->arr[p_queue->head];//因为要删除，所以先给
		p_queue->head++;//将取过的数跳过去
		return 1;
	}
}

//从队列里获得数字（不会把数字从队列删除）
int queue_front(const Queue *p_queue, int *p_num) {
	if (queue_empty(p_queue)) {
		return 0;
	}
	else {
		*p_num = p_queue->arr[p_queue->head];//多次调用是同一个数
		return 1;
	}
}

void main() {
	Queue qu;
	int y;
	queue_init(&qu);
	queue_push(&qu, 1);
	queue_push(&qu, 2);
	queue_push(&qu, 3);
	queue_push(&qu, 4);

	

	printf(">>>%d\r\n", queue_size(&qu));
       // 拿出一个数据，数据还在
	queue_front(&qu, &y);
	printf(">%d\r\n", y);
	printf(">>%d\r\n", queue_size(&qu));
       // 取出一个数据，数据已经被删除
	queue_pop(&qu, &y);
	printf(">%d\r\n", y);
	printf(">>%d\r\n", queue_size(&qu));
       // 插入一个数据
	queue_push(&qu, 5);

	queue_pop(&qu, &y);
	printf(">%d\r\n", y);
	printf(">>%d\r\n", queue_size(&qu));

//	while (qu.tail != qu.head) {
//		qu.head = qu.head + 1;
//		printf("当前队列值=%d\n", qu.arr[qu.head]);
//	}
}
