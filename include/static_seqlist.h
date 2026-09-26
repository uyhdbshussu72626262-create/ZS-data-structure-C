#pragma once

#include <stdbool.h>

// 静态顺序表最大容量
// Maximum capacity of the static sequential list
#define MAX_SIZE 100

typedef int ElemType;

// 静态顺序表结构体：用定长数组存储数据，容量固定不可扩容
// Static sequential list struct: fixed-size array, capacity never changes
typedef struct
{
    ElemType data[MAX_SIZE];  // 定长数组存储元素 / fixed-size array for elements
    int length;               // 当前元素个数 / current number of elements
    int capacity;             // 最大容量，始终等于 MAX_SIZE / max capacity, always MAX_SIZE
} StaticSqList;

// ---- 初始化 / Init ----

// 初始化静态顺序表：数组清零，length 归零，capacity 设为 MAX_SIZE
// Initialize the static list: zero out array, length=0, capacity=MAX_SIZE
void Static_Init(StaticSqList* L);

// ---- 元素增删改查 / CRUD Operations ----

// 在 pos 位置插入元素 e，后续元素后移，表满则插入失败
// Insert element 'e' at position 'pos', fails if the list is full
bool Static_Insert(StaticSqList* L, int pos, ElemType e);

// 删除 pos 位置的元素，被删值通过指针 e 带出，后续元素前移填坑
// Delete element at 'pos', output deleted value via 'e', shift left
bool Static_Delete(StaticSqList* L, int pos, ElemType* e);

// 将 pos 位置的元素修改为 e
// Update the element at position 'pos' to value 'e'
bool Static_Update(StaticSqList* L, int pos, ElemType e);

// 按值查找（索引从 0 开始）：找到返回下标，找不到返回 -1
// Search by value (0-based index), returns -1 if not found
int  Static_Search(const StaticSqList* L, ElemType e);

// 按位置查找：把 pos 处的元素值写入 *e
// Get element by position, write to *e
bool Static_GetElem(const StaticSqList* L, int pos, ElemType* e);

// 按值查找（位序从 1 开始）：找到返回位序，找不到返回 -1
// Locate element by value (1-based index), returns -1 if not found
int  Static_LocateElem(StaticSqList* L, ElemType e);

// ---- 状态查询 / Status Queries ----

// 获取当前元素个数
// Get the current number of elements
int  Static_GetLength(const StaticSqList* L);

// 判断是否为空
// Check if the list is empty
bool Static_IsEmpty(const StaticSqList* L);

// 判断是否已满（length == MAX_SIZE）
// Check if the list is full
bool Static_IsFull(const StaticSqList* L);

// 清空所有元素（length 置零，数据不清）
// Clear all elements: sets length=0, data remains unchanged
void Static_Clear(StaticSqList* L);

// 打印顺序表内容（含长度和最大容量）
// Print the list contents with length and max capacity info
void Static_Print(const StaticSqList* L);

// ---- 高级算法 / Advanced Algorithms ----

// 复制：将 src 的内容复制到 dest（直接数组拷贝，不涉及堆内存）
// Copy src into dest (simple array copy, no heap allocation needed)
bool Static_Copy(const StaticSqList* src, StaticSqList* dest);

// 原地反转顺序表（双指针法，时间复杂度 O(n)）
// Reverse the list in-place (two-pointer method, O(n) time)
void Static_Reverse(StaticSqList* L);

// 对顺序表排序，ascending 为 true 升序，false 降序（快速排序实现）
// Sort the list: ascending=true for ascending order (uses quicksort)
void Static_Sort(StaticSqList* L, bool ascending);

// 去除重复元素（HashSet 思想），返回去重后的新长度
// Remove duplicates (HashSet approach), returns new length after dedup
int  Static_RemoveDuplicates(StaticSqList* L);

// 合并两个顺序表，将 L1 和 L2 的元素依次放入 result（要求总长度 ≤ MAX_SIZE）
// Merge L1 and L2 into result, requires total length ≤ MAX_SIZE
bool Static_Merge(const StaticSqList* L1, const StaticSqList* L2, StaticSqList* result);
