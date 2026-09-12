#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 纯 C 双链表标准结构体定义
typedef struct Node {
    int data;               // 数据域
    struct Node *next;      // 后继指针
    struct Node *pre;       // 前驱指针
} Node, *DLinkList;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化双链表
// 目标：动态申请头结点，将其 next 与 pre 均置为 NULL 并返回
DLinkList InitDLink() {
    // 请在此编写你的代码：
    DLinkList head = (DLinkList)malloc(sizeof(Node));
    if(!head) return NULL;
    head->next = NULL;
    head->pre = NULL;
    return head;
}

// 2. 双链表头插法
// 目标：在头结点 L 之后插入值为 k 的新结点
// 注意：必须维护双向指针关系，小心处理 L->next 为 NULL 的初始情况
DLinkList Head_insert(DLinkList L, int k) {
    // 请在此编写你的代码：
    DLinkList node = (DLinkList)malloc(sizeof(Node));
    node->next = NULL;
    node->pre = NULL;
    node->data = k;
    if(L->next) {
      node->next = L->next;
      L->next->pre = node;
    }
    L->next = node;
    node->pre = L;
    return L;
}

// 3. 双链表尾插法
// 目标：在双链表末尾追加值为 k 的新结点
// 注意：找到尾结点后，必须确保新尾结点的 next 为 NULL，pre 指向前驱
DLinkList Rear_insert(DLinkList L, int k) {
    // 请在此编写你的代码：
    DLinkList cur = L;
    while(cur->next) {
      cur = cur->next;
    }
    // DLinkList node = (DLinkList)malloc(sizeof(DLinkList));
    // DLinkList 是指针类型， 要传Node结构体
    DLinkList node = (DLinkList)malloc(sizeof(Node));
    node->data = k;
    node->next = NULL;
    node->pre = NULL;
    
    cur->next = node;
    node->pre = cur;
    return L;
}

// 4. 按值查找结点
// 目标：查找值为 x 的首个有效数据结点，找到返回该结点指针，未找到返回 NULL
Node* Find(DLinkList L, int x) {
    // 请在此编写你的代码：
    // DLinkList cur = L;   ----> 语义错误及潜在bug

    DLinkList cur = L->next;
    // 从首个元素开始遍历
    while(cur) {
      if(cur->data == x) {
        return cur;
      }
      cur = cur->next;    // 漏写了（悲）
    }
    return NULL;
}

// 5. 在指定元素后插入
// 目标：在值为 x 的结点后插入新结点 k
// 提示：先通过 Find 找到结点 p，若 p 存在，修改 4 个指针链（p, s, 以及 p 原后继）
DLinkList Insert(DLinkList L, int x, int k) {
    // 请在此编写你的代码：
    DLinkList tar = Find(L, x);

    // 没写扣分 
    if(!tar) return L;

    // DLinkList node = (DLinkList)malloc(sizeof(DLinkList));
    // DLinkList 是指针类型， 要传Node结构体
    DLinkList node = (DLinkList)malloc(sizeof(Node));
    node->data = k;
    node->next = NULL;
    node->pre = NULL;
    if(tar->next) {
      tar->next->pre = node;
      node->next = tar->next;
    }
    tar->next = node;
    node->pre = tar;
    return L;
}

// 6. 按值删除结点
// 目标：删除链表中首个值为 k 的有效数据结点，释放该节点空间
// 提示：先通过 Find 找到结点 p，修改 p->pre 的后继与 p->next 的前驱，再 free(p)
DLinkList Delete(DLinkList L, int k) {
    // 请在此编写你的代码：
    // DLinkList cur = L;   ----> 语义错误及潜在bug

    DLinkList cur = L->next;
    // 从首个元素开始遍历

    while(cur) {
      if(cur->data == k) {
        cur->pre->next = cur->next;
        if(cur->next) {
          cur->next->pre = cur->pre;
        }
        free(cur);
        // break忘了
        break;
      }
      cur = cur->next;     // 又漏写了（悲）
    }
    return L;
}

// 7. 双链表原地逆置（408 高频进阶）
// 目标：空间复杂度 O(1)，将整条双链表就地逆置，并保持正确的 pre 和 next 关系
// 提示：遍历链表，将每个结点的 pre 和 next 指针相互交换
DLinkList Reverse(DLinkList L) {
    // 请在此编写你的代码：
    // --->扣分 缺少边界特判
    if(!L || !L->next) return L;

    DLinkList cur = L->next;
    DLinkList front = NULL;
    DLinkList nxt;
    while(cur) {
      nxt = cur->next;
      cur->next = front;
      cur->pre = nxt;
      front = cur;
      cur = nxt;
    }
    L->next = front;
    if(front) front->pre = L;  // 加上判断防止front为NULL
    return L;
}

// ============================================================================
//                          【自动化双向校验驱动区（无需修改）】
// ============================================================================

// 打印双链表（同时验证正向与反向完整性）
void Printff(DLinkList L) {
    if (L == NULL || L->next == NULL) {
        printf("[Head] <=> NULL (空双链表)\n");
        return;
    }
    printf("正向: [Head] <=> ");
    Node *p = L->next;
    Node *tail = NULL;
    while (p != NULL) {
        printf("%d <=> ", p->data);
        tail = p;
        p = p->next;
    }
    printf("NULL\n");

    printf("反向: [Tail] <=> ");
    p = tail;
    while (p != NULL && p != L) {
        printf("%d <=> ", p->data);
        p = p->pre;
    }
    printf("[Head]\n");
}

// 严格双向比对验证
bool CheckDList(DLinkList L, const int expected[], int n) {
    if (L == NULL) return false;
    Node *p = L->next;
    Node *tail = NULL;

    // 1. 正向比对
    for (int i = 0; i < n; i++) {
        if (p == NULL || p->data != expected[i]) return false;
        tail = p;
        p = p->next;
    }
    if (p != NULL) return false;

    // 2. 反向指针有效性比对
    p = tail;
    for (int i = n - 1; i >= 0; i--) {
        if (p == NULL || p->data != expected[i]) return false;
        p = p->pre;
    }
    return (p == L); // 反向必须一路无误地退回到头结点 L
}

// 释放整条双链表
void FreeList(DLinkList L) {
    Node *p = L;
    while (p != NULL) {
        Node *temp = p->next;
        free(p);
        p = temp;
    }
}

int main() {
    printf("============= 双链表核心操作与双向自测开始 =============\n\n");

    // 1. 初始化
    DLinkList L = InitDLink();

    // 2. 头插法测试：依次插入 6, 7, 1, 8
    L = Head_insert(L, 6);
    L = Head_insert(L, 7);
    L = Head_insert(L, 1);
    L = Head_insert(L, 8);
    int exp_head[] = {8, 1, 7, 6};
    printf("测试 1 [头插法构建]: %s\n", CheckDList(L, exp_head, 4) ? "PASS" : "FAIL");
    Printff(L);

    // 3. 尾插法测试：在末尾追加 4, 9, 10
    L = Rear_insert(L, 4);
    L = Rear_insert(L, 9);
    L = Rear_insert(L, 10);
    int exp_rear[] = {8, 1, 7, 6, 4, 9, 10};
    printf("测试 2 [尾插法追加]: %s\n", CheckDList(L, exp_rear, 7) ? "PASS" : "FAIL");
    Printff(L);

    // 4. 指定位置后插入：在 6 后面插入 19，在 4 后面插入 12
    L = Insert(L, 6, 19);
    L = Insert(L, 4, 12);
    int exp_ins[] = {8, 1, 7, 6, 19, 4, 12, 9, 10};
    printf("测试 3 [指定值后插入]: %s\n", CheckDList(L, exp_ins, 9) ? "PASS" : "FAIL");
    Printff(L);

    // 5. 删除测试：删除不存在的 100，再删除存在的 4
    L = Delete(L, 100);
    L = Delete(L, 4);
    int exp_del[] = {8, 1, 7, 6, 19, 12, 9, 10};
    printf("测试 4 [按值查找并删除]: %s\n", CheckDList(L, exp_del, 8) ? "PASS" : "FAIL");
    Printff(L);

    // 6. 原地逆置测试
    L = Reverse(L);
    int exp_rev[] = {10, 9, 12, 19, 6, 7, 1, 8};
    printf("测试 5 [双链表原地逆置]: %s\n", CheckDList(L, exp_rev, 8) ? "PASS" : "FAIL");
    Printff(L);

    // 回收内存
    FreeList(L);

    printf("\n============= 双链表测试验证结束 =============\n");
    return 0;
}