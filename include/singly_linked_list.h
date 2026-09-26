#pragma once
// 单链表的功能函数注册文件
#include <stdbool.h>

typedef int ElemType;

typedef struct Node
{
	ElemType data;
	struct Node* next;
} Nodes;

// 创建单链表
Nodes* InitList(void);

// 判断链表是否为空
int IsEmpty(Nodes* head);

// 获取有效节点的长度
int GetLength(Nodes* head);

// 销毁链表,要释放所有节点的内存
void DestoryList(Nodes** head);

// 清空链表，保留头节点，释放数据节点
void ClearList(Nodes* head);

// 头插法,在链表的头部插入新节点
bool InsertAtHead(Nodes* head, ElemType value);

// 尾插法，在链表尾部插入新节点
bool InsertAtEnd(Nodes* head, ElemType value);

// 指定位置插入，pos为插入点
bool InsertAtPosition(Nodes* head, int pos, ElemType value);

// 指定位置删除节点,pos为删除点，outValue为被删数据
bool DeleteAtPosition(Nodes* head, int pos, ElemType *outValue);

// 按值删除:删除链表中第一个值为value的节点
int DeleteByValue(Nodes* head, ElemType value);

// 按序查找节点
Nodes* FindByIndex(Nodes* head, int index);

// 查找节点，查找第一个值为 value 的节点，返回节点指针（找不到返回 NULL）
Nodes* Find(Nodes* head, ElemType value);

// 修改指定位置的节点的值
int UpdateAtPosition(Nodes* head, int pos, ElemType newValue);

// 打印/遍历链表
void PrintList(Nodes* head);

// 原地反转链表
void ReverseList(Nodes* head);

// 合并两个已排序链表，返回新链表头节点
Nodes* MergeSortedLists(Nodes* head1, Nodes* head2);
	
// 检查链表是否成环（适用于无头节点的纯指针检测，1 表示有环）
int HasCycle(Nodes* head);
