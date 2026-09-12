#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 100

// 408 纯 C 顺序表结构体定义
typedef struct {
    int data[MaxSize];
    int length;
} SqList;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化顺序表
// 目标：将顺序表长度置 0
void InitList(SqList *L) {
  // 请在此编写你的代码：
  L->length = 0;
}

// 2. 顺序表插入
// 目标：在第 i 个位置（位序 1 <= i <= L->length + 1）插入元素 e
// 返回：成功返回 true，非法或表满返回 false
// 注意：注意元素后移的方向
bool ListInsert(SqList *L, int i, int e) {
  // 请在此编写你的代码：
  if(i < 0 || i > L->length + 1) return false;  // i <= 0
  // 漏判表满：if (L->length >= MaxSize) return false; 
  for(int pos = L->length; pos >= i; pos--) {
    L->data[pos] = L->data[pos - 1];
  }
  L->data[i - 1] = e;
  L->length++;
  return true;
}

// 3. 顺序表删除
// 目标：删除第 i 个位置（位序 1 <= i <= L->length）的元素，并通过指针 e 带回其值
// 返回：成功返回 true，位置非法返回 false
// 注意：注意元素前移覆盖的方向
bool ListDelete(SqList *L, int i, int *e) {
    // 请在此编写你的代码：
    if(i < 1 || i > L->length) return false;
    *e = L->data[i - 1];
    // 应该为 L->length - 1，否则会访问到未初始化地界
    for(int pos = i - 1; pos < L->length; pos++) {
        L->data[pos] = L->data[pos + 1];
    }
    L->length--;
    return true;
}

// 4. 按值查找
// 目标：在顺序表 L 中查找第一个值为 e 的元素，返回其位序（从 1 开始）；未找到返回 0
int LocateElem(const SqList *L, int e) {
    // 请在此编写你的代码：
    for(int i = 0; i < L->length; i++) {
        if(L->data[i] == e) {
            return i + 1;
        }
    }
    return 0;
}

// 5. 原地逆置（空间复杂度 O(1)）
// 目标：将顺序表中的所有元素在原地颠倒顺序
void Reverse(SqList *L) {
    // 请在此编写你的代码：
    int left = 0;
    int right = L->length - 1;
    while(left < right) {
        int tmp = L->data[left];
        L->data[left] = L->data[right];
        L->data[right] = tmp;
        left++;
        right--;
    }
}
// 拓展循环左移动p个元素 -> 三次翻转

// 6. 有序表原地去重（408 高频大题原型，要求时间 O(n)，空间 O(1)）
// 目标：递增有序的顺序表中，删除所有重复元素，使表中所有元素互不相同
void DeleteDuplicates(SqList *L) {
    // 请在此编写你的代码：
    // int k = 0;     // 去除后的长度
    // int front = L->data[0] - 1;   // ->bug
    // for(int i = 0; i < L->length; i++) {
    //     if(L->data[i] == front) continue;
    //     L->data[k] = L->data[i];
    //     front = L->data[i];
    //     k++;
    // }
    // L->length = k;
    if(L->length <= 1) return ;
    int k = 0;
    for(int i = 1; i < L->length; i++) {
        if(L->data[i] == L->data[k]) continue;
        k++;
        L->data[k] = L->data[i];
        
    }
    L->length = k + 1;
}

// 7. 有序表合并（归并思想，两表递增，合并后 C 递增）
// 目标：将有序表 A 和 B 合并到 C 中
// 返回：若合并后超出最大容量返回 false，否则返回 true
bool Merge(const SqList *A, const SqList *B, SqList *C) {
    // 请在此编写你的代码：
    if(A->length + B->length > MaxSize) return false;
    int a = 0;
    int b = 0;
    while(a != A->length && b != B->length) {
        if(A->data[a] < B->data[b]) {
            C->data[a + b] = A->data[a];
            a++;
        }else {
            C->data[a + b] = B->data[b];
            b++;
        }
    }
    while(a != A->length) {
        C->data[a + b] = A->data[a];
        a++;  
    }
    while(b != B->length) {
        C->data[a + b] = B->data[b];
        b++;
    }
    C->length = a + b;
    return true;
}

// ============================================================================
//                          【自动化测试驱动区（无需修改）】
// ============================================================================

// 辅助打印函数
void PrintList(const SqList *L) {
    printf("[ ");
    for (int i = 0; i < L->length; i++) {
        printf("%d ", L->data[i]);
    }
    printf("] (长度: %d)\n", L->length);
}

// 比较数组是否一致
bool CheckEqual(const SqList *L, const int expected[], int expected_len) {
    if (L->length != expected_len) return false;
    for (int i = 0; i < expected_len; i++) {
        if (L->data[i] != expected[i]) return false;
    }
    return true;
}

int main() {
    printf("============= 顺序表功能自测开始 =============\n\n");

    // 测试 1：初始化与插入
    SqList L1;
    InitList(&L1);
    ListInsert(&L1, 1, 10);
    ListInsert(&L1, 2, 30);
    ListInsert(&L1, 2, 20); // 期望顺序: 10, 20, 30
    int exp1[] = {10, 20, 30};
    printf("测试 1 [初始化与插入]: %s\n", CheckEqual(&L1, exp1, 3) ? "PASS" : "FAIL");
    PrintList(&L1);

    // 测试 2：查找
    int pos = LocateElem(&L1, 20);
    printf("测试 2 [按值查找 20]: %s (查得位序: %d, 期望: 2)\n", pos == 2 ? "PASS" : "FAIL", pos);

    // 测试 3：删除
    int del_val = 0;
    ListDelete(&L1, 2, &del_val); // 删除第2位的20
    int exp3[] = {10, 30};
    printf("测试 3 [删除第2个元素]: %s (带出值: %d, 期望: 20)\n", 
           (CheckEqual(&L1, exp3, 2) && del_val == 20) ? "PASS" : "FAIL", del_val);
    PrintList(&L1);

    // 测试 4：原地逆置
    SqList L4 = {{1, 2, 3, 4, 5}, 5};
    Reverse(&L4);
    int exp4[] = {5, 4, 3, 2, 1};
    printf("测试 4 [原地逆置]: %s\n", CheckEqual(&L4, exp4, 5) ? "PASS" : "FAIL");
    PrintList(&L4);

    // 测试 5：有序表去重
    SqList L5 = {{1, 1, 2, 3, 3, 3, 4, 5, 5}, 9};
    DeleteDuplicates(&L5);
    int exp5[] = {1, 2, 3, 4, 5};
    printf("测试 5 [有序表去重]: %s\n", CheckEqual(&L5, exp5, 5) ? "PASS" : "FAIL");
    PrintList(&L5);

    // 测试 6：有序表合并
    SqList A = {{1, 3, 5, 7}, 4};
    SqList B = {{2, 4, 6, 8, 10}, 5};
    SqList C;
    InitList(&C);
    Merge(&A, &B, &C);
    int exp6[] = {1, 2, 3, 4, 5, 6, 7, 8, 10};
    printf("测试 6 [有序表合并]: %s\n", CheckEqual(&C, exp6, 9) ? "PASS" : "FAIL");
    PrintList(&C);

    printf("\n============= 自测流程结束 =============\n");
    return 0;
}