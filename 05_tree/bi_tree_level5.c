#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 408 统编纯 C 规范：二叉排序树结点定义
typedef struct BSTNode {
    int key;
    struct BSTNode *lchild;
    struct BSTNode *rchild;
} BSTNode, *BSTree;

// 辅助整树安全销毁（后序释放）
void DestroyBST(BSTree *T) {
    if (!T || !(*T)) return;
    DestroyBST(&((*T)->lchild));
    DestroyBST(&((*T)->rchild));
    free(*T);
    *T = NULL;
}

// ============================================================================
//                          【关卡 5：BST 核心考场手撕区】
// ============================================================================

// 1. BST 查找（建议非递归迭代实现，空间复杂度 O(1)）
// 目标：在以 T 为根的树中查找 key，若找到返回结点指针，找不到返回 NULL
BSTNode* BST_Search(BSTree T, int key) {
    // 请在此编写你的代码：
    if(!T) return NULL;
    BSTree cur = T;
    while(cur) {
      if(cur->key > key) {
        cur = cur->lchild;
      }else if(cur->key < key) {
        cur = cur->rchild;
      }else {
        return cur;
      }
    }
    return NULL;
}

// 2. BST 插入操作（采用二级指针 BSTree *T 方便直接修改父结点的指针域）
// 目标：若树中不存在 key，则作为叶子结点插入并返回 true；若已存在则返回 false
bool BST_Insert(BSTree *T, int key) {
    // 请在此编写你的代码：
    if(!T) return false;
    if(*T == NULL) {
      BSTree node = (BSTree)malloc(sizeof(BSTNode));
      node->key = key;
      node->lchild = node->rchild = NULL;
      *T = node;
      return true;
    }

    BSTree cur = *T;
    BSTree pre = NULL;
    while(cur) {
      if(cur->key == key) return false;
      pre = cur;
      if(key > cur->key) {
        cur = cur->rchild;
      }else {
        cur = cur->lchild;
      }
    }
    BSTree node = (BSTree)malloc(sizeof(BSTNode));
    node->key = key;
    node->lchild = node->rchild = NULL;
    if(key > pre->key) {
      pre->rchild = node;
    }else {
      pre->lchild = node;
    }
    return true;

    // if(!T) return false;
    // if(*T == NULL) {
    //   BSTree node = (BSTree)malloc(sizeof(BSTNode));
    //   node->key = key;
    //   node->lchild = NULL;
    //   node->rchild = NULL;
    //   *T = node;
    //   return true;
    // }
    // if(key > (*T)->key) {
    //   return BST_Insert(&((*T)->rchild), key);
    // }else if(key < (*T)->key) {
    //   return BST_Insert(&((*T)->lchild), key);
    // }else {
    //   return false;
    // }

}

// 3. 基于数组连续构建 BST
// 目标：循环调用 BST_Insert，将 keys 数组依次插入构建成一棵完整的 BST
void Create_BST(BSTree *T, const int *keys, int n) {
    // 请在此编写你的代码：
    if(!keys || n <= 0) return ;
    for(int i = 0; i < n; i++) {
      BST_Insert(T, keys[i]);
    }
}

// 4. 判定一棵二叉树是否为合法的 BST（408 经典真题原型）
// 目标：利用中序遍历严格单调递增的性质校验，pre 记录前一个结点的数值
// 提示：初次调用时 *pre 传入一个极小值（如 long long 的最小值）
bool IsValidBST(BSTree T, long long *pre) {
    // 请在此编写你的代码：
    if(!T) return true;
    if(!IsValidBST(T->lchild, pre)) return false;
    // <= 严格递增
    if(T->key <= *pre) return false;
    *pre = T->key;
    return (IsValidBST(T->rchild, pre));
}

// 5. BST 结点删除（408 难点手撕大题）
// 目标：在以 *T 为根的树中删除关键字为 key 的结点，维持 BST 特性
// 规则：若待删结点左右双全，使用其“直接后继”（右子树的最左下角结点）顶替其值
bool BST_Delete(BSTree *T, int key) {
    // 请在此编写你的代码：
    if(!T || !(*T)) return false;
    if(key > (*T)->key) {
      return BST_Delete(&((*T)->rchild), key);
    }else if(key < (*T)->key) {
      return BST_Delete(&((*T)->lchild), key);
    }else {
      // 找到了要删的结点
      BSTree del = *T;
      // 左边是空的直接让右结点后来居上
      if((*T)->lchild == NULL) {
        *T = (*T)->rchild;
        free(del);
      // 右边是空的让左结点补上
      }else if((*T)->rchild == NULL) {
        *T = (*T)->lchild;
        free(del);
      // 左右都有结点，找右孩子最左边的结点或者左孩子最右边的结点继任，再递归删除继任者原来的位置
      }else {
        BSTree successor = del->rchild;
        while(successor->lchild) {
          successor = successor->lchild;
        }
        del->key = successor->key;
        // 此时继任结点可能还有右子树
        return BST_Delete(&((*T)->rchild), successor->key);
      }
      return true;
    }
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

// 辅助中序遍历输出序列
void InOrder(BSTree T, int *res, int *len) {
    if (!T) return;
    InOrder(T->lchild, res, len);
    res[(*len)++] = T->key;
    InOrder(T->rchild, res, len);
}

int main() {
    printf("============= 考研 408 二叉树【关卡5：二叉排序树 BST】自测开始 =============\n\n");

    BSTree root = NULL;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(keys) / sizeof(keys[0]);

    // 1. 测试连续插入构建
    Create_BST(&root, keys, n);
    printf("测试 1 [BST 连续插入构建]: %s\n", (root != NULL && root->key == 50) ? "PASS" : "FAIL");

    // 2. 测试中序严格递增合法性验证
    long long pre_val = -2147483648LL - 1; // 极小初值
    bool valid = IsValidBST(root, &pre_val);
    printf("测试 2 [中序严格递增合法性校验]: %s\n", valid ? "PASS" : "FAIL");

    // 3. 测试关键字查找
    BSTNode *node = BST_Search(root, 40);
    bool search_ok = (node != NULL && node->key == 40 && BST_Search(root, 99) == NULL);
    printf("测试 3 [BST 关键字查找]: %s\n", search_ok ? "PASS" : "FAIL");

    // 4. 测试复合删除（叶子结点与双全根结点的删除）
    BST_Delete(&root, 20); // 删叶子 20
    BST_Delete(&root, 50); // 删双全根结点 50（后继 60 应顶替上来）

    int after_del[50];
    int del_len = 0;
    InOrder(root, after_del, &del_len);

    // 期望剩余序列：30, 40, 60, 70, 80
    int expected[] = {30, 40, 60, 70, 80};
    bool del_ok = (del_len == 5);
    for (int i = 0; i < 5 && del_ok; i++) {
        if (after_del[i] != expected[i]) del_ok = false;
    }
    printf("测试 4 [BST 结点复合删除]: %s\n", del_ok ? "PASS" : "FAIL");

    // 5. 测试树销毁与指针置空
    DestroyBST(&root);
    printf("测试 5 [BST 树结构安全释放]: %s\n", (root == NULL) ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}