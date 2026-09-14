#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 规范：带头结点的循环双链表类型定义
typedef struct DNode {
    int data;               // 数据域
    struct DNode *prior;    // 前驱指针（408教材惯用 prior，等价于 pre）
    struct DNode *next;     // 后继指针
} DNode, *DLinkList;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化循环双链表
// 目标：动态申请头结点，将其 next 与 prior 均指向自己，形成双向自环
// 返回：成功返回 true，内存分配失败返回 false
bool InitDList(DLinkList *L) {
    // 请在此编写你的代码：
    DLinkList head = (DLinkList)malloc(sizeof(DNode));
    if(!head) return false;
    head->next = head;
    head->prior = head;
    *L = head;
    return true;
}

// 2. 判空操作
// 目标：判断循环双链表是否为空表
// 返回：为空返回 true，非空返回 false
bool Empty(DLinkList L) {
    // 请在此编写你的代码：
    if(!L || L->next == L) return true;
    return false;
}

// 3. 尾插法批量建表（利用 L->prior 的 O(1) 特性）
// 目标：根据数组 a 中的 n 个元素正序尾插建表，尾结点与头结点 L 双向闭环
void CreateListTail(DLinkList L, const int a[], int n) {
    // 请在此编写你的代码：

    if(!L || n <= 0) return ;
    for(int i = 0; i < n; i++) {
      DLinkList node = (DLinkList)(malloc(sizeof(DNode)));
      if(!node) return ;
      node->data = a[i];
      DLinkList rear = L->prior;
      rear->next = node;
      node->prior = rear;
      node->next = L;
      L->prior = node;
    }
    // if(!L || n <= 0) return ;
    // DLinkList cur = L;
    // for(int i = 0; i < n; i++) {
    //   DLinkList node = (DLinkList)malloc(sizeof(DNode));
    //   if(!node) return ;
    //   node->data = a[i];
    //   cur->next = node;
    //   node->prior = cur;
    //   node->next = NULL;
    //   cur = cur->next;
    // }
    // cur->next = L;
    // L->prior = cur;
}

// 4. 按位序插入（408 标准基本操作）
// 目标：在带头结点的循环双链表 L 的第 i 个位置（位序 1 <= i <= length + 1）插入值为 e 的新结点
// 返回：位序合法插入成功返回 true，非法返回 false
// 注意：寻找第 i-1 个结点作为前驱，修改 4 根指针链
bool ListInsert(DLinkList L, int i, int e) {
    // 请在此编写你的代码：
    if(!L || i < 1) return false;
    int count = 0;
    DLinkList cur = L;
    while(cur->next != L && count < i - 1) {
      cur = cur->next;
      count++;
    }

    if(count != i - 1) return false;

    DLinkList node = (DLinkList)malloc(sizeof(DNode));
    if(!node) return false;
    node->data = e;
    cur->next->prior = node;
    node->next = cur->next;
    cur->next = node;
    node->prior = cur;
    return true;
}

// 5. 按位序删除（408 标准基本操作）
// 目标：删除第 i 个位置的结点，将被删结点的值通过指针 e 带回，并用 free() 回收内存
// 返回：位序合法删除成功返回 true，非法返回 false
bool ListDelete(DLinkList L, int i, int *e) {
    // 请在此编写你的代码：
    if(!L || i <= 0) return false;
    int count = 0;
    DLinkList cur = L;
    while(cur->next != L && count < i - 1) {
      cur = cur->next;
      count++;
    }
    if(cur->next == L || count != i - 1) return false;

    DLinkList del = cur->next;
    *e = del->data;
    cur->next = del->next;
    del->next->prior = cur;
    free(del);
    del = NULL;
    return true;
}

// 6. 按值查找（408 标准基本操作）
// 目标：查找链表中首个值为 e 的有效数据结点
// 返回：找到返回该结点指针；未找到必须严格返回 NULL
DNode* LocateElem(DLinkList L, int e) {
    // 请在此编写你的代码：
    if(!L || Empty(L)) return NULL;
    DLinkList cur = L->next;
    while(cur != L) {
      if(cur->data == e) return cur;
      cur = cur->next;
    }
    return NULL;
}

// 7. 循环双链表原地逆置（408 高频进阶考点）
// 目标：保留头结点 L，将所有数据结点原地逆置，且逆置后保持正确的 prior 与 next 双向闭环
// 约束：空间复杂度必须严格为 O(1)
// 提示：遍历链表（包含头结点或纯数据结点），将其 prior 和 next 指针逐个对调
void ReverseDList(DLinkList L) {
    // 请在此编写你的代码：
    // 最简单的做法，把每个结点的前后指针交换
    if(!L || Empty(L)) return ;
    DLinkList cur = L;
    do{
        DLinkList tmp = cur->next;
        cur->next = cur->prior;
        cur->prior = tmp;
        
        // 别忘了推进到下一个结点
        cur = cur->next;
    }while(cur != L);

    // if(!L || Empty(L)) return ;
    // DLinkList cur = L->next;
    // DLinkList pre = L;
    // DLinkList nxt;
    // L->prior = cur;
    // while(cur != L) {
    //   nxt = cur->next;
    //   cur->next = pre;
    //   //   pre->prior = cur;     // wrong
    //   cur->prior = nxt;
    //   pre = cur;
    //   cur = nxt;
    // }
    // L->next = pre;
}

// ============================================================================
//                          【自动化双向闭环校验驱动区（无需修改）】
// ============================================================================

// 打印循环双链表（正反双向闭环检查）
void PrintList(DLinkList L) {
    if (L == NULL || L->next == L) {
        printf("[Head] <=> [Head] (空循环双链表)\n");
        return;
    }
    printf("正向: [Head] <=> ");
    DNode *p = L->next;
    while (p != L) {
        printf("%d <=> ", p->data);
        p = p->next;
    }
    printf("[Head] (正向闭环完成)\n");

    printf("反向: [Head] <=> ");
    p = L->prior;
    while (p != L) {
        printf("%d <=> ", p->data);
        p = p->prior;
    }
    printf("[Head] (反向闭环完成)\n");
}

// 自动化对比验证函数（严格校验正向序列与反向序列）
bool CheckDList(DLinkList L, const int expected[], int n) {
    if (L == NULL) return false;
    DNode *p = L->next;

    // 1. 正向验证
    for (int i = 0; i < n; i++) {
        if (p == L || p == NULL || p->data != expected[i]) return false;
        p = p->next;
    }
    if (p != L) return false;

    // 2. 反向验证
    p = L->prior;
    for (int i = n - 1; i >= 0; i--) {
        if (p == L || p == NULL || p->data != expected[i]) return false;
        p = p->prior;
    }
    return (p == L);
}

// 释放循环双链表内存
void FreeList(DLinkList L) {
    if (L == NULL) return;
    DNode *p = L->next;
    while (p != L && p != NULL) {
        DNode *temp = p->next;
        free(p);
        p = temp;
    }
    free(L);
}

int main() {
    printf("============= 考研 408 循环双链表标准自测开始 =============\n\n");

    // 1. 初始化与判空
    DLinkList L;
    InitDList(&L);
    printf("测试 1 [初始化与判空]: %s\n", Empty(L) ? "PASS" : "FAIL");
    PrintList(L);

    // 2. 尾插法批量建表
    int arr[] = {10, 20, 30, 40};
    CreateListTail(L, arr, 4);
    printf("测试 2 [尾插法批量建表]: %s\n", CheckDList(L, arr, 4) ? "PASS" : "FAIL");
    PrintList(L);

    // 3. 按位序插入：表头插 99，中间插 88
    ListInsert(L, 1, 99); // 位序 1 插入 99
    ListInsert(L, 4, 88); // 位序 4 插入 88
    int exp_ins[] = {99, 10, 20, 88, 30, 40};
    printf("测试 3 [按位序插入]: %s\n", CheckDList(L, exp_ins, 6) ? "PASS" : "FAIL");
    PrintList(L);

    // 4. 按值查找
    DNode *pos_found = LocateElem(L, 88);
    DNode *pos_not_found = LocateElem(L, 999);
    bool find_pass = (pos_found != NULL && pos_found->data == 88) && (pos_not_found == NULL);
    printf("测试 4 [按值查找]: %s (找88: %s, 找999: %s)\n",
           find_pass ? "PASS" : "FAIL",
           pos_found ? "成功" : "失败",
           pos_not_found ? "异常非空" : "正确返回NULL");

    // 5. 按位序删除并回收内存
    int del_val = 0;
    ListDelete(L, 1, &del_val); // 删除第 1 位 (99)
    ListDelete(L, 3, &del_val); // 删除当前第 3 位 (88)
    int exp_del[] = {10, 20, 30, 40};
    printf("测试 5 [按位序删除并带出值]: %s (带出值: %d, 期望: 88)\n",
           (CheckDList(L, exp_del, 4) && del_val == 88) ? "PASS" : "FAIL", del_val);
    PrintList(L);

    // 6. 原地逆置
    ReverseDList(L);
    int exp_rev[] = {40, 30, 20, 10};
    printf("测试 6 [循环双链表原地逆置]: %s\n", CheckDList(L, exp_rev, 4) ? "PASS" : "FAIL");
    PrintList(L);

    FreeList(L);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}