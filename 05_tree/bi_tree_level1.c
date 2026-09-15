#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 408 统编规范：二叉链表结点定义
typedef struct BiTNode {
    char data;                  // 数据域
    struct BiTNode *lchild;     // 左孩子指针
    struct BiTNode *rchild;     // 右孩子指针
} BiTNode, *BiTree;

// ============================================================================
//                          【关卡 1：核心递归手撕区】
// ============================================================================

// 1. 基于先序扩展字符串递归建树
// 规则：str[*index] 为当前字符。若为 '#'，表示当前子树为空（返回 NULL）；
//       若为字母，malloc 申请结点填入数据，然后递归构造左子树，再递归构造右子树。
// 注意：每次读取一个字符后，记得让 (*index)++
BiTree CreateBiTree(const char *str, int *index) {
    // 请在此编写你的代码：
    // if(!str) return NULL;
    if (!str || !index || str[*index] == '\0') return NULL;
    if(str[*index] == '#') {
        (*index)++;
        return NULL;
    }
    BiTree node = (BiTree)malloc(sizeof(BiTNode));
    if(!node) return NULL;
    node->data = str[*index];
    (*index)++;
    node->lchild = CreateBiTree(str, index);
    node->rchild = CreateBiTree(str, index);
    return node;
}

// 2. 先序遍历（递归）
// 目标：根 -> 左 -> 右。访问到字符时存入 out 数组，并累加 (*out_len)
void PreOrder(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    out[*out_len] = T->data;
    (*out_len)++;
    PreOrder(T->lchild, out, out_len);
    PreOrder(T->rchild, out, out_len);
}

// 3. 中序遍历（递归）
// 目标：左 -> 根 -> 右。访问到字符时存入 out 数组，并累加 (*out_len)
void InOrder(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    InOrder(T->lchild, out, out_len);
    out[*out_len] = T->data;
    (*out_len)++;
    InOrder(T->rchild, out, out_len);
}

// 4. 后序遍历（递归）
// 目标：左 -> 右 -> 根。访问到字符时存入 out 数组，并累加 (*out_len)
void PostOrder(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    PostOrder(T->lchild, out, out_len);
    PostOrder(T->rchild, out, out_len);
    out[*out_len] = T->data;
    (*out_len)++;
}

// 5. 统计二叉树的总结点数（经典递归分治）
// 目标：若为空树返回 0；否则返回 1 + 左子树结点数 + 右子树结点数
int CountNodes(BiTree T) {
    // 请在此编写你的代码：
    if(!T) return 0;
    return (CountNodes(T->lchild) + CountNodes(T->rchild) + 1);
}

// 6. 求二叉树的高度/深度（经典递归分治）
// 目标：若为空树返回 0；否则返回 1 + max(左子树高, 右子树高)
int TreeDepth(BiTree T) {
    // 请在此编写你的代码：
    if(!T) return 0;
    int left_max = TreeDepth(T->lchild);
    int right_max = TreeDepth(T->rchild);
    if(left_max >= right_max) {
        return left_max + 1;
    }else {
        return right_max + 1;
    }
}

// 7. 递归销毁整棵树（后序遍历释放）
// 目标：先销毁左子树，再销毁右子树，最后释放当前根结点并置 NULL
void DestroyTree(BiTree *T) {
    // 请在此编写你的代码：
    if(!T || !(*T)) return ;
    // BiTNode* left = (*T)->lchild;
    // BiTNode* right = (*T)->rchild;
    // 大错特错，传的是局部变量的地址
    DestroyTree(&((*T)->lchild));
    DestroyTree(&((*T)->rchild));
    free(*T);
    *T = NULL;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 二叉树【关卡1：递归基石】自测开始 =============\n\n");

    // 测试树结构形态：
    //             A
    //           /   \               //
    //          B     C
    //         / \     \             //
    //        D   E     F
    // 先序扩展序列: "ABD##E##C#F##"

    const char *tree_str = "ABD##E##C#F##";
    int build_idx = 0;
    BiTree root = CreateBiTree(tree_str, &build_idx);

    printf("测试 1 [二叉树构建]: %s\n", (root != NULL) ? "PASS" : "FAIL");

    // 测试 2: 结点统计与树深度
    int total_nodes = CountNodes(root);
    int depth = TreeDepth(root);
    bool test2_ok = (total_nodes == 6) && (depth == 3);
    printf("测试 2 [总结点数与树深]: %s (总结点数: %d/期望: 6, 树深度: %d/期望: 3)\n",
           test2_ok ? "PASS" : "FAIL", total_nodes, depth);

    // 测试 3: 先序遍历 (根左右 -> ABDECF)
    char pre_out[50] = {0};
    int pre_len = 0;
    PreOrder(root, pre_out, &pre_len);
    pre_out[pre_len] = '\0';
    bool test3_ok = (strcmp(pre_out, "ABDECF") == 0);
    printf("测试 3 [先序递归遍历]: %s (输出: %s / 期望: ABDECF)\n",
           test3_ok ? "PASS" : "FAIL", pre_out);

    // 测试 4: 中序遍历 (左根右 -> DBEACF)
    char in_out[50] = {0};
    int in_len = 0;
    InOrder(root, in_out, &in_len);
    in_out[in_len] = '\0';
    bool test4_ok = (strcmp(in_out, "DBEACF") == 0);
    printf("测试 4 [中序递归遍历]: %s (输出: %s / 期望: DBEACF)\n",
           test4_ok ? "PASS" : "FAIL", in_out);

    // 测试 5: 后序遍历 (左右根 -> DEBFCA)
    char post_out[50] = {0};
    int post_len = 0;
    PostOrder(root, post_out, &post_len);
    post_out[post_len] = '\0';
    bool test5_ok = (strcmp(post_out, "DEBFCA") == 0);
    printf("测试 5 [后序递归遍历]: %s (输出: %s / 期望: DEBFCA)\n",
           test5_ok ? "PASS" : "FAIL", post_out);

    // 测试 6: 树销毁与置空
    DestroyTree(&root);
    printf("测试 6 [整树递归销毁]: %s\n", (root == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}