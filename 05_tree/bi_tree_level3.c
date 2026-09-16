#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 408 统编规范：二叉链表结点定义
typedef struct BiTNode {
    char data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
} BiTNode, *BiTree;

// 辅助栈容量定义
#define StackSize 50

typedef struct {
    BiTree data[StackSize];
    int top;
} SqStack;

void InitStack(SqStack *S) {
    S->top = -1;
}

bool StackEmpty(const SqStack *S) {
    return S->top == -1;
}

bool Push(SqStack *S, BiTree node) {
    if (S->top == StackSize - 1) return false;
    S->data[++(S->top)] = node;
    return true;
}

bool Pop(SqStack *S, BiTree *node) {
    if (StackEmpty(S)) return false;
    *node = S->data[(S->top)--];
    return true;
}

bool GetTop(const SqStack *S, BiTree *node) {
    if (StackEmpty(S)) return false;
    *node = S->data[S->top];
    return true;
}

// 辅助建树函数（先序扩展字符串建树）
BiTree CreateBiTree(const char *str, int *index) {
    if (!str || !index || str[*index] == '\0' || str[*index] == '#') {
        if (str && index && str[*index] == '#') (*index)++;
        return NULL;
    }
    BiTree node = (BiTree)malloc(sizeof(BiTNode));
    if (!node) return NULL;
    node->data = str[*index];
    (*index)++;
    node->lchild = CreateBiTree(str, index);
    node->rchild = CreateBiTree(str, index);
    return node;
}

// 递归整树销毁
void DestroyTree(BiTree *T) {
    if (!T || !(*T)) return;
    DestroyTree(&((*T)->lchild));
    DestroyTree(&((*T)->rchild));
    free(*T);
    *T = NULL;
}

// ============================================================================
//                   【关卡 3：非递归遍历核心手撕区（严禁递归）】
// ============================================================================

// 1. 先序遍历（非递归实现）
// 目标：利用辅助栈，实现 根 -> 左 -> 右 顺序，访问字符追加到 out，累加 (*out_len)
void PreOrderNonRec(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    SqStack s;
    InitStack(&s);
    BiTree p = T;
    while(p != NULL || !StackEmpty(&s)) {
      if(p != NULL) {
        out[*out_len] = p->data;
        (*out_len)++;
        Push(&s, p);
        p = p->lchild;
      }else {
        Pop(&s, &p);
        p = p->rchild;
      }
    }
    // wrong
    // if(!T || !out || !out_len) return ;
    // SqStack s;
    // InitStack(&s);
    // Push(&s, T);
    // while(!StackEmpty(&s)) {
    //   BiTree node;
    //   GetTop(&s, &node);
    //   while(node) {
    //     out[*out_len] = node->data;
    //     (*out_len)++;
    //     if(node->lchild) Push(&s, node->lchild);
    //     node = node->lchild;
    //   }
    //   BiTree top;
    //   Pop(&s, &top);
    //   if(top->rchild) {
    //     Push(&s, top->rchild);
    //   }
    // }
}

// 2. 中序遍历（非递归实现）
// 目标：利用辅助栈，实现 左 -> 根 -> 右 顺序，访问字符追加到 out，累加 (*out_len)
void InOrderNonRec(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：


    if(!T || !out || !out_len) return ;
    SqStack s;
    InitStack(&s);
    BiTree p = T;
    while(p != NULL || !StackEmpty(&s)) {
      if(p != NULL) {
        Push(&s, p);
        p = p->lchild;
      }else {
        Pop(&s, &p);
        out[*out_len] = p->data;
        (*out_len)++;
        p = p->rchild;
      }
    }
    // if(!T || !out || !out_len) return ;
    // SqStack s;
    // InitStack(&s);
    // Push(&s, T);
    // while(!StackEmpty(&s)) {
    //   BiTree node;
    //   GetTop(&s, &node);
    //   while(node) {
    //     Push(&s, node->lchild);
    //     node = node->lchild;
    //   }
    //   out[*out_len] = node->data;
    //   (*out_len)++;
    //   BiTree top;
    //   Pop(&s, &top);
    //   if(top->rchild) {
    //     Push(&s, top->rchild);
    //   }
    // }
}

// 3. 后序遍历（非递归实现）
// 目标：利用辅助栈，实现 左 -> 右 -> 根 顺序，访问字符追加到 out，累加 (*out_len)
// 提示：可选用“双栈法”或“最近访问指针 pre 标记法”
void PostOrderNonRec(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    SqStack s;
    InitStack(&s);
    BiTree p = T;
    // p != NULL 说明还需要继续遍历左子树，否则就是说明左边没了要回溯到上一个结点
    BiTree q = NULL;
    while(p != NULL || !StackEmpty(&s)) {
      if(p != NULL) {
        Push(&s, p);
        p = p->lchild;
      }else {
        GetTop(&s, &p);
        if(p->rchild != NULL && p->rchild != q) {
          // 有右孩子并且未被访问过
          p = p->rchild;
        }else {
          // 否则就是没有右孩子或者右孩子已经被访问了，这时候就可以访问该结点了
          Pop(&s, &p);
          out[*out_len] = p->data;
          (*out_len)++;
          q = p;
          // 置空防止死循环
          p = NULL;
        }
      }
    }    
    // if(!T || !out || !out_len) return ;
    // SqStack s;
    // InitStack(&s);
    // Push(&s, T);
    // while(!StackEmpty(&s)) {
    //   BiTree node;
    //   GetTop(&s, &node);
    //   while(node) {
    //     Push(&s, node->lchild);
    //     node = node->lchild;
    //   }
    //   BiTree top;
    //   Pop(&s, &top);
    //   if(top->rchild) {
    //     Push(&s, top->rchild);
    //   }else {
    //     out[*out_len] = top->data;
    //     (*out_len)++;
    //   }
    // }

}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 二叉树【关卡3：非递归遍历】自测开始 =============\n\n");

    // 测试树结构形态：
    //             A
    //           /   \             //
    //          B     C
    //         / \     \           //
    //        D   E     F
    // 先序扩展序列: "ABD##E##C#F##"
    const char *tree_str = "ABD##E##C#F##";
    int build_idx = 0;
    BiTree root = CreateBiTree(tree_str, &build_idx);

    printf("测试 1 [二叉树构建]: %s\n", (root != NULL) ? "PASS" : "FAIL");

    // 1. 测试非递归先序遍历 (期望: ABDECF)
    char pre_out[50] = {0};
    int pre_len = 0;
    PreOrderNonRec(root, pre_out, &pre_len);
    pre_out[pre_len] = '\0';
    bool test2_ok = (strcmp(pre_out, "ABDECF") == 0);
    printf("测试 2 [非递归先序遍历]: %s (输出: %s / 期望: ABDECF)\n",
           test2_ok ? "PASS" : "FAIL", pre_out);

    // 2. 测试非递归中序遍历 (期望: DBEACF)
    char in_out[50] = {0};
    int in_len = 0;
    InOrderNonRec(root, in_out, &in_len);
    in_out[in_len] = '\0';
    bool test3_ok = (strcmp(in_out, "DBEACF") == 0);
    printf("测试 3 [非递归中序遍历]: %s (输出: %s / 期望: DBEACF)\n",
           test3_ok ? "PASS" : "FAIL", in_out);

    // 3. 测试非递归后序遍历 (期望: DEBFCA)
    char post_out[50] = {0};
    int post_len = 0;
    PostOrderNonRec(root, post_out, &post_len);
    post_out[post_len] = '\0';
    bool test4_ok = (strcmp(post_out, "DEBFCA") == 0);
    printf("测试 4 [非递归后序遍历]: %s (输出: %s / 期望: DEBFCA)\n",
           test4_ok ? "PASS" : "FAIL", post_out);

    DestroyTree(&root);
    printf("测试 5 [整树销毁与置空]: %s\n", (root == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}