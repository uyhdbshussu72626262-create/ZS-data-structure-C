#pragma once

#include <stdbool.h>

typedef int ElemType;

// 动态顺序表结构体：用堆内存存储数据，可以自动扩容
// Dynamic sequential list struct: uses heap memory, supports auto-growing
typedef struct
{
    ElemType* data;    // 指向堆内存的数据指针 / pointer to heap-allocated element array
    int size;          // 当前元素个数 / number of elements currently stored
    int capacity;      // 当前已分配的容量 / total allocated capacity
} DynSqList;

// ---- 初始化和销毁 / Init & Destroy ----

// 初始化空顺序表，指针置空，大小归零
// Initialize an empty list: set data=NULL, size=0, capacity=0
bool Dyn_Init(DynSqList* L);

// 销毁顺序表，释放堆内存，所有属性归零
// Destroy the list: free heap memory, reset all fields to 0/NULL
void Dyn_Destroy(DynSqList* L);

// ---- 容量管理 / Capacity Management ----

// 手动扩容，在当前容量基础上增加 length 个元素的空间
// Manually expand capacity by 'length' elements
void Dyn_IncreaseSize(DynSqList* L, int length);

// 预留至少 newCapacity 个元素的空间（不会缩小）
// Reserve space for at least 'newCapacity' elements (never shrinks)
bool Dyn_Reserve(DynSqList* L, int newCapacity);

// 收缩容量到恰好等于当前元素个数，释放多余内存
// Shrink capacity to exactly fit the current elements, freeing wasted space
void Dyn_ShrinkToFit(DynSqList* L);

// ---- 元素增删改查 / CRUD Operations ----

// 在 pos 位置插入元素 e，pos 后面的元素整体后移一位
// Insert element 'e' at position 'pos', shifting subsequent elements right
bool Dyn_Insert(DynSqList* L, int pos, ElemType e);

// 删除 pos 位置的元素，被删的值通过指针 e 带出，后续元素前移
// Delete element at 'pos', output deleted value via 'e', shift left to fill gap
bool Dyn_Delete(DynSqList* L, int pos, ElemType* e);

// 将 pos 位置的元素修改为 e
// Update the element at position 'pos' to value 'e'
bool Dyn_Update(DynSqList* L, int pos, ElemType e);

// 按位置查找：把 pos 处的元素值写入 *e，返回是否成功
// Get element at 'pos', write it to *e, return success/failure
bool Dyn_GetElem(const DynSqList* L, int pos, ElemType* e);

// 按值查找（位序从 1 开始）：找到返回位序，找不到返回 -1
// Locate element by value (1-based index), returns -1 if not found
int  Dyn_LocateElem(const DynSqList* L, ElemType e);

// 按值查找（索引从 0 开始）：找到返回下标，找不到返回 -1
// Search element by value (0-based index), returns -1 if not found
int  Dyn_Search(const DynSqList* L, ElemType e);

// ---- 尾部操作 / Push & Pop ----

// 尾部追加元素，容量不够自动扩容
// Append element at the tail, auto-grows if capacity is insufficient
bool Dyn_PushBack(DynSqList* L, ElemType e);

// 尾部弹出元素，size 减一即可（不释放内存）
// Pop the last element: just decrement size, memory stays allocated
bool Dyn_PopBack(DynSqList* L);

// ---- 状态查询 / Status Queries ----

// 判断是否为空（size == 0）
// Check if the list is empty
bool Dyn_IsEmpty(const DynSqList* L);

// 判断是否已满（动态表永远不会满，始终返回 false）
// Check if full (dynamic list never fills up, always returns false)
bool Dyn_IsFull(const DynSqList* L);

// 获取当前元素个数
// Get the current number of elements
int  Dyn_GetLength(const DynSqList* L);

// 清空所有元素（size 置 0，capacity 和 data 保持不变）
// Clear all elements: sets size=0, keeps capacity and data intact
void Dyn_Clear(DynSqList* L);

// 打印顺序表内容（含长度和容量信息）
// Print the list contents with length and capacity info
void Dyn_Print(const DynSqList* L);

// ---- 高级算法 / Advanced Algorithms ----

// 深拷贝：将 src 的内容完整复制到 dest（独立的堆内存）
// Deep copy: duplicate src's content into dest with separate heap memory
bool Dyn_Copy(DynSqList* dest, const DynSqList* src);

// 原地反转顺序表（双指针法，时间复杂度 O(n)）
// Reverse the list in-place (two-pointer method, O(n) time)
void Dyn_Reverse(DynSqList* L);

// 对顺序表排序，ascending 为 true 升序，false 降序（快速排序实现）
// Sort the list: ascending=true for ascending order (uses quicksort)
void Dyn_Sort(DynSqList* L, bool ascending);

// 去除重复元素（HashSet 思想，只保留第一次出现的值），返回去重后的长度
// Remove duplicates (HashSet approach, keeps first occurrence), returns new length
int  Dyn_RemoveDuplicates(DynSqList* L);

// 合并两个顺序表，将 L1 和 L2 的元素依次放入 result（新分配内存）
// Merge L1 and L2 into result (newly allocated), preserving original order
bool Dyn_Merge(const DynSqList* L1, const DynSqList* L2, DynSqList* result);
