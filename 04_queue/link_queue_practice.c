#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 规范：链式队列结点定义
typedef struct QNode {
    int data;               // 数据域
    struct QNode *next;     // 指针域
} QNode;

// 408 规范：链式队列结构体定义
typedef struct {
    QNode *front;           // 队头指针：指向头结点，就是哨兵结点，本身不存储数据
    QNode *rear;            // 队尾指针：指向尾结点，就是尾结点本身，本身存储数据
} LinkQueue;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化链式队列
// 目标：动态申请头结点，令 front 和 rear 均指向头结点，头结点的 next 置为 NULL
// 返回：成功返回 true，内存分配失败返回 false
bool InitQueue(LinkQueue *Q) {
    // 请在此编写你的代码：
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if(!node) return false;
    node->next = NULL;
    Q->front = node;
    Q->rear = node;
    return true;
}

// 2. 判空操作
// 目标：检查链式队列是否为空
// 返回：为空返回 true，非空返回 false
bool QueueEmpty(const LinkQueue *Q) {
    // 请在此编写你的代码：
    if(!Q) return false;
    return (Q->front->next == NULL);
}

// 3. 入队操作（EnQueue）
// 目标：为元素 x 申请新结点，通过 rear 执行 O(1) 尾插法入队
// 返回：成功返回 true，内存分配失败返回 false
bool EnQueue(LinkQueue *Q, int x) {
    // 请在此编写你的代码：
    if(!Q) return false;
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if(!node) return false;
    node->data = x;
    node->next = NULL;
    Q->rear->next = node;
    Q->rear = node;

    // return false;    // 注意不要写错
    return true;
}

// 4. 出队操作（DeQueue）
// 目标：队非空时，将队头首元结点删除，通过指针 x 带回其数据并释放空间
// 特别注意：若出队的恰好是最后一个数据结点，必须维护 rear 指针！
// 返回：成功返回 true，队空或非法返回 false
bool DeQueue(LinkQueue *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || QueueEmpty(Q)) return false;
    QNode* out = Q->front->next;
    *x = out->data;
    // 特判要删除的元素是不是最后一个元素，此时rear指向的也是out，不能让rear指向一个非法的空间
    if(Q->rear == out) {
      Q->rear = Q->front;
    }
    Q->front->next = out->next;
    free(out);

    // return false;      // 注意不要写错
    return true;
}

// 5. 获取队头元素（GetHead）
// 目标：队非空时，通过指针 x 获取首元结点的值（不改变队列结构）
// 返回：成功返回 true，队空或非法返回 false
bool GetHead(const LinkQueue *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || QueueEmpty(Q)) return false;
    *x = Q->front->next->data;
    return true;
}

// 6. 销毁并清空队列（DestroyQueue）
// 目标：释放包括头结点在内的所有结点的内存，并将 front 和 rear 置为 NULL
void DestroyQueue(LinkQueue *Q) {
    // 请在此编写你的代码：

    if(!Q) return ;
    while(Q->front) {
      QNode* out = Q->front;
      Q->front = out->next;
      free(out);
    }
    Q->rear = NULL;

    // wrong
    // if(!Q || QueueEmpty(Q)) return ;
    // while(Q->front->next) {
    //   QNode* out = Q->front->next;
    //   Q->front->next = out->next;
    //   free(out);
    // }
    // free(Q->front->next);
    // Q->front = Q->rear = NULL;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void PrintQueue(const LinkQueue *Q) {
    if (Q == NULL || Q->front == NULL || Q->front->next == NULL) {
        printf("[Front] (空队列) [Rear]\n");
        return;
    }
    printf("[Front] -> ");
    QNode *p = Q->front->next;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("<- [Rear: %d]\n", Q->rear->data);
}

int main() {
    printf("============= 考研 408 链式队列标准自测开始 =============\n\n");

    // 1. 初始化与判空
    LinkQueue Q;
    bool init_ok = InitQueue(&Q);
    printf("测试 1 [初始化与判空]: %s\n", (init_ok && QueueEmpty(&Q)) ? "PASS" : "FAIL");
    PrintQueue(&Q);

    // 2. 空队防御：空队出队与查队头应安全返回 false
    int dummy = 0;
    bool deq_empty = DeQueue(&Q, &dummy);
    bool get_empty = GetHead(&Q, &dummy);
    printf("测试 2 [空队防御拦截]: %s (空队DeQueue: %s, 空队GetHead: %s)\n",
           (!deq_empty && !get_empty) ? "PASS" : "FAIL",
           !deq_empty ? "正确拦截" : "异常穿透",
           !get_empty ? "正确拦截" : "异常穿透");

    // 3. 连续入队：压入 10, 20, 30
    EnQueue(&Q, 10);
    EnQueue(&Q, 20);
    EnQueue(&Q, 30);
    int head_val = 0;
    GetHead(&Q, &head_val);
    printf("测试 3 [连续入队与查队头]: %s (当前队头: %d, 期望: 10)\n",
           (head_val == 10) ? "PASS" : "FAIL", head_val);
    PrintQueue(&Q);

    // 4. 出队与 FIFO 检验
    int val1 = 0, val2 = 0;
    DeQueue(&Q, &val1); // 出队 10
    DeQueue(&Q, &val2); // 出队 20
    GetHead(&Q, &head_val);
    bool fifo_ok = (val1 == 10 && val2 == 20 && head_val == 30);
    printf("测试 4 [出队与FIFO检验]: %s (出队1: %d, 出队2: %d, 新队头: %d)\n",
           fifo_ok ? "PASS" : "FAIL", val1, val2, head_val);
    PrintQueue(&Q);

    // 5. 边界考查：出队最后一个元素并再次入队（检验 rear 野指针防范）
    int last_val = 0;
    DeQueue(&Q, &last_val); // 出队 30，此时队列应清空
    bool empty_ok = QueueEmpty(&Q);
    EnQueue(&Q, 999);       // 重新入队 999，验证 rear 能否正常接驳
    GetHead(&Q, &head_val);
    bool boundary_ok = empty_ok && (head_val == 999);
    printf("测试 5 [尾结点出队与指针自愈]: %s (单结点出队后判空: %s, 重新入队成功: %s)\n",
           boundary_ok ? "PASS" : "FAIL",
           empty_ok ? "成功" : "失败",
           (head_val == 999) ? "成功" : "失败");
    PrintQueue(&Q);

    // 6. 销毁队列
    DestroyQueue(&Q);
    printf("测试 6 [销毁与释放]: %s\n", (Q.front == NULL && Q.rear == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}