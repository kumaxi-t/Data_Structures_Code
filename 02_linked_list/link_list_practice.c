#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 带头结点的单链表类型定义
typedef struct Node {
    int data;           // 数据域
    struct Node *next;  // 指针域
} Node, *LinkList;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化链表：动态申请头结点，指针域置空并返回头指针
LinkList InitLink() {
    // 请在此编写你的代码：
    LinkList head = (LinkList)malloc(sizeof(Node));
    head->next = NULL;
    return head;
}

// 2. 头插法：在头结点后插入数据为 k 的新结点
// 特点：插入顺序与最终链表顺序相反
LinkList Head_insert(LinkList L, int k) {
    // 请在此编写你的代码：
    LinkList node = (LinkList)malloc(sizeof(Node));
    node->data = k;
    if(!L) return node;
    node->next = L->next;
    L->next = node;
    return L;
}

// 3. 尾插法：在链表末尾插入数据为 k 的新结点
// 提示：需要遍历找到当前的尾结点，再把新结点接在后面
LinkList Rear_insert(LinkList L, int k) {
    // 请在此编写你的代码：
    LinkList node = (LinkList)malloc(sizeof(Node));
    node->data = k;
    // node->next = NULL;    -->没写扣分
    if(!L) return node;
    LinkList cur = L;
    while(cur->next != NULL) cur = cur->next;
    cur->next = node;
    return L;
}

// 4. 按值查找结点：查找值为 x 的结点，找到返回该结点指针，未找到返回 NULL
Node* Find(LinkList L, int x) {
    // 请在此编写你的代码：
    // 408 明确规定带头结点的链表中，头结点不存储有效业务数据，其 data 域可能是未初始化的随机值
    // 如果头结点的脏数据恰好等于 x，就会误把头结点当作数据节点返回
    LinkList cur = L;
    // 修改--->LinkList cur = L->next;

    while(cur) {
      if(cur->data == x) return cur;
      cur = cur->next;
    }
    return NULL;
}

// 5. 在指定数据后插入：在值为 x 的结点后插入新结点 k
// 提示：先用 Find 找到目标结点，若未找到则不插入
LinkList Insert(LinkList L, int x, int k) {
    // 请在此编写你的代码：
    LinkList tar = Find(L, x);
    if(!tar) return L;
    LinkList node = (LinkList)malloc(sizeof(Node));
    // node->next = NULL;    -->没写扣分
    node->data = k;
    if(tar->next) {
      node->next = tar->next;
      tar->next = node;
    } else {
      tar->next = node;
    }
    // 冗余分支
    // node->next = tar->next;
    // tar->next = node;
    return L;
}

// 6. 按值删除结点：删除首个值为 k 的结点，并用 free() 释放内存
// 提示：需要维护前驱指针与当前指针，断链后释放目标结点
LinkList Delete(LinkList L, int k) {
    // 请在此编写你的代码：

    LinkList pre = L;
    LinkList cur = L->next;
    while(cur) {
      if(cur->data == k) {
        pre->next = cur->next;
        free(cur);
        break;
      }
      cur = cur->next;
      pre = pre->next;
    }
    return L;
    // 盲目使用虚拟头结点导致内存泄漏：链表本身已经自带头结点了，直接使用 Node *pre = L; Node *tar = L->next; 即可
    // 额外 malloc 了一个 dummy，却在退出函数时从未 free(dummy)，造成了确凿的内存泄漏。
    // 可能误删头结点：tar 从 L（头结点）开始遍历，如果头结点的脏数据等于 k，你的代码会把链表真正的头结点给 free 掉
    // LinkList pre = (LinkList)malloc(sizeof(Node));
    // LinkList dummy = pre;
    // pre->next = L;
    // LinkList tar = L;
    // while(tar) {
    //   if(tar->data == k) {
    //     pre->next = tar->next;
    //     free(tar);
    //     return dummy->next;
    //   }
    //   tar = tar->next;
    //   pre = pre->next;
    // }
    // return dummy->next;



}

// 7. 单链表原地逆置（408 大题高频考点）
// 目标：保留头结点，将后续数据结点就地逆置，空间复杂度要求严格为 O(1)
// 提示：可将链表断开为“头结点”和“剩余工作链”，逐个摘下结点进行头插
LinkList Reverse(LinkList L) {
    // 请在此编写你的代码：
    LinkList head = L;
    LinkList pre = NULL;
    LinkList cur = head->next;
    LinkList nxt;
    while(cur) {
      nxt = cur->next;
      cur->next = pre;
      pre = cur;
      cur = nxt;
    }
    head->next = pre;
    return head;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

// 辅助打印函数
void Printff(LinkList L) {
    if (L == NULL || L->next == NULL) {
        printf("[Head] -> NULL (空链表)\n");
        return;
    }
    printf("[Head] -> ");
    Node *p = L->next;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

// 验证链表元素序列是否与预期一致
bool CheckList(LinkList L, const int expected[], int n) {
    if (L == NULL) return false;
    Node *p = L->next;
    for (int i = 0; i < n; i++) {
        if (p == NULL || p->data != expected[i]) return false;
        p = p->next;
    }
    return p == NULL;
}

// 释放全链表空间
void FreeList(LinkList L) {
    Node *p = L;
    while (p != NULL) {
        Node *temp = p->next;
        free(p);
        p = temp;
    }
}

int main() {
    printf("============= 单链表全套手撕练习开始 =============\n\n");

    // 1. 初始化
    LinkList L = InitLink();

    // 2. 头插法测试：依次插入 6, 7, 1, 8
    L = Head_insert(L, 6);
    L = Head_insert(L, 7);
    L = Head_insert(L, 1);
    L = Head_insert(L, 8);
    int exp_head[] = {8, 1, 7, 6};
    printf("测试 1 [头插法构建]: %s\n", CheckList(L, exp_head, 4) ? "PASS" : "FAIL");
    Printff(L);

    // 3. 尾插法测试：在末尾追加 4, 9, 10
    L = Rear_insert(L, 4);
    L = Rear_insert(L, 9);
    L = Rear_insert(L, 10);
    int exp_rear[] = {8, 1, 7, 6, 4, 9, 10};
    printf("测试 2 [尾插法追加]: %s\n", CheckList(L, exp_rear, 7) ? "PASS" : "FAIL");
    Printff(L);

    // 4. 后继插入测试：在 6 后面插入 19，在 4 后面插入 12
    L = Insert(L, 6, 19);
    L = Insert(L, 4, 12);
    int exp_ins[] = {8, 1, 7, 6, 19, 4, 12, 9, 10};
    printf("测试 3 [指定值后插入]: %s\n", CheckList(L, exp_ins, 9) ? "PASS" : "FAIL");
    Printff(L);

    // 5. 删除测试：删除不存在的 100，再删除存在的 4
    L = Delete(L, 100);
    L = Delete(L, 4);
    int exp_del[] = {8, 1, 7, 6, 19, 12, 9, 10};
    printf("测试 4 [按值查找并删除]: %s\n", CheckList(L, exp_del, 8) ? "PASS" : "FAIL");
    Printff(L);

    // 6. 原地逆置测试
    L = Reverse(L);
    int exp_rev[] = {10, 9, 12, 19, 6, 7, 1, 8};
    printf("测试 5 [单链表原地逆置]: %s\n", CheckList(L, exp_rev, 8) ? "PASS" : "FAIL");
    Printff(L);

    // 回收内存
    FreeList(L);

    printf("\n============= 链表练习验证结束 =============\n");
    return 0;
}