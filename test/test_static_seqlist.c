#include <stdio.h>
#include "static_seqlist.h"

static int g_pass = 0;
static int g_fail = 0;

#define TEST(name) printf("  %-45s", name)
#define PASS() do { printf("[PASS]\n"); g_pass++; } while(0)
#define FAIL() do { printf("[FAIL]\n"); g_fail++; } while(0)

int main(void)
{
    StaticSqList L, L2, L3;
    ElemType val;

    printf("=== 静态顺序表单元测试 ===\n\n");

    printf("[Static_Init]\n");
    Static_Init(&L);
    TEST("length == 0, capacity == MAX_SIZE after init");
    if (Static_GetLength(&L) == 0 && L.capacity == MAX_SIZE) PASS() else FAIL();

    printf("[Static_Insert]\n");
    TEST("insert pos=0 val=10");
    if (Static_Insert(&L, 0, 10) && L.data[0] == 10 && L.length == 1) PASS() else FAIL();
    TEST("insert pos=1 val=20");
    if (Static_Insert(&L, 1, 20) && L.data[1] == 20 && L.length == 2) PASS() else FAIL();
    TEST("insert pos=0 val=5 (head)");
    if (Static_Insert(&L, 0, 5) && L.data[0] == 5 && L.data[1] == 10 && L.length == 3) PASS() else FAIL();
    TEST("invalid pos=-1 rejected");
    if (!Static_Insert(&L, -1, 99)) PASS() else FAIL();
    TEST("invalid pos=100 rejected");
    if (!Static_Insert(&L, 100, 99)) PASS() else FAIL();

    printf("[Static_GetLength]\n");
    TEST("length == 3");
    if (Static_GetLength(&L) == 3) PASS() else FAIL();

    printf("[Static_IsEmpty / Static_IsFull]\n");
    TEST("not empty");
    if (!Static_IsEmpty(&L)) PASS() else FAIL();
    TEST("not full");
    if (!Static_IsFull(&L)) PASS() else FAIL();

    printf("[Static_GetElem]\n");
    TEST("pos=0 -> 5");
    if (Static_GetElem(&L, 0, &val) && val == 5) PASS() else FAIL();
    TEST("pos=2 -> 20");
    if (Static_GetElem(&L, 2, &val) && val == 20) PASS() else FAIL();
    TEST("pos=10 invalid");
    if (!Static_GetElem(&L, 10, &val)) PASS() else FAIL();

    printf("[Static_LocateElem / Static_Search]\n");
    TEST("LocateElem 10 (1-based) -> 2");
    if (Static_LocateElem(&L, 10) == 2) PASS() else FAIL();
    TEST("Search 10 (0-based) -> 1");
    if (Static_Search(&L, 10) == 1) PASS() else FAIL();
    TEST("LocateElem 999 -> -1");
    if (Static_LocateElem(&L, 999) == -1) PASS() else FAIL();

    printf("[Static_Update]\n");
    TEST("update pos=1 val=99");
    if (Static_Update(&L, 1, 99) && L.data[1] == 99) PASS() else FAIL();
    TEST("invalid pos=100 rejected");
    if (!Static_Update(&L, 100, 99)) PASS() else FAIL();

    printf("[Static_Delete]\n");
    val = -1;
    TEST("delete pos=0 val=5, length->2");
    if (Static_Delete(&L, 0, &val) && val == 5 && L.length == 2) PASS() else FAIL();
    TEST("invalid pos=-1 rejected");
    if (!Static_Delete(&L, -1, &val)) PASS() else FAIL();

    printf("[Static_Reverse]\n");
    Static_Clear(&L);
    Static_Insert(&L, 0, 1);
    Static_Insert(&L, 1, 2);
    Static_Insert(&L, 2, 3);
    Static_Reverse(&L);
    TEST("reverse [1,2,3] -> [3,2,1]");
    if (L.data[0] == 3 && L.data[1] == 2 && L.data[2] == 1) PASS() else FAIL();

    printf("[Static_Sort]\n");
    Static_Clear(&L);
    Static_Insert(&L, 0, 30);
    Static_Insert(&L, 1, 10);
    Static_Insert(&L, 2, 20);
    Static_Sort(&L, true);
    TEST("ascending sort -> [10,20,30]");
    if (L.data[0] == 10 && L.data[1] == 20 && L.data[2] == 30) PASS() else FAIL();
    Static_Sort(&L, false);
    TEST("descending sort -> [30,20,10]");
    if (L.data[0] == 30 && L.data[1] == 20 && L.data[2] == 10) PASS() else FAIL();

    printf("[Static_RemoveDuplicates]\n");
    Static_Clear(&L);
    Static_Insert(&L, 0, 1);
    Static_Insert(&L, 1, 2);
    Static_Insert(&L, 2, 2);
    Static_Insert(&L, 3, 3);
    Static_Insert(&L, 4, 3);
    int ret = Static_RemoveDuplicates(&L);
    TEST("remove dups len 5->3, returns 3");
    if (ret == 3 && Static_GetLength(&L) == 3) PASS() else FAIL();

    printf("[Static_Clear]\n");
    Static_Clear(&L);
    TEST("length == 0 after clear");
    if (Static_GetLength(&L) == 0) PASS() else FAIL();

    printf("[Static_Copy]\n");
    Static_Insert(&L, 0, 10);
    Static_Insert(&L, 1, 20);
    Static_Init(&L2);
    TEST("copy [10,20] -> dest has same");
    if (Static_Copy(&L, &L2) && L2.length == 2 && L2.data[0] == 10 && L2.data[1] == 20) PASS() else FAIL();

    printf("[Static_Merge]\n");
    Static_Init(&L3);
    TEST("merge [10,20]+[10,20] -> [10,20,10,20] len=4");
    if (Static_Merge(&L, &L2, &L3) && L3.length == 4 &&
        L3.data[0] == 10 && L3.data[1] == 20 &&
        L3.data[2] == 10 && L3.data[3] == 20) PASS() else FAIL();

    printf("[Fill to MAX_SIZE / Static_IsFull]\n");
    Static_Clear(&L);
    while (L.length < MAX_SIZE) {
        Static_Insert(&L, L.length, 0);
    }
    TEST("IsFull returns true");
    if (Static_IsFull(&L)) PASS() else FAIL();
    TEST("insert when full rejected");
    if (!Static_Insert(&L, 0, 999)) PASS() else FAIL();

    printf("[NULL pointer safety]\n");
    TEST("Static_Init(NULL) safe");
    Static_Init(NULL); PASS();
    TEST("Static_Insert(NULL,...) returns false");
    if (!Static_Insert(NULL, 0, 99)) PASS() else FAIL();
    TEST("Static_GetLength(NULL) returns -1");
    if (Static_GetLength(NULL) == -1) PASS() else FAIL();
    TEST("Static_IsEmpty(NULL) returns true");
    if (Static_IsEmpty(NULL)) PASS() else FAIL();

    printf("\n--- 结果: %d 通过, %d 失败 ---\n", g_pass, g_fail);
    return (g_fail > 0) ? 1 : 0;
}
