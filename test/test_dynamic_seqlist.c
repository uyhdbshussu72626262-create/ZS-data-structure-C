#include <stdio.h>
#include "dynamic_seqlist.h"

static int g_pass = 0;
static int g_fail = 0;

#define TEST(name) printf("  %-40s", name)
#define PASS() do { printf("[PASS]\n"); g_pass++; } while(0)
#define FAIL() do { printf("[FAIL]\n"); g_fail++; } while(0)

int main(void)
{
    DynSqList L, L2, L3;
    ElemType val;

    printf("=== 动态顺序表单元测试 ===\n\n");

    printf("[Dyn_Init / Dyn_Destroy]\n");
    TEST("init sets NULL/size=0/cap=0");
    if (Dyn_Init(&L) && L.data == NULL && L.size == 0 && L.capacity == 0) PASS() else FAIL();

    printf("[Dyn_PushBack]\n");
    TEST("push 10");
    if (Dyn_PushBack(&L, 10) && L.size == 1 && L.data[0] == 10) PASS() else FAIL();
    TEST("push 20");
    if (Dyn_PushBack(&L, 20) && L.size == 2 && L.data[1] == 20) PASS() else FAIL();
    TEST("push 30");
    if (Dyn_PushBack(&L, 30) && L.size == 3 && L.data[2] == 30) PASS() else FAIL();

    printf("[Dyn_IsEmpty / Dyn_GetLength / Dyn_IsFull]\n");
    TEST("not empty");
    if (!Dyn_IsEmpty(&L)) PASS() else FAIL();
    TEST("length == 3");
    if (Dyn_GetLength(&L) == 3) PASS() else FAIL();
    TEST("IsFull always false");
    if (!Dyn_IsFull(&L)) PASS() else FAIL();

    printf("[Dyn_Insert]\n");
    TEST("insert at head (pos=0) val=5");
    if (Dyn_Insert(&L, 0, 5) && L.size == 4 && L.data[0] == 5) PASS() else FAIL();
    TEST("insert at tail (pos=4) val=40");
    if (Dyn_Insert(&L, 4, 40) && L.size == 5 && L.data[4] == 40) PASS() else FAIL();
    TEST("insert at middle (pos=2) val=15");
    if (Dyn_Insert(&L, 2, 15) && L.size == 6 && L.data[2] == 15) PASS() else FAIL();
    TEST("invalid pos=-1 rejected");
    if (!Dyn_Insert(&L, -1, 99)) PASS() else FAIL();

    printf("[Dyn_GetElem]\n");
    TEST("get pos=2 -> 15");
    if (Dyn_GetElem(&L, 2, &val) && val == 15) PASS() else FAIL();
    TEST("get pos=5 -> 40");
    if (Dyn_GetElem(&L, 5, &val) && val == 40) PASS() else FAIL();
    TEST("invalid pos=100 rejected");
    if (!Dyn_GetElem(&L, 100, &val)) PASS() else FAIL();

    printf("[Dyn_LocateElem / Dyn_Search]\n");
    TEST("LocateElem 15 (1-based) -> 3");
    if (Dyn_LocateElem(&L, 15) == 3) PASS() else FAIL();
    TEST("Search 15 (0-based) -> 2");
    if (Dyn_Search(&L, 15) == 2) PASS() else FAIL();
    TEST("LocateElem 999 -> -1");
    if (Dyn_LocateElem(&L, 999) == -1) PASS() else FAIL();

    printf("[Dyn_Update]\n");
    TEST("update pos=1 to 99");
    if (Dyn_Update(&L, 1, 99) && L.data[1] == 99) PASS() else FAIL();
    TEST("invalid pos=100 rejected");
    if (!Dyn_Update(&L, 100, 99)) PASS() else FAIL();

    printf("[Dyn_Delete]\n");
    val = -1;
    TEST("delete pos=2 return 15");
    if (Dyn_Delete(&L, 2, &val) && val == 15 && L.size == 5) PASS() else FAIL();
    TEST("invalid pos=-1 rejected");
    if (!Dyn_Delete(&L, -1, &val)) PASS() else FAIL();

    printf("[Dyn_PopBack]\n");
    Dyn_Init(&L2);
    Dyn_PushBack(&L2, 100);
    Dyn_PushBack(&L2, 200);
    TEST("popback returns true, size->1");
    if (Dyn_PopBack(&L2) && L2.size == 1 && L2.data[0] == 100) PASS() else FAIL();
    Dyn_PopBack(&L2);
    TEST("popback on empty returns false");
    if (!Dyn_PopBack(&L2)) PASS() else FAIL();
    Dyn_Destroy(&L2);

    printf("[Dyn_Reserve]\n");
    Dyn_Init(&L2);
    TEST("reserve to 100");
    if (Dyn_Reserve(&L2, 100) && L2.capacity == 100) PASS() else FAIL();
    Dyn_Destroy(&L2);

    printf("[Dyn_Clear]\n");
    int oldCap = L.capacity;
    Dyn_Clear(&L);
    TEST("clear sets size=0, cap unchanged");
    if (L.size == 0 && L.capacity == oldCap) PASS() else FAIL();

    printf("[Dyn_Reverse]\n");
    Dyn_Clear(&L);
    Dyn_PushBack(&L, 1);
    Dyn_PushBack(&L, 2);
    Dyn_PushBack(&L, 3);
    Dyn_Reverse(&L);
    TEST("reverse [1,2,3] -> [3,2,1]");
    if (L.data[0] == 3 && L.data[1] == 2 && L.data[2] == 1) PASS() else FAIL();

    printf("[Dyn_Sort]\n");
    Dyn_Clear(&L);
    Dyn_PushBack(&L, 30);
    Dyn_PushBack(&L, 10);
    Dyn_PushBack(&L, 20);
    Dyn_Sort(&L, true);
    TEST("ascending sort -> [10,20,30]");
    if (L.data[0] == 10 && L.data[1] == 20 && L.data[2] == 30) PASS() else FAIL();
    Dyn_Sort(&L, false);
    TEST("descending sort -> [30,20,10]");
    if (L.data[0] == 30 && L.data[1] == 20 && L.data[2] == 10) PASS() else FAIL();

    printf("[Dyn_RemoveDuplicates]\n");
    Dyn_Clear(&L);
    Dyn_PushBack(&L, 1);
    Dyn_PushBack(&L, 2);
    Dyn_PushBack(&L, 2);
    Dyn_PushBack(&L, 3);
    Dyn_PushBack(&L, 3);
    int newLen = Dyn_RemoveDuplicates(&L);
    TEST("remove dups len 5->3, returns 3");
    if (newLen == 3 && L.size == 3) PASS() else FAIL();

    printf("[Dyn_Copy]\n");
    Dyn_Clear(&L);
    Dyn_PushBack(&L, 1);
    Dyn_PushBack(&L, 2);
    Dyn_Init(&L2);
    TEST("copy [1,2] -> same values");
    if (Dyn_Copy(&L2, &L) && L2.size == 2 && L2.data[0] == 1 && L2.data[1] == 2) PASS() else FAIL();
    Dyn_Destroy(&L2);

    printf("[Dyn_Merge]\n");
    Dyn_Clear(&L);
    Dyn_PushBack(&L, 1);
    Dyn_PushBack(&L, 2);
    Dyn_Init(&L2);
    Dyn_PushBack(&L2, 3);
    Dyn_PushBack(&L2, 4);
    Dyn_Init(&L3);
    TEST("merge [1,2]+[3,4] -> [1,2,3,4]");
    if (Dyn_Merge(&L, &L2, &L3) && L3.size == 4 &&
        L3.data[0] == 1 && L3.data[1] == 2 &&
        L3.data[2] == 3 && L3.data[3] == 4) PASS() else FAIL();
    Dyn_Destroy(&L2);
    Dyn_Destroy(&L3);

    printf("[Dyn_ShrinkToFit]\n");
    Dyn_Clear(&L);
    Dyn_Reserve(&L, 100);
    Dyn_PushBack(&L, 1);
    Dyn_ShrinkToFit(&L);
    TEST("capacity 100->1 after shrink");
    if (L.capacity == 1 && L.size == 1) PASS() else FAIL();

    printf("[NULL pointer safety]\n");
    TEST("Dyn_Init(NULL) returns false");
    if (!Dyn_Init(NULL)) PASS() else FAIL();
    TEST("Dyn_Insert(NULL,...) returns false");
    if (!Dyn_Insert(NULL, 0, 99)) PASS() else FAIL();
    TEST("Dyn_GetLength(NULL) returns -1");
    if (Dyn_GetLength(NULL) == -1) PASS() else FAIL();

    Dyn_Destroy(&L);

    printf("\n--- 结果: %d 通过, %d 失败 ---\n", g_pass, g_fail);
    return (g_fail > 0) ? 1 : 0;
}
