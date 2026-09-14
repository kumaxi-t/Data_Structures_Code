#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 10  // 定义栈中元素的最大容量

// 408 标准规范：顺序栈类型定义（top = -1 流派）
typedef struct {
    int data[MaxSize];      // 静态数组存放栈中元素
    int top;                // 栈顶指针（存储实际栈顶元素下标）
} SqStack;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化顺序栈
// 目标：将栈顶指针置为 -1
void InitStack(SqStack *S) {
    // 请在此编写你的代码：
    if(!S) return ;
    S->top = -1;
    // 冗余的内存分配，内存泄漏
    // SqStack* head = (SqStack*)malloc(sizeof(SqStack));
    // if(!head) return ;
    // head->top = -1;
    // *S = *head;
}

// 2. 判空操作
// 目标：检查栈是否为空
// 返回：为空返回 true，非空返回 false
bool StackEmpty(const SqStack *S) {
    // 请在此编写你的代码：
    if(!S || S->top == -1) return true;
    return false;
}

// 3. 判满操作
// 目标：检查栈是否已满（防止上溢）
// 返回：栈满返回 true，未满返回 false
bool StackFull(const SqStack *S) {
    // 请在此编写你的代码：
    if(!S) return false;
    return (S->top == MaxSize - 1);
    // 有点冗余
    // if(!S || StackEmpty(S)) return false;
    // if(S->top == MaxSize - 1) return true;
    // return false;
}

// 4. 入栈操作（Push）
// 目标：在栈满时安全拦截；未满时将元素 e 压入栈顶
// 返回：成功返回 true，栈满返回 false
bool Push(SqStack *S, int e) {
    // 请在此编写你的代码：
    // if (!S || !e || StackEmpty(S)) return false;   // 可以加上完整的接口防爆
    if(StackFull(S)) return false;
    S->top++;
    S->data[S->top] = e;
    return true;
}

// 5. 出栈操作（Pop）
// 目标：栈非空时将栈顶元素弹出，并通过指针 e 带回其值；栈空时安全拦截
// 返回：成功返回 true，栈空返回 false
bool Pop(SqStack *S, int *e) {
    // 请在此编写你的代码：
    // if (!S || !e || StackEmpty(S)) return false;   // 可以加上完整的接口防爆
    if(StackEmpty(S)) return false;
    *e = S->data[S->top];
    S->top--;
    return true;
}

// 6. 读取栈顶元素（GetTop）
// 目标：栈非空时通过指针 e 查阅当前栈顶值（不改变栈结构）；栈空时安全拦截
// 返回：成功返回 true，栈空返回 false
bool GetTop(const SqStack *S, int *e) {
    // 请在此编写你的代码：
    // if (!S || !e || StackEmpty(S)) return false;   // 可以加上完整的接口防爆
    if(StackEmpty(S)) return false;
    *e = S->data[S->top];
    return true;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void PrintStack(const SqStack *S) {
    if (S == NULL || S->top == -1) {
        printf("[Bottom] (空栈) [Top]\n");
        return;
    }
    printf("[Bottom] -> ");
    for (int i = 0; i <= S->top; i++) {
        printf("%d ", S->data[i]);
    }
    printf("<- [Top: index %d]\n", S->top);
}

int main() {
    printf("============= 考研 408 顺序栈标准自测开始 =============\n\n");

    // 1. 初始化与判空
    SqStack S;
    InitStack(&S);
    printf("测试 1 [初始化与判空]: %s\n", StackEmpty(&S) ? "PASS" : "FAIL");
    PrintStack(&S);

    // 2. 空栈防御：对空栈执行 Pop 与 GetTop 应安全返回 false
    int dummy = 0;
    bool pop_empty = Pop(&S, &dummy);
    bool get_empty = GetTop(&S, &dummy);
    printf("测试 2 [空栈防御拦截]: %s (空栈Pop: %s, 空栈GetTop: %s)\n",
           (!pop_empty && !get_empty) ? "PASS" : "FAIL",
           !pop_empty ? "正确拦截" : "异常穿透",
           !get_empty ? "正确拦截" : "异常穿透");

    // 3. 连续入栈
    Push(&S, 10);
    Push(&S, 20);
    Push(&S, 30);
    int top_val = 0;
    GetTop(&S, &top_val);
    printf("测试 3 [入栈与查栈顶]: %s (当前栈顶: %d, 期望: 30)\n",
           (top_val == 30) ? "PASS" : "FAIL", top_val);
    PrintStack(&S);

    // 4. 出栈与 LIFO 验证
    int pop_val1 = 0, pop_val2 = 0;
    Pop(&S, &pop_val1); // 弹出 30
    Pop(&S, &pop_val2); // 弹出 20
    GetTop(&S, &top_val);
    bool lifo_ok = (pop_val1 == 30 && pop_val2 == 20 && top_val == 10);
    printf("测试 4 [出栈与LIFO检验]: %s (弹1: %d, 弹2: %d, 新栈顶: %d)\n",
           lifo_ok ? "PASS" : "FAIL", pop_val1, pop_val2, top_val);
    PrintStack(&S);

    // 5. 溢出防御：填满栈并测试上溢拦截（容量为 MaxSize = 10）
    // 当前已有 1 个元素 (10)，再压入 9 个填满
    for (int i = 1; i <= 9; i++) {
        Push(&S, i * 100);
    }
    bool is_full = StackFull(&S);
    bool overflow_blocked = !Push(&S, 9999); // 栈已满，再插应返回 false
    printf("测试 5 [栈满与上溢防御]: %s (栈满检查: %s, 溢出入栈拦截: %s)\n",
           (is_full && overflow_blocked) ? "PASS" : "FAIL",
           is_full ? "判定为满" : "未满",
           overflow_blocked ? "正确拦截" : "异常溢出");
    PrintStack(&S);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}