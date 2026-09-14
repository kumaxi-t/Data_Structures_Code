#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 规范：带头结点的链栈结点与指针类型定义
typedef struct SNode {
    int data;               // 数据域
    struct SNode *next;     // 指针域
} SNode, *LinkStack;

// ============================================================================
//                               【核心算法手写区】
// ============================================================================

// 1. 初始化链栈
// 目标：动态申请头结点，将其 next 置为 NULL
// 返回：成功返回 true，内存分配失败返回 false
bool InitStack(LinkStack *S) {
    // 请在此编写你的代码：
    LinkStack head = (LinkStack)malloc(sizeof(SNode));
    if(!head) return false;
    head->next = NULL;
    *S = head;
    return true;
}

// 2. 判空操作
// 目标：判断链栈是否为空栈
// 返回：为空返回 true，非空返回 false
bool StackEmpty(LinkStack S) {
    // 请在此编写你的代码：
    if(!S || S->next == NULL) return true;
    return false;
}

// 3. 入栈操作（进栈）
// 目标：在栈顶（头结点之后）插入数据为 x 的新结点（本质是单链表头插法）
// 返回：成功返回 true，内存分配失败返回 false
bool Push(LinkStack S, int x) {
    // 请在此编写你的代码：
    LinkStack node = (LinkStack)malloc(sizeof(SNode));
    if(!node) return false;
    node->data = x;
    node->next = S->next;
    S->next = node;
    return true;


    // 全写错了，链栈头指针指向的是栈顶元素（悲）
    // LinkStack node = (LinkStack)malloc(sizeof(SNode));
    // if(!node) return false;
    // node->data = x;
    // node->next = NULL;
    // LinkStack cur = S;
    // while(cur->next != NULL) {
    //   cur = cur->next;
    // }
    // cur->next = node;
    // return true;
}

// 4. 出栈操作
// 目标：栈非空时，将栈顶结点弹出，并通过指针 x 带回其数据，释放该结点空间
// 返回：出栈成功返回 true；栈空或非法返回 false
bool Pop(LinkStack S, int *x) {
    // 请在此编写你的代码：

    if(StackEmpty(S)) return false;
    LinkStack del = S->next;
    *x = del->data;
    S->next = del->next;
    free(del);
    return true;


    // if(StackEmpty(S)) return false;
    // LinkStack cur = S;
    // while(cur->next->next != NULL) {
    //   cur = cur->next;
    // }
    // *x = cur->next->data;
    // free(cur->next);
    // cur->next = NULL;
    // return true;
}

// 5. 读取栈顶元素
// 目标：栈非空时，通过指针 x 获取当前栈顶元素的值（不改变栈结构）
// 返回：读取成功返回 true；栈空或非法返回 false
bool GetTop(LinkStack S, int *x) {
    // 请在此编写你的代码：

    if(StackEmpty(S)) return false;
    *x = S->next->data;
    return true;

    // if(StackEmpty(S)) return false;
    // LinkStack cur = S;
    // while(cur->next != NULL) {
    //   cur = cur->next;
    // }
    // *x = cur->data;
    // return true;
}

// 6. 销毁并清空链栈
// 目标：释放链栈中所有数据结点及头结点的内存，并将头指针置为 NULL
void DestroyStack(LinkStack *S) {
    // 请在此编写你的代码：
    
    if (!S || !(*S)) return; // 健壮性防御二级指针与一级指针
    // if(!(*S)) return ;
    LinkStack cur = (*S)->next;
    LinkStack pre = *S;
    while(cur) {
      free(pre);
      pre = cur;
      cur = cur->next;
    }
    // 忘了free最后一个结点
    free(pre);
    // pre = NULL;
    *S = NULL;

    // if((*S)->next == NULL) {
    //   *S = NULL;
    //   return ;
    // }
    // LinkStack pre = (*S);
    // LinkStack cur = (*S)->next;
    // while(cur) {
    //   free(pre);
    //   pre = cur;
    //   cur = cur->next;
    // }
    // pre = NULL;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

// 辅助打印当前栈内元素（从栈顶到栈底）
void PrintStack(LinkStack S) {
    if (S == NULL || S->next == NULL) {
        printf("[Top] -> NULL (空栈)\n");
        return;
    }
    printf("[Top] -> ");
    SNode *p = S->next;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("[Bottom]\n");
}

int main() {
    printf("============= 考研 408 链栈标准自测开始 =============\n\n");

    // 1. 初始化与判空
    LinkStack S;
    bool init_ok = InitStack(&S);
    printf("测试 1 [初始化与判空]: %s\n", (init_ok && StackEmpty(S)) ? "PASS" : "FAIL");
    PrintStack(S);

    // 2. 空栈防御测试：空栈执行 Pop 与 GetTop 应安全返回 false
    int dummy_val = 0;
    bool pop_empty = Pop(S, &dummy_val);
    bool get_empty = GetTop(S, &dummy_val);
    printf("测试 2 [空栈防御测试]: %s (空栈Pop: %s, 空栈GetTop: %s)\n",
           (!pop_empty && !get_empty) ? "PASS" : "FAIL",
           !pop_empty ? "正确拒绝" : "异常通过",
           !get_empty ? "正确拒绝" : "异常通过");

    // 3. 入栈测试：依次压入 10, 20, 30, 40
    Push(S, 10);
    Push(S, 20);
    Push(S, 30);
    Push(S, 40);
    int top_val = 0;
    GetTop(S, &top_val);
    printf("测试 3 [连续入栈与读栈顶]: %s (当前栈顶: %d, 期望: 40)\n",
           (top_val == 40) ? "PASS" : "FAIL", top_val);
    PrintStack(S);

    // 4. 出栈测试：弹出 2 个元素，验证 LIFO（后进先出）特性
    int pop1 = 0, pop2 = 0;
    Pop(S, &pop1); // 弹出 40
    Pop(S, &pop2); // 弹出 30
    GetTop(S, &top_val);
    bool pop_ok = (pop1 == 40 && pop2 == 30 && top_val == 20);
    printf("测试 4 [出栈与LIFO检验]: %s (弹出1: %d, 弹出2: %d, 新栈顶: %d)\n",
           pop_ok ? "PASS" : "FAIL", pop1, pop2, top_val);
    PrintStack(S);

    // 5. 弹空至空栈
    Pop(S, &pop1); // 弹出 20
    Pop(S, &pop2); // 弹出 10
    printf("测试 5 [全部弹空检验]: %s\n", StackEmpty(S) ? "PASS" : "FAIL");
    PrintStack(S);

    // 6. 销毁链栈
    DestroyStack(&S);
    printf("测试 6 [销毁与释放]: %s\n", (S == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}