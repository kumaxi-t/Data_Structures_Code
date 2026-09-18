#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// AVL 树标准结点定义（显式维护高度）
struct AVLNode {
    int key;
    int height;          // 当前结点所处子树的高度（叶子结点高度通常记为 1）
    AVLNode *lchild;
    AVLNode *rchild;

    AVLNode(int val) : key(val), height(1), lchild(nullptr), rchild(nullptr) {}
};

// 辅助安全释放整树
void DestroyAVL(AVLNode* &T) {
    if (!T) return;
    DestroyAVL(T->lchild);
    DestroyAVL(T->rchild);
    delete T;
    T = nullptr;
}

// ============================================================================
//                          【关卡 6：AVL 树核心手撕区】
// ============================================================================

// 1. 获取结点高度（安全防空指针：空结点高度记为 0）
int GetHeight(AVLNode *node) {
    // 请在此编写你的代码：
    if(!node) return 0;
    return node->height;
    // int left_h = GetHeight(node->lchild);
    // int right_h = GetHeight(node->rchild);
    // return (1 + max(left_h, right_h));
}

// 2. 获取平衡因子 BF = 左子树高度 - 右子树高度
int GetBalanceFactor(AVLNode *node) {
    // 请在此编写你的代码：
    if(!node) return 0;
    return GetHeight(node->lchild) - GetHeight(node->rchild);
}

// 3. 刷新结点的高度（等于左右子树最大高度 + 1）
void UpdateHeight(AVLNode *node) {
    // 请在此编写你的代码：
    if(!node) return ;
    int left_h = GetHeight(node->lchild);
    int right_h = GetHeight(node->rchild);
    node->height = (1 + max(left_h, right_h));
}

// 4. 右单旋转（LL 型旋转）：围绕 y 顺时针旋转，将左孩子 x 提上去
// 注意：通过引用 AVLNode* &y，旋转后 y 直接变为新子树的根
void RightRotate(AVLNode* &y) {
    // 请在此编写你的代码：
    AVLNode* x = y->lchild;
    AVLNode* T2 = x->rchild;

    y->lchild = T2;
    x->rchild = y;
    // 从下而上更新树高
    UpdateHeight(y);
    UpdateHeight(x);
    // 修改引用，把x作为新树高
    y = x;
    // AVLNode* left = y->lchild;
    // y->lchild = left->rchild;
    // left->rchild = y;
}

// 5. 左单旋转（RR 型旋转）：围绕 x 逆时针旋转，将右孩子 y 提上去
// 注意：通过引用 AVLNode* &x，旋转后 x 直接变为新子树的根
void LeftRotate(AVLNode* &x) {
    // 请在此编写你的代码：
    AVLNode* y = x->rchild;
    AVLNode* T2 = y->lchild;

    x->rchild = T2;
    y->lchild = x;

    UpdateHeight(x);
    UpdateHeight(y);
    
    x = y;

    // AVLNode* right = x->rchild;
    // x->rchild = right->lchild;
    // right->lchild = x;
}

// 6. AVL 树插入与自平衡核心（递归回溯刷新树高与旋转）
// 目标：将 key 插入树中，并确保返回后整棵树依然满足 AVL 平衡性质
void AVL_Insert(AVLNode* &root, int key) {
    // 请在此编写你的代码：
    if(!root) {
        root = new AVLNode(key);
        return ;
    }
    if(key > root->key) {
        AVL_Insert(root->rchild, key);
    }else if(key < root->key) {
        AVL_Insert(root->lchild, key);
    }else {
        return ;
    }
    UpdateHeight(root);
    // 从下而上依次递归返回
    int balance = GetBalanceFactor(root);
    // 拿到第一个失衡的结点做处理
    // 左边失衡做右旋，右边失衡做左旋
    if(balance > 1 && key < root->lchild->key) {
        // LL
        RightRotate(root);
    }else if(balance < -1 && key > root->rchild->key) {
        // RR
        LeftRotate(root);
    }else if(balance > 1 && key > root->lchild->key) {
        // LR
        // 先处理子树的右失衡做左旋
        LeftRotate(root->lchild); 
        // 再处理根的左失衡做右旋
        RightRotate(root);
    }else if(balance < -1 && key < root->rchild->key) {
        // RL
        // 先处理子树的左失衡做右旋
        RightRotate(root->rchild);
        // 再处理根的右失衡做左旋
        LeftRotate(root);
    }

}

// 7. AVL 树合法性全局校验
// 目标：验证是否满足 BST（中序严格递增）且每一个结点的 |BF| <= 1
bool IsValidAVL(AVLNode *root, long long &pre_val) {
    // 请在此编写你的代码：
    if(!root) return true;
    // 先检查左子树有没有问题
    if(!IsValidAVL(root->lchild, pre_val)) return false;
    // 检查是否严格递增
    if(pre_val >= root->key) return false;
    // 检查平衡因子
    if(abs(GetBalanceFactor(root)) > 1) return false;
    // 更新pre_val用于后续检查
    pre_val = root->key;
    // 检查右子树
    return (IsValidAVL(root->rchild, pre_val));
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

void InOrder(AVLNode *root, vector<int> &res) {
    if (!root) return;
    InOrder(root->lchild, res);
    res.push_back(root->key);
    InOrder(root->rchild, res);
}

int main() {
    cout << "============= 工业级实战【关卡6：平衡二叉树 AVL】自测开始 =============\n\n";

    AVLNode *root = nullptr;

    // 测试用例 1：最极端的升序序列插入（若不平衡将退化为单链表，高度为 7）
    // 插入序列：10, 20, 30, 40, 50, 25, 5
    vector<int> keys = {10, 20, 30, 40, 50, 25, 5};
    for (int k : keys) {
        AVL_Insert(root, k);
    }

    // 1. 验证整树高度控制（7个结点的完全平衡树，高度应在 3 到 4 之间）
    int tree_height = GetHeight(root);
    bool height_ok = (tree_height <= 4);
    cout << "测试 1 [AVL 插入后树高控制]: " << (height_ok ? "PASS" : "FAIL") 
         << " (实际树高: " << tree_height << ")\n";

    // 2. 验证是否为合法 AVL 树（BST 单调性 + 任意结点 |BF| <= 1）
    long long pre = -2147483648LL - 1;
    bool avl_valid = IsValidAVL(root, pre);
    cout << "测试 2 [全局 AVL 平衡与有序校验]: " << (avl_valid ? "PASS" : "FAIL") << "\n";

    // 3. 验证中序遍历序列完整性
    vector<int> sorted_keys;
    InOrder(root, sorted_keys);
    vector<int> expected = {5, 10, 20, 25, 30, 40, 50};
    bool order_ok = (sorted_keys == expected);
    cout << "测试 3 [中序遍历结果数据完整性]: " << (order_ok ? "PASS" : "FAIL") << "\n";

    // 4. 验证根结点数值（通过模拟旋转推演，根应处于合理中间值）
    bool root_ok = (root != nullptr && (root->key == 30 || root->key == 20 || root->key == 40));
    cout << "测试 4 [核心新根定位校验]: " << (root_ok ? "PASS" : "FAIL") 
         << " (当前根: " << (root ? root->key : -1) << ")\n";

    // 5. 树安全释放
    DestroyAVL(root);
    cout << "测试 5 [AVL 树安全销毁与置空]: " << (root == nullptr ? "PASS" : "FAIL") << "\n";

    cout << "\n============= 自测流程全部结束 =============\n";
    return 0;
}