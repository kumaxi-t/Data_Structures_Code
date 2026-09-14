#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 规范：带头结点的循环单链表类型定义
typedef struct LNode {
    int data;               // 数据域
    struct LNode *next;     // 指针域
} LNode, *LinkList;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化循环单链表
// 目标：动态申请头结点，将其 next 指向自己（L->next = L），形成闭环
// 返回：成功返回 true，申请失败返回 false
bool InitList(LinkList *L) {
    // 请在此编写你的代码：
    LinkList head = (LinkList)malloc(sizeof(LNode));
    if(!head) return false;
    head->next = head;
    *L = head;      // 致命错误，*L未赋值
    return true;
}

// 2. 判空操作
// 目标：判断循环单链表是否为空表
// 返回：为空返回 true，非空返回 false
bool Empty(LinkList L) {
    // 请在此编写你的代码：
    if(!L) return true;
    if(L->next == L) return true;
    return false;
}

// 3. 尾插法批量建表
// 目标：根据数组 a 中的 n 个元素，正序建立循环单链表，尾结点必须指向头结点 L
void CreateListTail(LinkList L, const int a[], int n) {
    // 请在此编写你的代码：
    // if(n <= 0) return ;
    if(n <= 0 || !L) return ;
    LinkList cur = L;
    for(int i = 0; i < n; i++) {
      // 不要写错malloc(sizeof(LinkList))
      LinkList node = (LinkList)malloc(sizeof(LNode));
      // 健壮性加上
      if(!node) return ;
      node->data = a[i];
      cur->next = node;
      cur = cur->next;
    }
    cur->next = L;
}

// 4. 按位序插入（408 标准基本操作）
// 目标：在带头结点的循环单链表 L 的第 i 个位置（位序从 1 开始）插入值为 e 的新节点
// 返回：位序合法插入成功返回 true，非法返回 false
// 注意：寻找第 i-1 个节点作为前驱，新节点的 next 正确接驳
bool ListInsert(LinkList L, int i, int e) {
    // 请在此编写你的代码：
    if(!L || i <= 0) return false;
    int count = 0;
    LinkList cur = L;
    while(cur->next != L && count < i - 1) {
    // while(cur != L->next && count < i - 1) {
        cur = cur->next;
        count++;    // 别忘记++
    }
    if(count != i - 1) return false;

    LinkList node = (LinkList)malloc(sizeof(LNode));
    if(!node) return false;     // 健壮性
    node->data = e;
    node->next = cur->next;
    cur->next = node;
    return true;


    // if(i <= 0) return false;
    // // LinkList node = (LinkList)malloc(sizeof(LinkList));    // 堆溢出
    // LinkList node = (LinkList)malloc(sizeof(LNode));
    // node->data = e;
    // LinkList cur = L;
    // i--;
    // while(i--) {
    //   cur = cur->next;
    // }
    // // node->next = cur->next->next;   // 错误：直接跳过了原先在第 i 个位置的节点！
    // node->next = cur->next;
    // cur->next = node;
    // return true;
}

// 5. 按位序删除（408 标准基本操作）
// 目标：删除第 i 个位置的节点，将被删节点的值通过指针 e 带回，并使用 free() 回收内存
// 返回：位序合法删除成功返回 true，非法返回 false
bool ListDelete(LinkList L, int i, int *e) {
    // 请在此编写你的代码：

    if(!L || i <= 0 || Empty(L)) return false;

    int count = 0;
    LinkList cur = L;
    while(cur->next != L && count < i - 1) {
        cur = cur->next;
        count++;
    }

    if(cur->next == L || count != i - 1) return false;

    *e = cur->next->data;
    LinkList nxt = cur->next;
    cur->next = cur->next->next;
    free(nxt);

    // 健壮性加上 
    nxt = NULL;
    
    return true;

    // if(Empty(L)) return false;
    // i--;
    // LinkList cur = L;
    // while(i--) {
    //   cur = cur->next;
    // }
    // *e = cur->next->data;
    // LinkList front = cur->next->next;
    // free(cur->next);
    // cur->next = front;
    // return true;
}

// 6. 按值查找（408 标准基本操作）
// 目标：查找链表中第一个值为 e 的数据节点
// 返回：找到返回该节点指针；未找到必须严格返回 NULL
LNode* LocateElem(LinkList L, int e) {
    // 请在此编写你的代码：

    // 健壮性检测别忘了
    if(!L || Empty(L)) return NULL;

    LinkList cur = L->next;
    while(cur != L) {
      if(cur->data == e) return cur;
      cur = cur->next;
    }
    return NULL;
}

// 7. 循环单链表原地逆置（408 高频大题题型）
// 目标：保留头结点 L，将所有数据节点原地逆置，逆置后的末尾节点必须重新闭环指向 L
// 约束：空间复杂度必须严格为 O(1)
void ReverseList(LinkList L) {
    // 请在此编写你的代码：
    // if(Empty(L)) return ;
    if(!L || Empty(L)) return ;
    LinkList cur = L->next;
    LinkList pre = L;
    LinkList nxt;
    while(cur != L) {
      nxt = cur->next;
      cur->next = pre;
      pre = cur;
      cur = nxt;
    }
    L->next = pre;
}

// ============================================================================
//                          【自动化闭环校验驱动区（无需修改）】
// ============================================================================

// 打印循环单链表
void PrintList(LinkList L) {
    if (L == NULL || L->next == L) {
        printf("[Head] -> [Head] (空循环单链表)\n");
        return;
    }
    printf("[Head] -> ");
    LNode *p = L->next;
    while (p != L) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("[Head] (闭环完成)\n");
}

// 自动化对比验证函数
bool CheckList(LinkList L, const int expected[], int n) {
    if (L == NULL) return false;
    LNode *p = L->next;

    for (int i = 0; i < n; i++) {
        if (p == L || p == NULL || p->data != expected[i]) return false;
        p = p->next;
    }
    // 恰好遍历 n 个有效节点后指回头结点
    return (p == L);
}

// 释放循环单链表内存
void FreeList(LinkList L) {
    if (L == NULL) return;
    LNode *p = L->next;
    while (p != L && p != NULL) {
        LNode *temp = p->next;
        free(p);
        p = temp;
    }
    free(L);
}

int main() {
    printf("============= 考研 408 循环单链表标准自测开始 =============\n\n");

    // 1. 初始化与判空
    LinkList L;
    InitList(&L);
    printf("测试 1 [初始化与判空]: %s\n", Empty(L) ? "PASS" : "FAIL");
    PrintList(L);

    // 2. 尾插法建表
    int arr[] = {10, 20, 30, 40};
    CreateListTail(L, arr, 4);
    printf("测试 2 [尾插法建表]: %s\n", CheckList(L, arr, 4) ? "PASS" : "FAIL");
    PrintList(L);

    // 3. 按位序插入：表头插 99，中间插 88
    ListInsert(L, 1, 99); // 位序 1 插入 99
    ListInsert(L, 4, 88); // 位序 4 插入 88
    int exp_ins[] = {99, 10, 20, 88, 30, 40};
    printf("测试 3 [按位序插入]: %s\n", CheckList(L, exp_ins, 6) ? "PASS" : "FAIL");
    PrintList(L);

    // 4. 按值查找
    LNode *pos_found = LocateElem(L, 88);
    LNode *pos_not_found = LocateElem(L, 999);
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
           (CheckList(L, exp_del, 4) && del_val == 88) ? "PASS" : "FAIL", del_val);
    PrintList(L);

    // 6. 原地逆置
    ReverseList(L);
    int exp_rev[] = {40, 30, 20, 10};
    printf("测试 6 [循环单链表原地逆置]: %s\n", CheckList(L, exp_rev, 4) ? "PASS" : "FAIL");
    PrintList(L);

    FreeList(L);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}