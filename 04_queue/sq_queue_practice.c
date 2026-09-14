#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 5   // 队列数组大小（牺牲1个单元，实际容量为 MaxSize - 1 = 4）

// 408 标准规范：顺序循环队列类型定义
typedef struct {
    int data[MaxSize];  // 静态数组存放队列元素
    int front;          // 队头指针：指向队头元素
    int rear;           // 队尾指针：指向队尾元素的下一个空位
} SqQueue;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化循环队列
// 目标：将队头指针 front 和队尾指针 rear 均初始化为 0
void InitQueue(SqQueue *Q) {
    // 请在此编写你的代码：
    if(!Q) return ;
    Q->front = Q->rear = 0;
}

// 2. 判空操作
// 目标：检查队列是否为空
// 返回：为空返回 true，非空返回 false
bool QueueEmpty(const SqQueue *Q) {
    // 请在此编写你的代码：
    if(!Q || Q->front == Q->rear) return true;
    return false;
}

// 3. 判满操作（方案1：牺牲一个单元）
// 目标：检查队列是否已满
// 返回：队满返回 true，未满返回 false
bool QueueFull(const SqQueue *Q) {
    // 请在此编写你的代码：
    if(!Q) return false;
    return ((Q->rear + 1) % MaxSize == Q->front);
}

// 4. 求当前队列中有效元素的个数
// 目标：通过公式计算环形数组中的元素个数
// 返回：元素个数
int QueueLength(const SqQueue *Q) {
    // 请在此编写你的代码：
    if(!Q) return 0;
    return (Q->rear - Q->front + MaxSize) % MaxSize;
}

// 5. 入队操作（EnQueue）
// 目标：队满时安全拦截；未满时将元素 x 存入 rear 处，rear 循环后移
// 返回：成功返回 true，队满返回 false
bool EnQueue(SqQueue *Q, int x) {
    // 请在此编写你的代码：
    if(!Q || QueueFull(Q)) return false;
    // 先放进去再加，rear指向的地方没有数据
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;
    return true;
}

// 6. 出队操作（DeQueue）
// 目标：队非空时，将 front 处的队头数据通过指针 x 带回，front 循环后移；队空时安全拦截
// 返回：成功返回 true，队空返回 false
bool DeQueue(SqQueue *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || QueueEmpty(Q)) return false;
    *x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;
    return true;
}

// 7. 读取队头元素（GetHead）
// 目标：队非空时，通过指针 x 获取 front 处的队头数据（不改变队列结构）；队空拦截
// 返回：成功返回 true，队空返回 false
bool GetHead(const SqQueue *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || QueueEmpty(Q)) return false;
    *x = Q->data[Q->front];
    return true;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void PrintQueue(const SqQueue *Q) {
    if (Q == NULL || QueueEmpty(Q)) {
        printf("[Front] (空队列) [Rear]\n");
        return;
    }
    printf("[Front] -> ");
    int i = Q->front;
    while (i != Q->rear) {
        printf("%d ", Q->data[i]);
        i = (i + 1) % MaxSize;
    }
    printf("<- [Rear: index %d, Length: %d]\n", Q->rear, QueueLength(Q));
}

int main() {
    printf("============= 考研 408 循环队列标准自测开始 =============\n\n");

    // 1. 初始化与判空
    SqQueue Q;
    InitQueue(&Q);
    printf("测试 1 [初始化与判空]: %s\n", QueueEmpty(&Q) ? "PASS" : "FAIL");
    PrintQueue(&Q);

    // 2. 空队防御：空队执行出队与查队头应安全返回 false
    int dummy = 0;
    bool deq_empty = DeQueue(&Q, &dummy);
    bool get_empty = GetHead(&Q, &dummy);
    printf("测试 2 [空队防御拦截]: %s (空队DeQueue: %s, 空队GetHead: %s)\n",
           (!deq_empty && !get_empty) ? "PASS" : "FAIL",
           !deq_empty ? "正确拦截" : "异常穿透",
           !get_empty ? "正确拦截" : "异常穿透");

    // 3. 入队至满队测试（MaxSize=5，牺牲1个单元，压入 4 个元素即满）
    EnQueue(&Q, 10);
    EnQueue(&Q, 20);
    EnQueue(&Q, 30);
    EnQueue(&Q, 40);
    bool full_ok = QueueFull(&Q);
    bool overflow_blocked = !EnQueue(&Q, 50); // 第 5 个应被安全拦截
    int len = QueueLength(&Q);
    printf("测试 3 [牺牲单元判满与防溢出]: %s (判满: %s, 溢出拦截: %s, 当前长度: %d/期望: 4)\n",
           (full_ok && overflow_blocked && len == 4) ? "PASS" : "FAIL",
           full_ok ? "满" : "未满",
           overflow_blocked ? "成功拦截" : "异常穿透",
           len);
    PrintQueue(&Q);

    // 4. 出队与 FIFO 验证：弹出 2 个元素
    int val1 = 0, val2 = 0;
    DeQueue(&Q, &val1); // 出队 10
    DeQueue(&Q, &val2); // 出队 20
    int head_val = 0;
    GetHead(&Q, &head_val);
    bool fifo_ok = (val1 == 10 && val2 == 20 && head_val == 30);
    printf("测试 4 [出队与FIFO检验]: %s (出队1: %d, 出队2: %d, 新队头: %d)\n",
           fifo_ok ? "PASS" : "FAIL", val1, val2, head_val);
    PrintQueue(&Q);

    // 5. 环形回绕自测：再次压入 2 个元素，验证 rear 指针能否成功穿过数组末尾回绕到 0
    EnQueue(&Q, 500);
    EnQueue(&Q, 600);
    bool loop_ok = QueueFull(&Q) && (Q.rear < Q.front); // 此时 rear 应绕回到 front 前面
    printf("测试 5 [环形回绕检验]: %s (指针环形回绕成功: %s, 当前队头下标: %d, 队尾下标: %d)\n",
           loop_ok ? "PASS" : "FAIL",
           loop_ok ? "是" : "否",
           Q.front, Q.rear);
    PrintQueue(&Q);

    // 6. 全部出队清空
    int temp = 0;
    while (!QueueEmpty(&Q)) {
        DeQueue(&Q, &temp);
    }
    printf("测试 6 [全部清空自测]: %s\n", QueueEmpty(&Q) ? "PASS" : "FAIL");
    PrintQueue(&Q);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}