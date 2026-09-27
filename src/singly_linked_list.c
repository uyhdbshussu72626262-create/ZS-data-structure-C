#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include "singly_linked_list.h"


//
// 链表是存储在堆内存中的数据
// 单链表的函数实现文件

/*

*/

// 初始化链表，创建头节点
Nodes* InitList(void)
{
	Nodes* head = (Nodes*)malloc(sizeof(Nodes));

    if (head == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }

    head->next = NULL;
    head->data = 0;

    return head;
}

// 查看链表是否为空
int IsEmpty(Nodes* head)
{
    // 带头节点的单链表，
    // 判空标准应当是头节点的下一个指针是否指向空
    return (head->next == NULL);
}

// 获取有效节点的长度
int GetLength(Nodes* head)
{
    int length = 0;
    Nodes* p = head;
    while (p->next != NULL)
    {
        p = p->next;
        length++;
    }
    return length;
}
// 销毁链表,要释放所有节点的内存
/* ----核心方法----
1.	保存当前节点的下一个节点；
2.	释放当前节点；
3.	移动到下一个节点；
4.	最后把头指针设置为 NULL。
*/
void DestoryList(Nodes** head)
{
    // 头节点为空，直接结束函数
    if (head == NULL) return;
    Nodes* current = *head;
    while (current != NULL) {
        Nodes* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

// 清空链表，保留头节点，释放数据节点
void ClearList(Nodes* head)
{
    if (head == NULL) return;
    // current代指为头节点指向的数据节点
    Nodes* current = head->next;
    while (current != NULL) {
        // 
        Nodes* next = current->next;
        free(current);
        current = next;
    }
    // 头节点保留，不在指向已经释放的数据节点
    head->next = NULL;
}


// 头插法,在链表的头部插入新节点
// -------------------------------------
/*
    操作步骤：
    1.创建一个新节点
    2.将新节点的next指针指向当前的头节点
    3.更新链表的头指针，使其指向新节点

    特点：
    1.插入速度快（只需修改两个指针）。
    2.插入顺序与原始输入相反（后插入的元素排在前面
    ----------------------------------------
*/
bool InsertAtHead(Nodes* head, ElemType value)
{
    Nodes* newNode = (Nodes*)malloc(sizeof(Nodes));
    if (newNode == NULL) return false;

    newNode->data = value;
    newNode->next = head->next;
    head->next = newNode;
    
    return true;
}

// 尾插法，在链表尾部插入新节点
// -------------------------------------
/*
    操作步骤：
    1.创建一个新节点，newt指针设为空指针
    2.如果链表为空，直接让头指针指向新节点
    3.如果链表非空，找到最后一个节点，将它的 next 指针指向新节点。
*/
bool InsertAtEnd(Nodes* head, ElemType value)
{
    // 创建头节点
    Nodes* newNode = (Nodes*)malloc(sizeof(Nodes));
    if (newNode == NULL) return false;

    newNode->data = value;
    newNode->next = NULL;

    Nodes* p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = newNode;
    return true;
}

// 指定位置插入，pos为插入点
bool InsertAtPosition(Nodes* head, int pos, ElemType value)
{
    if (pos < 1) return false;
    // 指针p先指向头节点
    Nodes* p = head;
    int j = 0;
    while (p != NULL && j < pos - 1)
    {
        p = p->next;
        j++;
    }
    // 若一开始就是没有一个节点，就直接返回false
    if (p == NULL) return false;
    // 分配新节点的内存空间
    Nodes* newNode = (Nodes*)malloc(sizeof(Nodes));
    if (newNode == NULL) return false;

    newNode->data = value;
    // 新节点指向的节点优先指向原来p指向的节点
    newNode->next = p->next;
    // 现在p所指向的下一个节点就是新分配的节点
    p->next = newNode;
    return true;
}

// 指定位置删除节点,pos为删除点，outValue为被删数据
bool DeleteAtPosition(Nodes* head, int pos, ElemType* outValue)
{
    if (pos < 1) return false;
    // 指针p先指向头节点
    Nodes* p = head;
    int j = 0;
    while (p != NULL && j < pos - 1)
    {
        p = p->next;
        j++;
    }
    // 若一开始就是没有一个节点，就直接返回false
    if (p == NULL || p->next == NULL || pos<1) return false;

    // 要删除的节点为原来指针p指向的下一个节点
    Nodes* oldNode = p->next;
    *outValue = oldNode->data;
    p->next = oldNode->next;
    free(oldNode);
    oldNode = NULL;
    return true;
}

// 按值删除节点:删除链表中第一个值为value的节点
int DeleteByValue(Nodes* head, ElemType value)
{
    if (head == NULL) return 0;
    Nodes* previous = head;
    Nodes* current = head->next;

    while (current != NULL)
    {
        if (current->data == value)
        {
            previous->next = current->next;
            free(current);
            return 1;
        }
        // 没有找到，两指针一起向后移动
        previous = current;
        current = current->next;
    }
    // 遍历结束，始终没有，则结束当前函数 
    return 0;
}

// 按序查找节点
Nodes* FindByIndex(Nodes* head, int index)
{
    if (index < 0) return NULL;

    Nodes* p = head;
    int j = 0;
    while (p != NULL && j < index)
    {
        p = p->next;
        j++;
    }
    return p;
}

// 按值查找节点，查找第一个值为 value 的节点，返回节点指针（找不到返回 NULL）
Nodes* Find(Nodes* head, ElemType value)
{
    Nodes* p = head->next;
    while (p != NULL && p->data != value)
    {
        p = p->next;
    }
    return p;
}

// 修改指定位置的节点的值
int UpdateAtPosition(Nodes* head, int pos, ElemType newValue)
{
    if (pos < 1 || head == NULL) return 0;

    // 从第一个数据节点开始查找（位置从1开始）
    Nodes* current = head->next;
    int index = 1;

    // 遍历列表直到找到第 pos 个数据节点
    while (current != NULL && index < pos)
    {
        current = current->next;
        index++;
    }

    // 位置超出链表长度
    if (current == NULL) return 0;

    // 找到后修改指定位置的数据域的值
    current->data = newValue;
    return 1;
}

// 打印/遍历链表
void PrintList(Nodes* head)
{
    Nodes* p = head->next;
    int index = 1;
    while (p != NULL)
    {
        printf("节点%d的值为%d\n", index++, p->data);
        p = p->next;
    }
}

// 原地反转链表 ---- 双指针
void ReverseList(Nodes* head)
{
    if (head == NULL) return;
    Nodes* start = head->next;
    Nodes* end = NULL;
    while (start != NULL)
    {
        Nodes* next = start->next;
        start->next = end;
        end = start;
        start = next; 
        // 实现迭代器，向链表后面的元素移动
    }
    head->next = end;
}

// 合并两个已排序链表，返回新链表头节点（归并排序）
Nodes* MergeSortedLists(Nodes* head1, Nodes* head2)
{
    Nodes dummy; // 任意的一个头节点
    Nodes* tail = &dummy;
    dummy.next = NULL;

    while (head1 != NULL && head2 != NULL)
    {
        if (head1->data <= head2->data)
        {
            tail->next = head1;
            head1 = head1->next;
        }
        else
        {
            tail->next = head2;
            head2 = head2->next;
        }
        tail = tail->next;
    }

    if (head1 != NULL) tail->next = head1;
    else tail->next = head1;

    return dummy.next;
}

// 检查链表是否成环（适用于无头节点的纯指针检测，1 表示有环）
int HasCycle(Nodes* head)
{
    // 使用快慢指针（Floyd 判圈算法）检测环路
    if (head == NULL) return 0;

    Nodes* slow = head;
    Nodes* fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1; // 有环
    }
    return 0; // 无环
}