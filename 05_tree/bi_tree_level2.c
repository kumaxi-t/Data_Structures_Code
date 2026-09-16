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

// 辅助循环队列定义（用于层序遍历）
#define QueueSize 50

typedef struct {
    BiTree data[QueueSize];
    int front;
    int rear;
} SqQueue;

void InitQueue(SqQueue *Q) {
    Q->front = Q->rear = 0;
}

bool QueueEmpty(const SqQueue *Q) {
    return Q->front == Q->rear;
}

bool EnQueue(SqQueue *Q, BiTree node) {
    if ((Q->rear + 1) % QueueSize == Q->front) return false;
    Q->data[Q->rear] = node;
    Q->rear = (Q->rear + 1) % QueueSize;
    return true;
}

bool DeQueue(SqQueue *Q, BiTree *node) {
    if (QueueEmpty(Q)) return false;
    *node = Q->data[Q->front];
    Q->front = (Q->front + 1) % QueueSize;
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

// ============================================================================
//                          【关卡 2：队列与层序核心手撕区】
// ============================================================================

// 1. 标准层序遍历
// 目标：借助队列 Q，按自上而下、从左往右的顺序遍历二叉树 T，依次存入 out 数组，更新 out_len
void LevelOrder(BiTree T, char *out, int *out_len) {
    // 请在此编写你的代码：
    if(!T || !out || !out_len) return;
    SqQueue q;
    InitQueue(&q);
    EnQueue(&q, T);
    while(!QueueEmpty(&q)) {
      BiTree node;
      if(!DeQueue(&q, &node)) continue;
      out[*out_len] = node->data;
      (*out_len)++;
      if(node->lchild) EnQueue(&q, node->lchild);
      if(node->rchild) EnQueue(&q, node->rchild);
    }
}

// 2. 求二叉树的最大宽度（某一层所拥有的最多结点数）
// 目标：空树返回 0；利用队列按层处理，每轮外层循环前读取当前层的总节点数（即队列当前元素个数）
// 提示：当前层有效节点数 count = (Q.rear - Q.front + QueueSize) % QueueSize
int MaxWidth(BiTree T) {
    // 请在此编写你的代码：
    if(!T) return 0;
    int max_width = 1;
    SqQueue q;
    InitQueue(&q);
    EnQueue(&q, T);
    while(!QueueEmpty(&q)) {
        int size = (q.rear - q.front + QueueSize) % QueueSize;
        if(size > max_width) max_width = size;
        while(size--) {
            BiTree node;
            if(!DeQueue(&q, &node)) continue;
            // 判断是否为有效结点，不然不能放入队列占据位置
            if(node->lchild) EnQueue(&q, node->lchild);
            if(node->rchild) EnQueue(&q, node->rchild);
        }
    }
    return max_width;
}

// 3. 判断二叉树是否为完全二叉树（408 经典命题大题）
// 算法思想：
// 采用层序遍历，所有结点（包括 NULL）全部允许入队；
// 当从队列首次取出 NULL 结点后，如果后续出队时还能遇到非 NULL 结点，说明出现“空缺断层”，绝不是完全二叉树！
// 返回：是完全二叉树返回 true，否则返回 false
bool IsCompleteTree(BiTree T) {
    // 请在此编写你的代码：

    // 空树是完全二叉树
    if(!T) return true; 
    // if(!T) return false;
    SqQueue q;
    InitQueue(&q);
    EnQueue(&q, T);
    int find = 0;
    // 正确思路：如果NULL后面是实体元素就不是完全二叉树
    while(!QueueEmpty(&q)) {
        // int size = (q.rear - q.front + QueueSize) % QueueSize;
        // 只能在最后面没找到， 如果非最后一层找到了就不是
        // int find = 0;

        // 没必要双层，直接顺序判断查找有效元素前是否有空结点即可
        // while(size--) {
        BiTree node;
        if(!DeQueue(&q, &node)) continue;
        if(!node) {
            // 发现了空结点
            find = 1;
            continue;
        }else {
            // 如果有效元素前面有空节点就不是完全二叉树
            if(find) return false;
            EnQueue(&q, node->lchild);
            EnQueue(&q, node->rchild);
        }
    }
    return true;
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
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 二叉树【关卡2：层序与队列扩展】自测开始 =============\n\n");

    // 树 1（标准完全二叉树）：
    //             A
    //           /   \   // 
    //          B     C
    //         / \   /
    //        D   E F
    // 先序扩展序列: "ABD##E##CF###"
    const char *tree1_str = "ABD##E##CF###";
    int idx1 = 0;
    BiTree tree1 = CreateBiTree(tree1_str, &idx1);

    // 1. 测试标准层序遍历 (期望: ABCDEF)
    char level_out[50] = {0};
    int level_len = 0;
    LevelOrder(tree1, level_out, &level_len);
    level_out[level_len] = '\0';
    bool test1_ok = (strcmp(level_out, "ABCDEF") == 0);
    printf("测试 1 [标准层序遍历]: %s (输出: %s / 期望: ABCDEF)\n",
           test1_ok ? "PASS" : "FAIL", level_out);

    // 2. 测试最大宽度
    // 第1层: A (1)
    // 第2层: B, C (2)
    // 第3层: D, E, F (3) -> 最大宽度期望为 3
    int width1 = MaxWidth(tree1);
    printf("测试 2 [求最大宽度]: %s (宽度: %d / 期望: 3)\n",
           (width1 == 3) ? "PASS" : "FAIL", width1);

    // 3. 测试完全二叉树判定（树 1 应为 true）
    bool is_complete1 = IsCompleteTree(tree1);
    printf("测试 3 [完全二叉树正例判定]: %s (判定结果: %s / 期望: 是)\n",
           is_complete1 ? "PASS" : "FAIL", is_complete1 ? "是" : "否");

    // 树 2（非完全二叉树，左缺右突）：
    //             A
    //           /   \            //
    //          B     C
    //           \     \          //
    //            E     F
    // 先序扩展序列: "AB#E##C#F##"
    const char *tree2_str = "AB#E##C#F##";
    int idx2 = 0;
    BiTree tree2 = CreateBiTree(tree2_str, &idx2);

    // 4. 测试完全二叉树反例判定（树 2 应为 false）
    bool is_complete2 = IsCompleteTree(tree2);
    printf("测试 4 [完全二叉树反例判定]: %s (判定结果: %s / 期望: 否)\n",
           (!is_complete2) ? "PASS" : "FAIL", is_complete2 ? "是" : "否");

    DestroyTree(&tree1);
    DestroyTree(&tree2);
    printf("测试 5 [双树销毁与置空]: PASS\n");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}