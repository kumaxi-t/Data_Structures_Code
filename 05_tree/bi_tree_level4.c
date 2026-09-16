#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// 要求：能看懂它的原理，知道 FirstNode 和 NextNode 是怎么找前驱后继的。

// 不需要在考场上徒手默写出这五六十行线索化代码。

// 408 统编规范：中序线索二叉树结点定义
// tag == 0 表示指向亲生孩子；tag == 1 表示指向前驱/后继线索
typedef struct ThreadNode {
    char data;
    struct ThreadNode *lchild;
    struct ThreadNode *rchild;
    int ltag, rtag;
} ThreadNode, *ThreadTree;

// 辅助普通建树函数（构建出的树所有 tag 初始默认为 0）
ThreadTree CreateThreadTree(const char *str, int *index) {
    if (!str || !index || str[*index] == '\0' || str[*index] == '#') {
        if (str && index && str[*index] == '#') (*index)++;
        return NULL;
    }
    ThreadTree node = (ThreadTree)malloc(sizeof(ThreadNode));
    if (!node) return NULL;
    node->data = str[*index];
    node->ltag = node->rtag = 0; // 初始全为普通孩子指针
    (*index)++;
    node->lchild = CreateThreadTree(str, index);
    node->rchild = CreateThreadTree(str, index);
    return node;
}

// 释放线索二叉树（需要借助线索遍历销毁，或在自测前完成）
void DestroyThreadTree(ThreadTree *T) {
    if (!T || !(*T)) return;
    // 销毁时需严防沿线索死循环，若 ltag == 0 才递归释放左，rtag == 0 才递归释放右
    if ((*T)->ltag == 0) DestroyThreadTree(&((*T)->lchild));
    if ((*T)->rtag == 0) DestroyThreadTree(&((*T)->rchild));
    free(*T);
    *T = NULL;
}

// ============================================================================
//                          【关卡 4：线索化核心手撕区】
// ============================================================================

// 1. 中序线索化核心递归工作函数
// 参数：p 为当前遍历结点；pre 为二级指针，记录刚访问过的直接前驱结点指针
void InThread(ThreadTree p, ThreadTree *pre) {
    // 请在此编写你的代码：
    // 如果他左边有孩子就先把左边处理了
    if(!p) return ;
    if(p->ltag == 0) InThread(p->lchild, pre);
    // 如果左孩子为空
    if(p->lchild == NULL) {
      p->lchild = *pre;
      p->ltag = 1;
    }
    // 更新前驱结点的后继结点
    if(*pre && (*pre)->rchild == NULL) {
      (*pre)->rchild = p;
      (*pre)->rtag = 1;
    }
    *pre = p;
    if(p->rtag == 0) {
      InThread(p->rchild, pre);
    }
}

// 2. 中序线索化包装主函数
// 目标：初始化 pre 为 NULL，调用 InThread，并处理整棵树最后一个结点的尾部线索收尾
void CreateInThreadTree(ThreadTree T) {
    // 请在此编写你的代码：
    ThreadTree pre = NULL;
    if(T != NULL){  
      InThread(T, &pre);
      pre->rchild = NULL;
      pre->rtag = 1;
    }
}

// 3. 辅助函数：找以 p 为根的子树里，在中序遍历下的“第一个被访问的结点”（即最左下角的结点）
ThreadNode* FirstNode(ThreadNode *p) {
    // 请在此编写你的代码：
    if(!p) return NULL;
    while(p->ltag == 0) {
      p = p->lchild;
    }
    return p;
}

// 4. 辅助函数：找结点 p 在中序下的“后继结点”
// 规则：若 rtag == 1，直接返回 rchild；若 rtag == 0，返回其右子树中最左下角的结点
ThreadNode* NextNode(ThreadNode *p) {
    // 请在此编写你的代码：
    if(!p) return NULL;
    if(p->rtag == 1) {
      return p->rchild;
    }else {
      // 找p的左子树最靠近左边的那个结点
      return FirstNode(p->rchild);
      // ThreadTree r = p->rchild;
      // while(r->ltag == 0) {
      //   r = r->lchild;
      // }
      // return r;
    }
    return NULL;
}

// 5. 基于线索的非递归中序遍历（严禁使用系统递归，严禁开辟辅助栈！）
// 目标：纯靠 FirstNode 和 NextNode，将中序遍历结果存入 out 数组，更新 out_len
void InOrderThreadTraversal(ThreadTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return ;
    ThreadTree node = FirstNode(T);
    while(node) {
      out[*out_len] = node->data;
      (*out_len)++;
      node = NextNode(node);
    }
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 二叉树【关卡4：中序线索化与无栈遍历】自测开始 =============\n\n");

    // 测试树结构形态：
    //             A
    //           /   \        //
    //          B     C
    //         / \     \      //
    //        D   E     F
    // 先序扩展序列: "ABD##E##C#F##"
    // 理论中序遍历序列: D B E A C F
    const char *tree_str = "ABD##E##C#F##";
    int build_idx = 0;
    ThreadTree root = CreateThreadTree(tree_str, &build_idx);

    printf("测试 1 [二叉树构建]: %s\n", (root != NULL) ? "PASS" : "FAIL");

    // 执行中序线索化
    CreateInThreadTree(root);

    // 校验线索指针构建是否正确
    // 叶子 D：前驱应为 NULL(ltag=1)，后继应为 B(rtag=1)
    ThreadNode *nodeD = root->lchild->lchild;
    bool d_ok = (nodeD->ltag == 1 && nodeD->lchild == NULL) &&
                (nodeD->rtag == 1 && nodeD->rchild == root->lchild);
    printf("测试 2 [最左叶子 D 的前驱/后继线索验证]: %s\n", d_ok ? "PASS" : "FAIL");

    // 校验非递归无栈线索遍历 (期望序列: DBEACF)
    char thread_out[50] = {0};
    int thread_len = 0;
    InOrderThreadTraversal(root, thread_out, &thread_len);
    thread_out[thread_len] = '\0';
    bool test3_ok = (strcmp(thread_out, "DBEACF") == 0);
    printf("测试 3 [零栈/零递归中序线索遍历]: %s (输出: %s / 期望: DBEACF)\n",
           test3_ok ? "PASS" : "FAIL", thread_out);

    DestroyThreadTree(&root);
    printf("测试 4 [整树线索安全释放]: %s\n", (root == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}