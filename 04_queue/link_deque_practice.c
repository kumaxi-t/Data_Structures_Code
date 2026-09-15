#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 规范：双向链表结点定义
typedef struct DLNode {
    int data;               // 数据域
    struct DLNode *prior;   // 前驱指针
    struct DLNode *next;    // 后继指针
} DLNode;

// prior node next  prior 哨兵结点 next
//       left              right

// 408 规范：链式双端队列结构体定义
// 采用主流规范：l 指向最左端有效结点，r 指向最右端哨兵结点（区间 [l, r)）
typedef struct {
    DLNode *l;              // 左端指针
    DLNode *r;              // 右端指针（尾哨兵）
} Deque;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化双端队列
// 目标：申请一个尾哨兵结点，令 l 和 r 同时指向该哨兵，哨兵前驱后继置为 NULL
// 返回：成功返回 true，内存分配失败返回 false
bool InitDeque(Deque *Q) {
    // 请在此编写你的代码：
    if(!Q) return false;
    DLNode* node = (DLNode*)malloc(sizeof(DLNode));
    // 别忘了校验
    if(!node) return false;
    node->next = NULL;
    node->prior = NULL;
    Q->l = node;
    Q->r = node;
    return true;
}

// 2. 判空操作
// 目标：检查双端队列是否为空
// 返回：为空返回 true，非空返回 false
bool DequeEmpty(const Deque *Q) {
    // 请在此编写你的代码：
    if(!Q) return false;
    return (Q->l == Q->r);
    // return (Q->l->next == NULL);
}

// 3. 左端入队（LInsert）
// 目标：为元素 x 申请新结点，插在 l 的左侧，更新 l
// 返回：成功返回 true，内存分配失败返回 false
bool PushFront(Deque *Q, int x) {
    // 请在此编写你的代码：
    if(!Q) return false;
    DLNode* node = (DLNode*)malloc(sizeof(DLNode));
    if(!node) return false;
    node->data = x;
    node->prior = NULL;
    node->next = Q->l;
    Q->l->prior = node;
    Q->l = node;
    return true;
    // node->data = x;
    // Q->l->next = node;
    // node->prior = Q->l;
    // node->next = NULL;
    // Q->l = node;
    // return true;
}

// 4. 右端入队（RInsert）
// 目标：将元素 x 填入当前 r 哨兵，在 r 右侧新建哨兵并更新 r
// 返回：成功返回 true，内存分配失败返回 false
bool PushBack(Deque *Q, int x) {
    // 请在此编写你的代码：
    if(!Q) return false;
    DLNode* node = (DLNode*)malloc(sizeof(DLNode));
    if(!node) return false;
    Q->r->data = x;
    node->prior = Q->r;
    // next别忘了
    node->next = NULL;
    Q->r->next = node;
    Q->r = node;
    return true;
    // node->data = x;
    // node->prior = Q->r->prior;
    // node->next = Q->r;
    // // bug->>如果队列没有元素其前驱结点可能为空
    // Q->r->prior->next = node;
    // Q->r->prior = node;
    // return true;
}

// 5. 左端出队（LDelete）
// 目标：队非空时，删除最左端有效结点，通过指针 x 带回数据并更新 l；空队安全拦截
// 返回：成功返回 true，队空或非法返回 false
bool PopFront(Deque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    *x = Q->l->data;
    DLNode* del = Q->l;
    Q->l = del->next;
    Q->l->prior = NULL;
    free(del);
    del = NULL;
    return true;
    // Q->l = del->prior;
    // Q->l->prior = NULL;
    // free(del);
    // del = NULL;
    // return true;
}

// 6. 右端出队（RDelete）
// 目标：队非空时，取出最右端有效数据（r->prior->data），释放旧哨兵并更新 r；空队拦截
// 返回：成功返回 true，队空或非法返回 false
bool PopBack(Deque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    DLNode* del = Q->r->prior;
    *x = del->data;
    Q->r->prior = del->prior;
    if(del == Q->l) {
      Q->l = Q->r;
    }else {
      del->prior->next = Q->r;
    }
    free(del);
    del = NULL;
    return true;
}

// 7. 读取两端元素（GetFront / GetBack）
// 目标：队非空时查阅端点值（不修改队列结构）
bool GetFront(const Deque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    *x = Q->l->data;
    return true;
}

bool GetBack(const Deque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;    
    *x = Q->r->prior->data;
    return true;
}

// 8. 销毁并清空队列（DestroyDeque）
// 目标：释放包括尾哨兵在内的所有结点，并将 l 和 r 置为 NULL
void DestroyDeque(Deque *Q) {
    // 请在此编写你的代码：
    if(!Q) return ;
    DLNode* cur = Q->l;
    while(cur != NULL) {
      DLNode* nxt = cur->next;
      free(cur);
      cur = nxt;
    }
    Q->l = NULL;
    Q->r = NULL;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void PrintDeque(const Deque *Q) {
    if (Q == NULL || DequeEmpty(Q)) {
        printf("[Left] (空双端队列) [Right]\n");
        return;
    }
    printf("[Left] <-> ");
    DLNode *p = Q->l;
    while (p != Q->r) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("<-> [Right]\n");
}

int main() {
    printf("============= 考研 408 链式双端队列标准自测开始 =============\n\n");

    // 1. 初始化与判空
    Deque Q;
    bool init_ok = InitDeque(&Q);
    printf("测试 1 [初始化与判空]: %s\n", (init_ok && DequeEmpty(&Q)) ? "PASS" : "FAIL");
    PrintDeque(&Q);

    // 2. 空队防御拦截
    int dummy = 0;
    bool pop_l_fail = !PopFront(&Q, &dummy);
    bool pop_r_fail = !PopBack(&Q, &dummy);
    printf("测试 2 [空队两端防御拦截]: %s (左出拦截: %s, 右出拦截: %s)\n",
           (pop_l_fail && pop_r_fail) ? "PASS" : "FAIL",
           pop_l_fail ? "PASS" : "FAIL",
           pop_r_fail ? "PASS" : "FAIL");

    // 3. 两端交替入队验证：
    // 左端入 20, 左端入 10 ==> 此时队列: 10, 20
    // 右端入 30, 右端入 40 ==> 此时队列: 10, 20, 30, 40
    PushFront(&Q, 20);
    PushFront(&Q, 10);
    PushBack(&Q, 30);
    PushBack(&Q, 40);
    int front_val = 0, back_val = 0;
    GetFront(&Q, &front_val);
    GetBack(&Q, &back_val);
    bool insert_ok = (front_val == 10 && back_val == 40);
    printf("测试 3 [两端混合入队与查端值]: %s (左端: %d, 右端: %d)\n",
           insert_ok ? "PASS" : "FAIL", front_val, back_val);
    PrintDeque(&Q);

    // 4. 两端交替出队验证：
    // 左端出 10
    // 右端出 40
    int pop_val1 = 0, pop_val2 = 0;
    PopFront(&Q, &pop_val1);
    PopBack(&Q, &pop_val2);
    GetFront(&Q, &front_val);
    GetBack(&Q, &back_val);
    bool pop_ok = (pop_val1 == 10 && pop_val2 == 40 && front_val == 20 && back_val == 30);
    printf("测试 4 [两端混合出队检验]: %s (弹左: %d, 弹右: %d, 新左: %d, 新右: %d)\n",
           pop_ok ? "PASS" : "FAIL", pop_val1, pop_val2, front_val, back_val);
    PrintDeque(&Q);

    // 5. 边界出队清空与自愈：弹出剩余的 20, 30
    PopFront(&Q, &dummy); // 弹出 20
    PopBack(&Q, &dummy);  // 弹出 30
    bool empty_again = DequeEmpty(&Q);
    PushBack(&Q, 888);    // 清空后重新入队
    GetFront(&Q, &front_val);
    bool recover_ok = empty_again && (front_val == 888);
    printf("测试 5 [清空与重新入队自愈]: %s (清空判空: %s, 重新入队正常: %s)\n",
           recover_ok ? "PASS" : "FAIL",
           empty_again ? "成功" : "失败",
           (front_val == 888) ? "成功" : "失败");
    PrintDeque(&Q);

    // 6. 销毁队列
    DestroyDeque(&Q);
    printf("测试 6 [销毁与释放]: %s\n", (Q.l == NULL && Q.r == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}