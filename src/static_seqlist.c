#include <stdio.h>
#include <stdbool.h>
#include "static_seqlist.h"

/*
 * Static_Init - 初始化静态顺序表
 * --------------------------------------------------------------
 * 中文：把定长数组全部清零，length 设为 0，capacity 设为 MAX_SIZE。
 *       静态表一出生容量就定死了，后面不能扩容，只能在这个固定大小的
 *       数组里玩，所以加数据前要记得检查容量。
 *
 * EN:   Zeroes out the fixed-size array, sets length=0, and
 *       capacity=MAX_SIZE. Static lists are born with fixed capacity
 *       — no growing later. So always check capacity before inserting.
 */
void Static_Init(StaticSqList* L)
{
    if (L == NULL) return;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        L->data[i] = 0;
    }
    L->length = 0;
    // 最大容量
    L->capacity = MAX_SIZE;
}

/*
 * Static_Insert - 在指定位置插入元素
 * --------------------------------------------------------------
 * 中文：和动态版本一样的逻辑，但多了一步"检查表是否已满"。因为是静态表，
 *       满了就是满了，没得扩容。其余流程不变：校验位置 -> 检查容量 ->
 *       挪元素 -> 插入 -> length 加一。
 *
 * EN:   Same logic as the dynamic version, but with one extra step:
 *       checking whether the list is already full. Since this is
 *       a static list, "full" means game over — no expanding.
 *       Flow: validate pos -> check capacity -> shift right ->
 *       insert -> bump length.
 */
bool Static_Insert(StaticSqList* L, int pos, ElemType e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos > L->length)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->length);
        return false;
    }
    if (L->length >= MAX_SIZE)
    {
        printf("顺序表已满,无法插入\n");
        return false;
    }
    for (int i = L->length; i > pos; i--)
    {
        L->data[i] = L->data[i - 1];
    }
    L->data[pos] = e;
    L->length++;
    return true;
}

/*
 * Static_Delete - 删除指定位置的元素
 * --------------------------------------------------------------
 * 中文：逻辑和动态版一样 —— 校验位置 -> 取出被删元素 -> 前移填坑 ->
 *       length 减一。不需要考虑释放内存的问题，因为静态表的数据是
 *       嵌在结构体里的定长数组，不存在堆内存管理。
 *
 * EN:   Same logic as dynamic delete: validate pos -> copy out
 *       deleted value -> shift left to fill gap -> decrement length.
 *       No memory management concerns here since the data array is
 *       a fixed-size member embedded in the struct itself.
 */
bool Static_Delete(StaticSqList* L, int pos, ElemType* e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos >= L->length)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->length);
        return false;
    }
    if (e != NULL)
    {
        *e = L->data[pos];
    }
    for (int i = pos; i < L->length - 1; i++)
    {
        L->data[i] = L->data[i + 1];
    }
    L->length--;
    return true;
}

/*
 * Static_Update - 修改指定位置的元素值
 * --------------------------------------------------------------
 * 中文：校验位置 -> 直接覆盖。比动态版还简单，因为不用担心指针问题。
 *
 * EN:   Validate position, then overwrite. Even simpler than the
 *       dynamic version since there are no pointer concerns.
 */
bool Static_Update(StaticSqList* L, int pos, ElemType e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos >= L->length)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->length);
        return false;
    }
    L->data[pos] = e;
    return true;
}

/*
 * Static_Search - 按值查找（返回下标，0-based）
 * --------------------------------------------------------------
 * 中文：从前往后扫，找到第一个匹配的就返回它的数组下标（0 开始），
 *       找不到返回 -1。注意 L 是 const 指针，只读不写。
 *
 * EN:   Linear scan from front to back. Returns the 0-based index
 *       of the first match, or -1 on miss. 'L' is const — read-only.
 */
int Static_Search(const StaticSqList* L, ElemType e)
{
    if (L == NULL) return -1;
    for (int i = 0; i < L->length; i++)
    {
        if (L->data[i] == e) return i;
    }
    return -1;
}

/*
 * Static_GetElem - 按位置获取元素
 * --------------------------------------------------------------
 * 中文：给定 pos，把对应位置的值通过 e 指针带出来。pos 范围检查是
 *       0 到 length-1，超出就返回 false 并打印错误信息。
 *
 * EN:   Fetches the element at position 'pos' and writes it to *e.
 *       Bounds check: pos must be within [0, length-1]. Returns
 *       false and prints an error message on out-of-bounds.
 */
bool Static_GetElem(const StaticSqList* L, int pos, ElemType* e)
{
    if (L == NULL || e == NULL) return false;
    if (pos < 0 || pos >= L->length)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->length);
        return false;
    }
    *e = L->data[pos];
    return true;
}

/*
 * Static_LocateElem - 按值查找（返回位序，1-based）
 * --------------------------------------------------------------
 * 中文：和 Search 一样是找值，但返回的是"第几个"（1 开始数）。
 *       方便你直接跟人说"这个值在第 3 个位置"，不用再加一换算。
 *
 * EN:   Same search as Static_Search, but returns 1-based position.
 *       Convenient when you want to say "it's the 3rd element"
 *       without mentally adding one to the index.
 */
int Static_LocateElem(StaticSqList* L, ElemType e)
{
    if (L == NULL) return -1;
    for (int i = 0; i < L->length; i++)
    {
        if (L->data[i] == e) return i + 1;
    }
    return -1;
}

/*
 * Static_GetLength - 获取元素个数
 * --------------------------------------------------------------
 * 中文：返回 length。NULL 指针返回 -1。
 *
 * EN:   Returns length. Returns -1 for NULL pointer.
 */
int Static_GetLength(const StaticSqList* L)
{
    if (L == NULL) return -1;
    return L->length;
}

/*
 * Static_IsEmpty - 判断表是否为空
 * --------------------------------------------------------------
 * 中文：length 为 0 就返回 true。NULL 指针也当空处理。
 *
 * EN:   Returns true if length is 0. NULL pointer is also treated
 *       as empty for safety.
 */
bool Static_IsEmpty(const StaticSqList* L)
{
    if (L == NULL) return true;
    return L->length == 0;
}

/*
 * Static_IsFull - 判断表是否已满
 * --------------------------------------------------------------
 * 中文：静态表的 length 达到 MAX_SIZE 就是满了，不能再插。
 *       NULL 指针返回 false（逻辑上不存在的东西谈不上满）。
 *
 * EN:   Returns true when length hits MAX_SIZE — no more room.
 *       Returns false for NULL pointer (can't be "full" if it
 *       doesn't exist).
 */
bool Static_IsFull(const StaticSqList* L)
{
    if (L == NULL) return false;
    return L->length == MAX_SIZE;
}

/*
 * Static_Clear - 清空表
 * --------------------------------------------------------------
 * 中文：把 length 设成 0 就清空了，数组里的旧数据不会擦除（反正
 *       后面插入时会覆盖）。对静态表来说这是最简单的重置方式。
 *
 * EN:   Clears the list by setting length to 0. Old data stays in
 *       the array but will be overwritten by future inserts. This
 *       is the simplest way to reset a static list.
 */
void Static_Clear(StaticSqList* L)
{
    if (L == NULL) return;
    L->length = 0;
}

/*
 * Static_Print - 打印顺序表
 * --------------------------------------------------------------
 * 中文：输出格式例如 "StaticSqList (length=3/100): 10 20 30"，
 *       清楚地告诉你用了多少个位置、总共多少个位置，以及每个元素的值。
 *
 * EN:   Prints in format like "StaticSqList (length=3/100): 10 20 30",
 *       showing used slots, total slots, and each element's value.
 */
void Static_Print(const StaticSqList* L)
{
    if (L == NULL) return;
    printf("StaticSqList (length=%d/%d): ", L->length, MAX_SIZE);
    for (int i = 0; i < L->length; i++)
    {
        printf("%d ", L->data[i]);
    }
    printf("\n");
}

/*
 * Static_Copy - 复制顺序表
 * --------------------------------------------------------------
 * 中文：直接把 src 里的元素一个一个搬到 dest 里，是浅拷贝（值拷贝）。
 *       因为两个表都是静态的、数据就嵌在结构体内部，不存在指针共享
 *       的问题，所以浅拷贝就够用了。length 和 capacity 也一并拷过去。
 *
 * EN:   Straight-up element-by-element copy from src to dest. This
 *       is a shallow (value) copy, which is perfectly fine for
 *       static lists because both arrays are embedded in the struct
 *       — no pointer aliasing issues. Also copies length and capacity.
 */
bool Static_Copy(const StaticSqList* src, StaticSqList* dest)
{
    if (src == NULL || dest == NULL) return false;
    for (int i = 0; i < src->length; i++)
    {
        dest->data[i] = src->data[i];
    }
    dest->length = src->length;
    dest->capacity = src->capacity;
    return true;
}

/*
 * Static_Reverse - 原地反转顺序表
 * --------------------------------------------------------------
 * 中文：和动态版一模一样的双指针法。头尾两个指针往中间走，边走边交换。
 *       时间复杂度 O(n)，不需要额外数组。
 *
 * EN:   Same two-pointer approach as the dynamic version. Head and
 *       tail pointers move toward the middle, swapping as they go.
 *       O(n) time, no extra array required.
 */
void Static_Reverse(StaticSqList* L)
{
    if (L == NULL) return;
    int start = 0;
    int end = L->length - 1;
    while (start < end)
    {
        ElemType temp = L->data[start];
        L->data[start] = L->data[end];
        L->data[end] = temp;
        start++;
        end--;
    }
}

/*
 * sta_swap (内部函数) - 交换两个元素
 * --------------------------------------------------------------
 * 中文：用临时变量中转一下，完成两个值的交换。
 *
 * EN:   Swaps two values using a temporary variable.
 */
static void sta_swap(ElemType* a, ElemType* b)
{
    ElemType temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * sta_quick_sort (内部函数) - 快速排序递归实现
 * --------------------------------------------------------------
 * 中文：和动态版的快排实现完全一样。选中点当基准，双指针分区，
 *       把小的放左边大的放右边，然后递归。ascending 控制方向。
 *
 * EN:   Identical quicksort implementation to the dynamic version.
 *       Picks middle element as pivot, partitions with two pointers,
 *       recurses on left and right halves. The 'ascending' flag
 *       controls the sort direction.
 */
static void sta_quick_sort(ElemType arr[], int left, int right, bool ascending)
{
    if (left >= right) return;
    int i = left;
    int j = right;
    ElemType pivot = arr[(left + right) / 2];
    while (i <= j)
    {
        if (ascending)
        {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
        }
        else
        {
            while (arr[i] > pivot) i++;
            while (arr[j] < pivot) j--;
        }
        if (i <= j)
        {
            sta_swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (left < j)  sta_quick_sort(arr, left, j, ascending);
    if (i < right) sta_quick_sort(arr, i, right, ascending);
}

/*
 * Static_Sort - 对顺序表排序
 * --------------------------------------------------------------
 * 中文：直接对表内数据进行快速排序。ascending=true 升序，false 降序。
 *       和动态版完全一样，只是操作的是静态表的 data 数组。
 *
 * EN:   Sorts the list in-place using quicksort. ascending=true for
 *       ascending, false for descending. Identical logic to the
 *       dynamic version — just operates on the static list's data.
 */
void Static_Sort(StaticSqList* L, bool ascending)
{
    if (L == NULL || L->length <= 1) return;
    sta_quick_sort(L->data, 0, L->length - 1, ascending);
}

/*
 * Static_RemoveDuplicates - 去重
 * --------------------------------------------------------------
 * 中文：和动态版一样用 visited 数组当 HashSet。0~999 范围内的元素
 *       能正常去重，超出范围的保留但不参与去重逻辑（防止数组越界崩溃）。
 *       返回去重后的新长度。
 *
 * EN:   Uses the same visited-array-as-HashSet approach as the
 *       dynamic version. Elements in 0~999 are deduped normally;
 *       out-of-range values are kept but not checked for duplicates
 *       (to prevent array bounds crash). Returns new length.
 */
int Static_RemoveDuplicates(StaticSqList* L)
{
    if (L == NULL) return -1;
    bool visited[1000] = { false };
    int newLength = 0;
    for (int i = 0; i < L->length; i++)
    {
        int value = L->data[i];
        if (value >= 0 && value < 1000 && !visited[value])
        {
            visited[value] = true;
            L->data[newLength] = value;
            newLength++;
        }
        else if (value < 0 || value >= 1000)
        {
            L->data[newLength] = value;
            newLength++;
        }
    }
    L->length = newLength;
    return newLength;
}

/*
 * Static_Merge - 合并两个静态顺序表
 * --------------------------------------------------------------
 * 中文：把 L1 和 L2 的元素依次复制到 result 里（L1 在前，L2 在后）。
 *       静态表不能扩容，所以合并前会先检查总长度是否超过 MAX_SIZE，
 *       超过了就直接返回 false。合并后 result 的 capacity 固定为 MAX_SIZE。
 *
 * EN:   Copies L1 then L2 into result (L1 first, L2 second). Since
 *       static lists can't grow, we first check if the total length
 *       exceeds MAX_SIZE — if so, bail out with false. After merge,
 *       result's capacity is fixed at MAX_SIZE.
 */
bool Static_Merge(const StaticSqList* L1, const StaticSqList* L2, StaticSqList* result)
{
    if (L1 == NULL || L2 == NULL || result == NULL) return false;
    if (L1->length + L2->length > MAX_SIZE) return false;
    for (int i = 0; i < L1->length; i++)
    {
        result->data[i] = L1->data[i];
    }
    for (int i = 0; i < L2->length; i++)
    {
        // 在一表后面插入
        result->data[L1->length + i] = L2->data[i];
    }
    result->capacity = MAX_SIZE;
    result->length = L1->length + L2->length;
    return true;
}
