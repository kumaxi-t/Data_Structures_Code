#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 5   // 牺牲一个单元，队列最大容量为 MaxSize - 1 = 4

// 408 统编标准定义：顺序双端队列
typedef struct {
    int data[MaxSize];  // 静态数组存放队列元素
    int front;          // 队头指针：指向队头元素
    int rear;           // 队尾指针：指向队尾元素的下一个空闲位置
} SqDeque;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化队列
// 目标：令 front 和 rear 均初始化为 0
void InitDeque(SqDeque *Q) {
    // 请在此编写你的代码：
    if(!Q) return ;
    Q->front = Q->rear = 0;
}

// 2. 判空操作
// 目标：两指针相遇即为空
// 返回：为空返回 true，非空返回 false
bool DequeEmpty(const SqDeque *Q) {
    // 请在此编写你的代码：
    if(!Q) return true;
    return (Q->front == Q->rear);
}

// 3. 判满操作（牺牲一个单元）
// 目标：rear 顺时针方向的下一个位置是 front 则满
// 返回：队满返回 true，未满返回 false
bool DequeFull(const SqDeque *Q) {
    // 请在此编写你的代码：
    if(!Q) return false;
    return (Q->rear + 1) % MaxSize == Q->front;
}

// 4. 求当前队列中有效元素的个数
// 目标：通过环形取模公式计算队长
// 返回：元素个数
int DequeLength(const SqDeque *Q) {
    // 请在此编写你的代码：
    if(!Q || DequeEmpty(Q)) return 0;
    return (Q->rear - Q->front + MaxSize) % MaxSize;
}

// 5. 队尾入队（PushRear）
// 目标：队满时安全拦截；未满时将元素 x 存入当前 rear 处，rear 顺时针后移
// 返回：成功返回 true，队满返回 false
bool PushRear(SqDeque *Q, int x) {
    // 请在此编写你的代码：
    if(!Q || DequeFull(Q)) return false;
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;
    return true;
}

// 6. 队头入队（PushFront）
// 目标：队满时安全拦截；未满时 front 先逆时针前移让出新空位，再存入元素 x
// 返回：成功返回 true，队满返回 false
bool PushFront(SqDeque *Q, int x) {
    // 请在此编写你的代码：
    if(!Q || DequeFull(Q)) return false;
    Q->front = (Q->front - 1 + MaxSize) % MaxSize;
    Q->data[Q->front] = x;
    return true;
}

// 7. 队头出队（PopFront）
// 目标：队非空时，将 front 处的队头数据通过指针 x 带回，front 顺时针后移；空队安全拦截
// 返回：成功返回 true，队空返回 false
bool PopFront(SqDeque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    *x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;
    return true;
}

// 8. 队尾出队（PopRear）
// 目标：队非空时，rear 先逆时针回退到真正的数据位置，再取出数据通过指针 x 带回；空队安全拦截
// 返回：成功返回 true，队空返回 false
bool PopRear(SqDeque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    Q->rear = (Q->rear - 1 + MaxSize) % MaxSize;
    *x = Q->data[Q->rear];
    return true;
}

// 9. 读取队头元素（GetFront）
// 目标：队非空时，通过指针 x 获取 front 处的队头数据（只读不改指针）；空队拦截
// 返回：成功返回 true，队空返回 false
bool GetFront(const SqDeque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    *x = Q->data[Q->front];
    return true;
}

// 10. 读取队尾元素（GetRear）
// 目标：队非空时，通过指针 x 获取队尾有效数据（只读不改指针）；空队拦截
// 返回：成功返回 true，队空返回 false
bool GetRear(const SqDeque *Q, int *x) {
    // 请在此编写你的代码：
    if(!Q || !x || DequeEmpty(Q)) return false;
    *x = Q->data[(Q->rear - 1 + MaxSize) % MaxSize];
    return true;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void PrintDeque(const SqDeque *Q) {
    if (Q == NULL || DequeEmpty(Q)) {
        printf("[Front: %d] (空双端队列) [Rear: %d]\n", Q ? Q->front : -1, Q ? Q->rear : -1);
        return;
    }
    printf("[Front: %d] -> ", Q->front);
    int cur = Q->front;
    while (cur != Q->rear) {
        printf("%d ", Q->data[cur]);
        cur = (cur + 1) % MaxSize;
    }
    printf("<- [Rear: %d | Length: %d]\n", Q->rear, DequeLength(Q));
}

int main() {
    printf("============= 考研 408 顺序循环双端队列标准自测开始 =============\n\n");

    // 1. 初始化与判空
    SqDeque Q;
    InitDeque(&Q);
    printf("测试 1 [初始化与判空]: %s\n", DequeEmpty(&Q) ? "PASS" : "FAIL");
    PrintDeque(&Q);

    // 2. 空队两端出队与窥探防御拦截
    int dummy = 0;
    bool pop_front_fail = !PopFront(&Q, &dummy);
    bool pop_rear_fail  = !PopRear(&Q, &dummy);
    bool get_front_fail = !GetFront(&Q, &dummy);
    bool get_rear_fail  = !GetRear(&Q, &dummy);
    bool test2_ok = pop_front_fail && pop_rear_fail && get_front_fail && get_rear_fail;
    printf("测试 2 [空队防御拦截]: %s (头出拦截: %s, 尾出拦截: %s, 窥头拦截: %s, 窥尾拦截: %s)\n",
           test2_ok ? "PASS" : "FAIL",
           pop_front_fail ? "PASS" : "FAIL",
           pop_rear_fail  ? "PASS" : "FAIL",
           get_front_fail ? "PASS" : "FAIL",
           get_rear_fail  ? "PASS" : "FAIL");

    // 3. 两端交替入队至满队（容量 MaxSize=5，牺牲1个单元，实际容纳 4 个）
    PushRear(&Q, 20);
    PushFront(&Q, 10);
    PushRear(&Q, 30);
    PushFront(&Q, 5);
    
    bool full_ok = DequeFull(&Q);
    bool overflow_blocked = !PushRear(&Q, 999) && !PushFront(&Q, 999);
    int front_val = 0, rear_val = 0;
    GetFront(&Q, &front_val);
    GetRear(&Q, &rear_val);
    int len = DequeLength(&Q);

    bool test3_ok = full_ok && overflow_blocked && (front_val == 5) && (rear_val == 30) && (len == 4);
    printf("测试 3 [两端入队与牺牲单元判满]: %s (判满: %s, 溢出拦截: %s, 队头: %d, 队尾: %d, 队长: %d/期望: 4)\n",
           test3_ok ? "PASS" : "FAIL",
           full_ok ? "满" : "未满",
           overflow_blocked ? "成功拦截" : "异常穿透",
           front_val, rear_val, len);
    PrintDeque(&Q);

    // 4. 两端交替出队
    int pop_val1 = 0, pop_val2 = 0;
    PopFront(&Q, &pop_val1); // 出队 5
    PopRear(&Q, &pop_val2);  // 出队 30
    GetFront(&Q, &front_val);
    GetRear(&Q, &rear_val);
    bool test4_ok = (pop_val1 == 5 && pop_val2 == 30 && front_val == 10 && rear_val == 20 && DequeLength(&Q) == 2);
    printf("测试 4 [两端交替出队检验]: %s (弹头: %d, 弹尾: %d, 新队头: %d, 新队尾: %d)\n",
           test4_ok ? "PASS" : "FAIL", pop_val1, pop_val2, front_val, rear_val);
    PrintDeque(&Q);

    // 5. 逆时针环形回绕自测（检验负数取模安全性）
    PushFront(&Q, 100);
    PushFront(&Q, 200);
    GetFront(&Q, &front_val);
    bool test5_ok = DequeFull(&Q) && (front_val == 200);
    printf("测试 5 [逆时针环形回绕自测]: %s (环形回绕成功: %s, 当前队头: %d, front下标: %d, rear下标: %d)\n",
           test5_ok ? "PASS" : "FAIL",
           test5_ok ? "是" : "否",
           front_val, Q.front, Q.rear);
    PrintDeque(&Q);

    // 6. 全部出队清空与自愈
    int temp = 0;
    while (!DequeEmpty(&Q)) {
        PopFront(&Q, &temp);
    }
    bool empty_ok = DequeEmpty(&Q);
    PushRear(&Q, 666);
    GetFront(&Q, &front_val);
    bool test6_ok = empty_ok && (front_val == 666);
    printf("测试 6 [清空与重新入队自愈]: %s (清空判空: %s, 重新入队正常: %s)\n",
           test6_ok ? "PASS" : "FAIL",
           empty_ok ? "成功" : "失败",
           (front_val == 666) ? "成功" : "失败");
    PrintDeque(&Q);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}