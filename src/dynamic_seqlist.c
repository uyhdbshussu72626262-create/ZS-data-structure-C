#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "dynamic_seqlist.h"

/*
 * Dyn_Init - 初始化动态顺序表
 * --------------------------------------------------------------
 * 中文：这就是个"开机"操作，把指针置空，元素个数和容量都清零。
 *       刚初始化完的表是个空壳，等真正插入数据时才会去分配内存。
 *
 * EN:   Sets the data pointer to NULL, zeros out both size and
 *        capacity. The list starts as an empty shell - heap memory
 *        won't be allocated until you actually insert elements.
 */
bool Dyn_Init(DynSqList* L)
{
    if (L == NULL) return false;
    L->data = NULL;
    L->size = 0;
    L->capacity = 0;
    return true;
}

/*
 * Dyn_Destroy - 销毁动态顺序表
 * --------------------------------------------------------------
 * 中文：把之前申请的堆内存 free 掉，然后把指针重新置空、size和capacity
 *       全部归零。调用完这个函数后，这张表就回到解放前了，但结构体本身还在
 *       （栈上的变量不会被释放）。
 *
 * EN:   Frees the heap memory, then resets all fields (data=NULL,
 *        size=0, capacity=0). After calling this the list is back
 *        to ground zero, but the struct itself (on the stack) still
 *        exists - you can Dyn_Init it again if needed.
 */
void Dyn_Destroy(DynSqList* L)
{
    if (L == NULL) return;
    free(L->data);
    L->data = NULL;
    L->size = 0;
    L->capacity = 0;
}

/*
 * dyn_do_grow (内部函数) - 自动扩容逻辑
 * --------------------------------------------------------------
 * 中文：这是内部用的扩容函数，Insert 和 PushBack 在空间不够时会调用它。
 *       策略：第一次从0开始就分配4个位置，之后每次翻倍扩容。
 *       如果翻倍还不够用（比如一口气插入很多），就直接扩到需要的大小。
 *       核心流程：开新空间 -> 搬数据 -> 释放旧空间 -> 更新指针和容量。
 *
 * EN:   Internal grow helper called by Insert and PushBack when
 *        space runs out. Strategy: start with 4 slots on first
 *        alloc, then double each time. If doubling still isn't
 *        enough, jump straight to the required size.
 *        Flow: allocate new -> copy old data -> free old -> update.
 */
static void dyn_do_grow(DynSqList* L, int min_capacity)
{
    int new_cap = (L->capacity == 0) ? 4 : L->capacity * 2;
    if (new_cap < min_capacity) new_cap = min_capacity;
    ElemType* newData = (ElemType*)malloc(new_cap * sizeof(ElemType));
    if (newData == NULL) return;
    for (int i = 0; i < L->size; i++)
    {
        newData[i] = L->data[i];
    }
    free(L->data);
    L->data = newData;
    L->capacity = new_cap;
}

/*
 * Dyn_IncreaseSize - 手动增加容量
 * --------------------------------------------------------------
 * 中文：在现有容量的基础上再追加 length 个位置。不管你现在用了多少，
 *       直接 capacity += length。适用于你提前知道要加多少数据、
 *       想一次性把空间申请到位，避免多次扩容的开销。
 *
 * EN:   Adds 'length' extra slots on top of current capacity.
 *        Doesn't care about current size — just bumps capacity by
 *        the given amount. Useful when you know ahead of time how
 *        many elements you'll add and want to avoid repeated grows.
 */
void Dyn_IncreaseSize(DynSqList* L, int length)
{
    if (L == NULL || length <= 0) return;
    ElemType* newData = (ElemType*)malloc((L->capacity + length) * sizeof(ElemType));
    if (newData == NULL) return;
    for (int i = 0; i < L->size; i++)
    {
        newData[i] = L->data[i];
    }
    free(L->data);
    L->data = newData;
    L->capacity = L->capacity + length;
}

/*
 * Dyn_Reserve - 预留空间
 * --------------------------------------------------------------
 * 中文：保证顺序表至少有 newCapacity 个位置可用。如果当前容量已经
 *       够大了，什么都不做直接返回成功；不够就扩容到指定大小。
 *       和 IncreaseSize 的区别：这个是"至少多少"，IncreaseSize 是"追加多少"。
 *
 * EN:   Guarantees the list has at least 'newCapacity' slots. If
 *        current capacity is already sufficient, does nothing and
 *        returns true. Otherwise expands to the requested size.
 *        Unlike IncreaseSize (which adds), this sets a floor.
 */
bool Dyn_Reserve(DynSqList* L, int newCapacity)
{
    if (L == NULL) return false;
    if (newCapacity <= L->capacity) return true;
    ElemType* newData = (ElemType*)malloc(newCapacity * sizeof(ElemType));
    if (newData == NULL)
    {
        printf("分配堆内存失败!\n");
        return false;
    }
    for (int i = 0; i < L->size; i++)
    {
        newData[i] = L->data[i];
    }
    free(L->data);
    L->data = newData;
    L->capacity = newCapacity;
    return true;
}

/*
 * Dyn_ShrinkToFit - 缩容到恰好装下
 * --------------------------------------------------------------
 * 中文：你删了一堆元素后，capacity 可能远大于 size，这时候调用这个函数
 *       可以把多余的内存还给系统。如果 size 已经是 0 了，就直接 free 掉；
 *       如果 capacity 本来就等于 size，说明没浪费，直接返回。
 *
 * EN:   After you've removed a bunch of elements, capacity might
 *        be way larger than size. This function trims the fat:
 *        reallocates to exactly fit the current elements. Special
 *        case: when size hits 0, just free everything.
 */
void Dyn_ShrinkToFit(DynSqList* L)
{
    if (L == NULL) return;
    if (L->capacity == L->size) return;
    if (L->size == 0)
    {
        free(L->data);
        L->data = NULL;
        L->capacity = 0;
        return;
    }
    ElemType* newData = (ElemType*)malloc(L->size * sizeof(ElemType));
    if (newData == NULL)
    {
        printf("释放失败\n");
        return;
    }
    for (int i = 0; i < L->size; i++)
    {
        newData[i] = L->data[i];
    }
    free(L->data);
    L->data = newData;
    L->capacity = L->size;
}

/*
 * Dyn_Insert - 在指定位置插入元素
 * --------------------------------------------------------------
 * 中文：经典顺序表插入操作。先检查 pos 是否合法（0 <= pos <= size），
 *       然后看容量够不够（不够就自动扩容），接着把 pos 到末尾的所有元素
 *       整体往后挪一位，最后把新元素填入 pos，size 加一。
 *
 * EN:   Classic sequential list insert. Step 1: validate position
 *       (0 <= pos <= size). Step 2: check capacity and grow if
 *       needed. Step 3: shift elements from pos to the end right
 *       by one. Step 4: drop the new element at pos, bump size.
 */
bool Dyn_Insert(DynSqList* L, int pos, ElemType e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos > L->size)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->size);
        return false;
    }
    if (L->size >= L->capacity)
    {
        dyn_do_grow(L, L->size + 1);
    }
    for (int i = L->size; i > pos; i--)
    {
        L->data[i] = L->data[i - 1];
    }
    L->data[pos] = e;
    L->size++;
    return true;
}

/*
 * Dyn_Delete - 删除指定位置的元素
 * --------------------------------------------------------------
 * 中文：删除也是顺序表的经典操作。先检查 pos 范围（0 <= pos < size，
 *       注意这里是严格小于，因为没有元素的位置不能删），然后如果调用者
 *       传了 e 指针就把被删的值带出去，接着把 pos 后面的所有元素往前
 *       挪一位填坑，最后 size 减一。
 *
 * EN:   Classic delete from sequential list. Validate pos range
 *       (0 <= pos < size, strict less-than since you can't delete
 *       a non-existent element). If caller provides an 'e' pointer,
 *       copy out the deleted value. Then shift all elements after
 *       pos left by one to fill the hole, and decrement size.
 */
bool Dyn_Delete(DynSqList* L, int pos, ElemType* e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos >= L->size)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->size);
        return false;
    }
    if (e != NULL)
    {
        *e = L->data[pos];
    }
    for (int i = pos; i < L->size - 1; i++)
    {
        L->data[i] = L->data[i + 1];
    }
    L->size--;
    return true;
}

/*
 * Dyn_Update - 修改指定位置的元素值
 * --------------------------------------------------------------
 * 中文：最简单的操作之一，检查位置合法后直接覆盖。就像数组赋值一样，
 *       只不过多了一层边界保护，防止你写到不该写的地方去。
 *
 * EN:   One of the simplest ops: validate position, then overwrite.
 *       Essentially array assignment with bounds checking to prevent
 *       writing to invalid locations.
 */
bool Dyn_Update(DynSqList* L, int pos, ElemType e)
{
    if (L == NULL) return false;
    if (pos < 0 || pos >= L->size)
    {
        printf("非法: pos=%d, len=%d\n", pos, L->size);
        return false;
    }
    L->data[pos] = e;
    return true;
}

/*
 * Dyn_GetElem - 按位置获取元素
 * --------------------------------------------------------------
 * 中文：给定位置 pos，把那个位置的值通过指针 e 带出来。pos 必须是
 *       有效范围内（0 到 size-1）。注意这里 L 是 const，说明只是读
 *       数据不会修改顺序表。
 *
 * EN:   Fetches the element at position 'pos' and writes it to *e.
 *       'pos' must be within [0, size-1]. Note that 'L' is const
 *       because we're only reading, not modifying the list.
 */
bool Dyn_GetElem(const DynSqList* L, int pos, ElemType* e)
{
    if (L == NULL || e == NULL) return false;
    if (pos < 0 || pos >= L->size)
    {
        printf("非法: pos=%d\n", pos);
        return false;
    }
    *e = L->data[pos];
    return true;
}

/*
 * Dyn_LocateElem - 按值查找（返回位序，从1开始）
 * --------------------------------------------------------------
 * 中文：从前往后遍历，找到第一个值等于 e 的元素，返回它的"位序"
 *       （第几个元素，从 1 开始数）。找不到就返回 -1。
 *       适合用来判断"这个值在表的第几个位置"。
 *
 * EN:   Scans from front to back for the first element matching 'e'.
 *       Returns the 1-based position (i.e., "it's the Nth element").
 *       Returns -1 if not found. Useful for "where is this value?"
 *       kinds of queries.
 */
int Dyn_LocateElem(const DynSqList* L, ElemType e)
{
    if (L == NULL) return -1;
    for (int i = 0; i < L->size; i++)
    {
        if (L->data[i] == e) return i + 1;
    }
    return -1;
}

/*
 * Dyn_Search - 按值查找（返回下标，从0开始）
 * --------------------------------------------------------------
 * 中文：和 LocateElem 一样是找值，但这个返回的是数组下标（0-based），
 *       找不到也是 -1。适用场景：你拿到下标后想直接用它做 Update 或 Delete。
 *
 * EN:   Same value lookup as LocateElem, but returns the 0-based
 *       array index instead. Also returns -1 on miss. Use this
 *       when you plan to immediately use the index for Update or
 *       Delete operations.
 */
int Dyn_Search(const DynSqList* L, ElemType e)
{
    if (L == NULL) return -1;
    for (int i = 0; i < L->size; i++)
    {
        if (L->data[i] == e) return i;
    }
    return -1;
}

/*
 * Dyn_PushBack - 尾部追加元素
 * --------------------------------------------------------------
 * 中文：动态数组最常用的操作，直接往屁股后面塞一个元素。如果容量满了
 *       会自动扩容（调用 dyn_do_grow）。流程特别短：检查容量 ->
 *       放到末尾 -> size 加一，三步搞定。
 *
 * EN:   The most common operation on a dynamic array: tack an
 *       element onto the end. Auto-grows if capacity is full.
 *       Super short flow: check capacity -> write to end -> bump
 *       size. Three steps, done.
 */
bool Dyn_PushBack(DynSqList* L, ElemType e)
{
    if (L == NULL) return false;
    if (L->size >= L->capacity)
    {
        dyn_do_grow(L, L->size + 1);
    }
    L->data[L->size] = e;
    L->size++;
    return true;
}

/*
 * Dyn_PopBack - 尾部弹出元素
 * --------------------------------------------------------------
 * 中文：把最后一个元素"删掉"，其实就是 size 减一。原来的数据还在内存里，
 *       但逻辑上已经不属于这张表了（后面再 PushBack 会覆盖掉）。
 *       如果表本身已经空了就返回 false，啥也不做。
 *
 * EN:   "Removes" the last element by simply decrementing size.
 *       The old data is still physically in memory but logically
 *       no longer part of the list (a future PushBack will overwrite
 *       it). Returns false on an already-empty list.
 */
bool Dyn_PopBack(DynSqList* L)
{
    if (L == NULL) return false;
    if (L->size == 0) return false;
    L->size--;
    return true;
}

/*
 * Dyn_IsEmpty - 判断表是否为空
 * --------------------------------------------------------------
 * 中文：size 为 0 就是空，简单粗暴。NULL 指针也当空处理。
 *
 * EN:   Empty means size == 0. Also treats NULL pointer as empty.
 */
bool Dyn_IsEmpty(const DynSqList* L)
{
    if (L == NULL) return true;
    return L->size == 0;
}

/*
 * Dyn_IsFull - 判断表是否已满
 * --------------------------------------------------------------
 * 中文：动态表理论上是"无上限"的（只要系统还有内存就能扩容），
 *       所以这个函数永远返回 false。存在的意义是和静态表保持 API 一致，
 *       方便你写泛型代码时不用判断用的是哪种表。
 *
 * EN:   A dynamic list technically has no upper bound (as long as
 *       the OS can give more memory). So this always returns false.
 *       Exists purely for API symmetry with StaticSqList, so you
 *       can write generic code without checking which list type.
 */
bool Dyn_IsFull(const DynSqList* L)
{
    (void)L;
    return false;
}

/*
 * Dyn_GetLength - 获取元素个数
 * --------------------------------------------------------------
 * 中文：返回 size，其实就是"表里有几个元素"。NULL 指针返回 -1。
 *
 * EN:   Returns size - the number of elements in the list.
 *       Returns -1 for NULL pointer.
 */
int Dyn_GetLength(const DynSqList* L)
{
    if (L == NULL) return -1;
    return L->size;
}

/*
 * Dyn_Clear - 清空表
 * --------------------------------------------------------------
 * 中文：直接把 size 设成 0 就完事了，数据还在内存里但逻辑上表已经
 *       "清空"了。capacity 不变，下次插入时如果容量还够用就不会重新分配。
 *       想连内存也释放的话，用 Dyn_ShrinkToFit 或者 Dyn_Destroy。
 *
 * EN:   Simply sets size to 0. The data is still in memory but
 *       logically the list is "cleared". Capacity stays the same,
 *       so future inserts won't reallocate if there's still room.
 *       If you want to also free memory, use Dyn_ShrinkToFit or
 *       Dyn_Destroy instead.
 */
void Dyn_Clear(DynSqList* L)
{
    if (L == NULL) return;
    L->size = 0;
}

/*
 * Dyn_Print - 打印顺序表内容
 * --------------------------------------------------------------
 * 中文：遍历打印每个元素，前面先输出长度和容量信息方便调试。
 *       格式大概是这样：DynSeqList (length=3 capacity=8): 10 20 30
 *
 * EN:   Iterates and prints each element, with a header line
 *       showing length and capacity for debugging purposes.
 *       Output looks like: DynSeqList (length=3 capacity=8): 10 20 30
 */
void Dyn_Print(const DynSqList* L)
{
    if (L == NULL) return;
    printf("DynSeqList (length=%d capacity=%d): ", L->size, L->capacity);
    for (int i = 0; i < L->size; i++)
    {
        printf("%d ", L->data[i]);
    }
    printf("\n");
}

/*
 * Dyn_Copy - 深拷贝顺序表
 * --------------------------------------------------------------
 * 中文：把 src 的所有元素完整复制一份到 dest。注意：这是深拷贝，
 *       dest 会拿到一份独立的堆内存，和 src 互不影响。内部会先
 *       清除 dest 的旧数据（如果有的话），然后按 src 的大小重新分配。
 *
 * EN:   Makes a complete duplicate of 'src' into 'dest'. This is a
 *       deep copy — dest gets its own independent heap memory, so
 *       modifying one won't affect the other. Internally, dest's
 *       old data (if any) is freed first, then reallocated to
 *       match src's size.
 */
bool Dyn_Copy(DynSqList* dest, const DynSqList* src)
{
    if (dest == NULL || src == NULL) return false;
    Dyn_Destroy(dest);
    if (src->size == 0)
    {
        dest->data = NULL;
        dest->size = 0;
        dest->capacity = 0;
        return true;
    }
    dest->data = (ElemType*)malloc(src->size * sizeof(ElemType));
    if (dest->data == NULL) return false;
    for (int i = 0; i < src->size; i++)
    {
        dest->data[i] = src->data[i];
    }
    dest->size = src->size;
    dest->capacity = src->size;
    return true;
}

/*
 * Dyn_Reverse - 原地反转顺序表
 * --------------------------------------------------------------
 * 中文：经典的双指针法。一个指针从头部往后走，另一个从尾部往前走，
 *       每次交换两个指针指向的元素，走到中间碰头就完事了。时间复杂度
 *       O(n)，空间复杂度 O(1)（不需要额外数组）。
 *
 * EN:   Classic two-pointer reversal. One pointer goes forward from
 *       the head, the other backward from the tail. Swap elements
 *       at each step until they meet in the middle. O(n) time,
 *       O(1) space (no extra array needed).
 */
void Dyn_Reverse(DynSqList* L)
{
    if (L == NULL || L->size <= 1) return;
    int start = 0;
    int end = L->size - 1;
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
 * dyn_swap (内部函数) - 交换两个元素的值
 * --------------------------------------------------------------
 * 中文：经典三行交换法，用临时变量暂存一下。
 *
 * EN:   Classic three-line swap using a temporary variable.
 */
static void dyn_swap(ElemType* a, ElemType* b)
{
    ElemType temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * dyn_quick_sort (内部函数) - 快速排序递归实现
 * --------------------------------------------------------------
 * 中文：快排的核心递归函数。选中间元素当基准（pivot），然后用双指针
 *       把小于基准的扔左边、大于的扔右边，最后递归处理左右两半。
 *       ascending 控制升序还是降序：true 是升序（小的在左），
 *       false 是降序（大的在左）。
 *
 * EN:   Core quicksort recursion. Picks the middle element as pivot,
 *       then uses two pointers to partition: smaller elements go left,
 *       larger go right. Recursively sorts the left and right halves.
 *       The 'ascending' flag controls order: true = ascending
 *       (small left), false = descending (large left).
 */
static void dyn_quick_sort(ElemType arr[], int left, int right, bool ascending)
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
            dyn_swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (left < j)  dyn_quick_sort(arr, left, j, ascending);
    if (i < right) dyn_quick_sort(arr, i, right, ascending);
}

/*
 * Dyn_Sort - 对顺序表排序
 * --------------------------------------------------------------
 * 中文：直接对顺序表内部的数据进行快速排序。ascending 为 true 是升序，
 *       false 是降序。注意这是原地排序，不分配额外的大数组，空间开销
 *       主要来自递归栈（平均 O(log n)）。
 *
 * EN:   Sorts the list in-place using quicksort. Set ascending=true
 *       for ascending order, false for descending. This is an
 *       in-place sort — no large extra arrays are allocated. Space
 *       overhead comes from the recursion stack, averaging O(log n).
 */
void Dyn_Sort(DynSqList* L, bool ascending)
{
    if (L == NULL || L->size <= 1) return;
    dyn_quick_sort(L->data, 0, L->size - 1, ascending);
}

/*
 * Dyn_RemoveDuplicates - 去重
 * --------------------------------------------------------------
 * 中文：用了一个"visited"数组当 HashSet 用（大小 1000，所以假设元素
 *       在 0~999 范围内）。遍历每个元素，如果之前没见过（visited 为 false），
 *       就保留它并标记已见过；如果见过了就直接跳过。超出 0~999 范围的元素
 *       会被直接保留（不去重，但至少不会越界崩溃）。返回去重后的新长度。
 *
 * EN:   Uses a "visited" boolean array as a simple HashSet (size 1000,
 *       so assumes elements are in the 0~999 range). Scans each element:
 *       if unseen (visited[e] is false), keep it and mark seen; if
 *       already seen, skip. Elements outside the 0~999 range are kept
 *       as-is (won't be deduped, but won't crash either). Returns the
 *       new length after deduplication.
 */
int Dyn_RemoveDuplicates(DynSqList* L)
{
    if (L == NULL) return -1;
    bool visited[1000] = { false };
    int newLength = 0;
    for (int i = 0; i < L->size; i++)
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
    L->size = newLength;
    return newLength;
}

/*
 * Dyn_Merge - 合并两个顺序表
 * --------------------------------------------------------------
 * 中文：把 L1 和 L2 的元素依次放进 result 里面，L1 的在前，L2 的在后。
 *       result 的堆内存是新分配的，大小是 L1->size + L2->size。
 *       注意：调用者要确保 result 之前的数据（如果有）能被覆盖，
 *       本函数不会帮你释放 result 的旧内存。
 *
 * EN:   Copies elements from L1 then L2 into result (L1 first,
 *       L2 second). Result gets freshly allocated heap memory sized
 *       to fit both. Note: caller is responsible for ensuring
 *       result's previous data (if any) can be overwritten — this
 *       function doesn't free result's old memory for you.
 */
bool Dyn_Merge(const DynSqList* L1, const DynSqList* L2, DynSqList* result)
{
    if (L1 == NULL || L2 == NULL || result == NULL) return false;
    int total_size = L1->size + L2->size;
    result->data = (ElemType*)malloc(total_size * sizeof(ElemType));
    if (result->data == NULL) return false;
    for (int i = 0; i < L1->size; i++)
    {
        result->data[i] = L1->data[i];
    }
    for (int i = 0; i < L2->size; i++)
    {
        result->data[L1->size + i] = L2->data[i];
    }
    result->size = total_size;
    result->capacity = total_size;
    return true;
}
