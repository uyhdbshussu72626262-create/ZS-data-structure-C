#include <stdio.h>
#include <Windows.h>
#include "dynamic_seqlist.h"
#include "static_seqlist.h"
#include "singly_linked_list.h"

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    printf("=== 动态顺序表演示 ===\n");
    DynSqList L1;
    Dyn_Init(&L1);

    Dyn_PushBack(&L1, 10);
    Dyn_PushBack(&L1, 20);
    Dyn_PushBack(&L1, 30);
    Dyn_Print(&L1);

    Dyn_Insert(&L1, 1, 15);
    Dyn_Print(&L1);

    int deleted;
    Dyn_Delete(&L1, 2, &deleted);
    printf("删除了元素: %d\n", deleted);

    Dyn_Update(&L1, 0, 100);
    Dyn_Print(&L1);

    Dyn_Reverse(&L1);
    printf("反转后: ");
    Dyn_Print(&L1);

    Dyn_Sort(&L1, true);
    printf("升序排序后: ");
    Dyn_Print(&L1);

    printf("长度: %d, 是否为空: %d\n", Dyn_GetLength(&L1), Dyn_IsEmpty(&L1));
    Dyn_Destroy(&L1);

    printf("\n=== 静态顺序表演示 ===\n");
    StaticSqList L2;
    Static_Init(&L2);

    Static_Insert(&L2, 0, 10);
    Static_Insert(&L2, 1, 20);
    Static_Insert(&L2, 0, 5);
    Static_Insert(&L2, 3, 30);
    Static_Insert(&L2, 2, 15);
    Static_Print(&L2);

    Static_Delete(&L2, 2, &deleted);
    printf("删除了元素: %d\n", deleted);

    Static_Update(&L2, 1, 99);
    Static_Print(&L2);

    Static_Reverse(&L2);
    printf("反转后: ");
    Static_Print(&L2);

    int pos = Static_Search(&L2, 20);
    if (pos != -1)
        printf("元素20位于下标 %d\n", pos);
    else
        printf("未找到20\n");

    Static_Sort(&L2, true);
    printf("升序排序后: ");
    Static_Print(&L2);

    printf("长度: %d, 是否为空: %d, 是否为满: %d\n",
        Static_GetLength(&L2), Static_IsEmpty(&L2), Static_IsFull(&L2));

    /* ---------------- 单链表演示 ---------------- */
    printf("\n=== 单链表演示 ===\n");

    // 初始化（创建头节点）
    Nodes* head = InitList();
    if (head == NULL) {
        printf("InitList 失败\n");
    }
    else {
        // 插入：尾插、头插、指定位置插入（位置从1开始，对数据节点计数）
        InsertAtEnd(head, 10);
        InsertAtEnd(head, 30);
        InsertAtHead(head, 5);
        InsertAtPosition(head, 2, 15); // 在位置2插入15
        printf("初始链表:\n");
        PrintList(head);

        // 长度与判空
        printf("长度: %d, 是否为空: %d\n", GetLength(head), IsEmpty(head));

        // 查找：按索引与按值
        Nodes* n = FindByIndex(head, 2); // 位置2的数据节点
        if (n != NULL) printf("位置2的值: %d\n", n->data);

        Nodes* f = Find(head, 30);
        if (f != NULL) printf("查找值30: 找到\n");
        else printf("查找值30: 未找到\n");

        // 更新指定位置
        if (UpdateAtPosition(head, 3, 100)) printf("更新位置3为100\n");

        printf("更新后:\n");
        PrintList(head);

        // 按位置删除
        if (DeleteAtPosition(head, 2, &deleted)) printf("删除位置2，值=%d\n", deleted);
        else printf("删除位置2失败\n");

        PrintList(head);

        // 按值删除（删除第一个匹配项）
        if (DeleteByValue(head, 30)) printf("按值删除30成功\n");
        else printf("按值删除30失败\n");

        PrintList(head);

        // 原地反转
        ReverseList(head);
        printf("反转后:\n");
        PrintList(head);

        // 销毁当前示例链表（释放头结点及数据节点）
        DestoryList(&head);
        printf("销毁后 head == NULL ? %d\n", head == NULL);
    }

    // 合并两个已排序链表示例（带头节点，升序）
    Nodes* headA = InitList();
    Nodes* headB = InitList();
    if (headA && headB) {
        InsertAtEnd(headA, 1);
        InsertAtEnd(headA, 4);
        InsertAtEnd(headA, 7);

        InsertAtEnd(headB, 2);
        InsertAtEnd(headB, 3);
        InsertAtEnd(headB, 8);

        printf("\nheadA:\n"); PrintList(headA);
        printf("headB:\n"); PrintList(headB);

        // 合并后结果放在 headA（函数内部会 free(headB)）
        Nodes* merged = MergeSortedLists(headA, headB);
        printf("合并后:\n"); PrintList(merged);

        // 环检测（无环应返回0）
        printf("HasCycle(merged) = %d\n", HasCycle(merged));

        // 清空（保留头节点）然后销毁头节点
        ClearList(merged);
        printf("清空后: \n"); PrintList(merged);

        DestoryList(&merged);
        printf("销毁后 merged == NULL ? %d\n", merged == NULL);
    }
    else {
        // 若分配失败，确保释放已分配的头
        if (headA) DestoryList(&headA);
        if (headB) DestoryList(&headB);
    }

    // 构造有环链表并检测（示例：创建后形成环然后解除）
    Nodes* headCycle = InitList();
    if (headCycle) {
        InsertAtEnd(headCycle, 1);
        InsertAtEnd(headCycle, 2);
        InsertAtEnd(headCycle, 3);
        // 找到尾结点并指向第一个数据节点形成环
        Nodes* tail = headCycle->next;
        while (tail->next != NULL) tail = tail->next;
        tail->next = headCycle->next; // 形成环

        printf("HasCycle(headCycle) = %d (应为1)\n", HasCycle(headCycle));

        // 解除环（把尾结点指回 NULL）并销毁链表
        tail->next = NULL;
        DestoryList(&headCycle);
    }

    return 0;
}