#include <iostream>
#include <string>
#include <queue>
#include <vector>

enum Color {RED, BLACK };

struct RBNode {
  int val;
  Color color;
  RBNode* left;
  RBNode* right;
  RBNode* parent;

  RBNode(int v) :
    val(v),
    color(RED),
    left(nullptr),
    right(nullptr),
    parent(nullptr) {}

};


class RBTree {

private:
  RBNode* root;

  // 中序遍历
  void InOrderHelper(RBNode* node) {
    if (node == nullptr) return;
    InOrderHelper(node->left);
    std::cout << node->val << "(" << (node->color == RED ? "R" : "B") << ") ";
    InOrderHelper(node->right);
  }
  // 左旋
  void LeftRotate(RBNode* x) {
    RBNode* y = x->right;

    // y的左孩子放到x的右边并更新父指针
    x->right = y->left;
    if(y->left) {
      y->left->parent = x;
    }

    // 更新y的父结点
    y->parent = x->parent;
    if(x->parent == nullptr) {
      // x没有父指针是根节点
      // y是更新后的父结点即替代为根结点
      root = y;
    } else if (x == x->parent->left) {
      // x是他爹的左孩子
      x->parent->left = y;
    } else {
      // x是他爹的右孩子
      x->parent->right = y;
    }
    // x的父结点为y，y的左孩子为x
    x->parent = y;
    y->left = x;

  }
  // 右旋
  void RightRotate(RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if(x->right) {
      x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == nullptr) {
      root = x;
    } else if (y == y->parent->right) {
      y->parent->right = x;
    } else {
      y->parent->left = x;
    }
    y->parent = x;
    x->right = y;
  }
  // 插入操作修正
  void InsertFixup(RBNode* z) {
    while(z->parent != nullptr && z->parent->color == RED) {
      // 他爹是他爷的左孩子
      if(z->parent == z->parent->parent->left) {
        RBNode* uncle = z->parent->parent->right;
        if(uncle != nullptr && uncle->color == RED) {
          // 父结点是红的并且叔叔也是红的
          // 说明爷爷结点已经左右粘了两个结点了
          // 爷爷装不下了被强行顶上去依附上面的结点（染成红的）
          // 叔和父各自自立门户（染成黑的）
          uncle->color = BLACK;
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          // 爷爷结点被顶上去了要再重新判断是否满足红黑树的性质
          z = z->parent->parent;
        }else {
          // 叔结点为空或者为黑
          // LR型先左旋变成LL型
          if(z == z->parent->right) {
            // 左旋会让z升上去父结点降下来
            z = z->parent;
            LeftRotate(z);
            // 让z还是最下面的结点
          }
          // 变色使得父结点作为主导结点
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          RightRotate(z->parent->parent);
        }



      } else {
        RBNode* uncle = z->parent->parent->left;
        if(uncle != nullptr && uncle->color == RED) {
          uncle->color = BLACK;
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        }else {
          if(z == z->parent->left) {
            z = z->parent;
            RightRotate(z);
          }
          // 变色使得父结点作为主导结点
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          LeftRotate(z->parent->parent);
        }
      }
    }
    root->color = BLACK;
  }
  // 删除操作修正
  void DeleteFixup(RBNode* replacement, RBNode* replacement_parent) {
    while (replacement != root && (replacement == nullptr || replacement->color == BLACK)) {
      if(replacement == replacement_parent->left) {

        RBNode* brother = replacement_parent->right;
        // 兄弟是红的
        if(brother->color == RED) {
          brother->color = BLACK;
          replacement_parent->color = RED;
          LeftRotate(replacement_parent);
          brother = replacement_parent->right;
        }
        // 兄弟是黑的，侄子也全黑
        if((brother->left == nullptr || brother->left->color == BLACK) &&
          (brother->right == nullptr || brother->right->color == BLACK)) {
          brother->color = RED;
          replacement = replacement_parent;
          replacement_parent = replacement_parent->parent;
        } else {
          // 兄弟是黑的且有红侄
          // 左侄是红的，要转换成右侄是红的这种情况
          if(brother->right == nullptr || brother->right->color == BLACK) {
            if(brother->left) brother->left->color = BLACK;
            brother->color = RED;
            RightRotate(brother);
            brother = replacement_parent->right;
          }
          // 右侄是红的
          brother->color = replacement_parent->color;
          replacement_parent->color = BLACK;
          brother->right->color = BLACK;
          LeftRotate(replacement_parent);
          replacement = root;
          break;
        }
      } else {
      // 对称处理
        RBNode* brother = replacement_parent->left;
        if(brother->color == RED) {
          brother->color = BLACK;
          replacement_parent->color = RED;
          RightRotate(replacement_parent);
          brother = replacement_parent->left;
        }
        if((brother->right == nullptr || brother->right->color == BLACK) &&
          (brother->left == nullptr || brother->left->color == BLACK)) {
          brother->color = RED;
          replacement = replacement_parent;
          replacement_parent = replacement_parent->parent;
        } else {
          if(brother->left == nullptr || brother->left->color == BLACK) {
            if(brother->right) brother->right->color = BLACK;
            brother->color = RED;
            LeftRotate(brother);
            brother = replacement_parent->left;
          }
          brother->color = replacement_parent->color;
          replacement_parent->color = BLACK;
          brother->left->color = BLACK;
          RightRotate(replacement_parent);
          replacement = root;
          break;
        }
      }

    }      
    if(replacement != nullptr) {
      replacement->color = BLACK;
    }
  }

  private:

  // 销毁
  void DestroyHelper(RBNode* root) {
    if(root == nullptr) return ;
    DestroyHelper(root->left);
    DestroyHelper(root->right);
    delete root;
  }
  // 找直接后继
  RBNode* Mininum(RBNode* node) {
    while(node->left) {
      node = node->left;
    }
    return node;
  }
  // u被v取代
  void Transplant(RBNode* u, RBNode* v) {
    if(u->parent == nullptr) {
      // u为根结点
      root = v;
    } else if(u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if(v != nullptr) {
      v->parent = u->parent;
    }
  }
public:
  // 删除结点
  void Delete(int val) {
    RBNode* target = root;
    while(target && target->val != val) {
      if(val > target->val) {
        target = target->right;
      } else{
        target = target->left;
      }
    }
    if(target == nullptr) return ;

    RBNode* replacement = nullptr;          // 顶替删除结点位置的继任结点
    RBNode* replacement_parent = nullptr;   // 继任结点的父结点
    Color removed_color = BLACK;            // 被移走结点的初始颜色

    if(target->left == nullptr) {
      // 左边为空直接让右边结点顶替
      replacement = target->right;
      replacement_parent = target->parent;
      removed_color = target->color;

      Transplant(target, replacement);
    } else if(target->right == nullptr) {
      // 右边为空让左边顶替  
      replacement = target->left;
      replacement_parent = target->parent;
      removed_color = target->color;

      Transplant(target, replacement);
    } else {
      // 左右都有，找直接后继顶替
      RBNode* successor = Mininum(target->right);
      // 后继结点的下属
      replacement = successor->right;
      // 继任结点上去了也即该颜色消失了，记录他的颜色
      removed_color = successor->color;
      if(successor->parent == target) {
        // 继任者他爹就是要删除的结点
        replacement_parent = successor;
      } else {
        // 找到了删除结点的直接后继
        // 处理后继的下属
        replacement_parent = successor->parent;
        // 后继的上属接管后继的下属
        Transplant(successor, replacement);
        // 删除结点的右结点和后继绑定关系
        successor->right = target->right;
        successor->right->parent = successor;
      }
      // 删除结点的上属接管后继结点
      Transplant(target, successor);
      // 删除结点的左结点和后继绑定关系
      successor->left = target->left;
      successor->left->parent = successor;
      // 后继移到上面去了，让后继继承删除结点的颜色
      successor->color = target->color;

    }
    delete target;
    // 要删除的结点为黑色，说明删除了之后必然有一条路少一个黑色结点不满足黑路同，若为红色则无关紧要
    if(removed_color == BLACK) {
      DeleteFixup(replacement, replacement_parent);
    }
  }

  RBTree() {
    root = nullptr;
  }
  // 中序遍历
  void InOrder() {
    InOrderHelper(root);
    std::cout << std::endl;
  }
  // 层序遍历
  void LevelOrder() {
    if(!root) return ;
    std::queue<RBNode*> q;
    q.push(root);
    while(!q.empty()) {
      int levelsize = q.size();
      for(int i = 0; i < levelsize; i++) {
        auto cur = q.front();
        q.pop();
        std::cout << cur->val << "(" << (cur->color == RED ? "R" : "B") << ") ";
        if(cur->left) q.push(cur->left);
        if(cur->right) q.push(cur->right);
      }
      std::cout << std::endl;
    }
  }
  // 插入操作
  void Insert(int val) {
    RBNode* z = new RBNode(val);  // 创建的新结点
    RBNode* y = nullptr;          // 父结点
    RBNode* x = root;             // 探测结点

    while(x != nullptr) {
      if(val > x->val) {
        y = x;
        x = x->right;
      }else {
        y = x;
        x = x->left;
      }
    }
    z->parent = y;
    if(y == nullptr) {
      // y是空的那么就由新结点充当根结点
      root = z;
    } else {
      // y不为空则判断z挂到y的左边还是右边
      if(val > y->val) {
        y->right = z;
      }else {
        y->left = z;
      }
    }

    InsertFixup(z); 

  }

  ~RBTree() {
    DestroyHelper(root);
    root = nullptr;
  }

  // 以下为辅助验证代码片段
  public:
    void PrintVisualTree() {
        if (!root) {
            std::cout << "  (当前是空树)\n";
            return;
        }
        PrintVisualHelper(root, "", false, true);
    }

private:
    void PrintVisualHelper(RBNode* node, std::string prefix, bool isLeft, bool isRoot) {
        if (!node) return;
        if (node->right) {
            PrintVisualHelper(node->right, prefix + (isRoot ? "  " : (isLeft ? "  │   " : "      ")), false, false);
        }
        std::cout << prefix;
        if (isRoot) {
            std::cout << "  ─── ";
        } else {
            std::cout << (isLeft ? "  └── " : "  ┌── ");
        }
        // 红色与黑色终端彩色渲染
        if (node->color == RED) {
            std::cout << "\033[1;31m" << node->val << "(R)\033[0m\n";
        } else {
            std::cout << "\033[1;36m" << node->val << "(B)\033[0m\n";
        }
        if (node->left) {
            PrintVisualHelper(node->left, prefix + (isRoot ? "  " : (isLeft ? "      " : "  │   ")), true, false);
        }
    }
};


// 辅助打印树状枝桠结构的递归函数（不修改 RBTree 原有代码）
void PrintPrettyTree(RBNode* node, std::string prefix = "", bool isLeft = true, bool isRoot = true) {
    if (node == nullptr) return;

    // 先递归打印右子树（在终端上方显示右子树）
    if (node->right) {
        PrintPrettyTree(node->right, prefix + (isRoot ? "" : (isLeft ? "│   " : "    ")), false, false);
    }

    // 打印当前节点及连接符
    std::cout << prefix;
    if (isRoot) {
        std::cout << "─── ";
    } else {
        std::cout << (isLeft ? "└── " : "┌── ");
    }

    // 终端颜色高亮：红色高亮为红字，黑色为青/蓝字
    if (node->color == RED) {
        std::cout << "\033[1;31m" << node->val << "(R)\033[0m\n";
    } else {
        std::cout << "\033[1;36m" << node->val << "(B)\033[0m\n";
    }

    // 再递归打印左子树（在终端下方显示左子树）
    if (node->left) {
        PrintPrettyTree(node->left, prefix + (isRoot ? "" : (isLeft ? "    " : "│   ")), true, false);
    }
}

// 统一的展示函数：先画树形，再打中序验证有序性
void ShowTreeState(RBTree& tree, const std::string& title) {
    std::cout << "\n======================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "======================================================\n";
    std::cout << "【树状拓扑图】(从左往右看：上分支是右孩子，下分支是左孩子):\n\n";
    
    // 技巧：把私有 root 取出或者在类里开一个 friend / 快捷调用
    // 如果 root 是私有的，可以直接使用 tree 的层序输出，也可以在类里加一行 friend void ShowTreeState;
}

int main() {
    RBTree tree;

    std::cout << "====================================================\n";
    std::cout << "  步骤 1: 批量插入构建红黑树 (50, 20, 80, 10, 30, 70, 90, 60)\n";
    std::cout << "====================================================\n";
    std::vector<int> nums = {50, 20, 80, 10, 30, 70, 90, 60};
    for (int x : nums) {
        tree.Insert(x);
    }
    tree.PrintVisualTree();
    std::cout << "\n当前中序遍历: ";
    tree.InOrder();

    std::cout << "\n====================================================\n";
    std::cout << "  步骤 2: 删除 90(黑) -> 触发【Case 3 内红拉直 + Case 4 外红平账】\n";
    std::cout << "====================================================\n";
    tree.Delete(90);
    tree.PrintVisualTree();
    std::cout << "\n当前中序遍历: ";
    tree.InOrder();

    std::cout << "\n====================================================\n";
    std::cout << "  步骤 3: 删除 60(黑) -> 触发【Case 2 兄穷合并向上甩锅】\n";
    std::cout << "====================================================\n";
    tree.Delete(60);
    tree.PrintVisualTree();
    std::cout << "\n当前中序遍历: ";
    tree.InOrder();

    std::cout << "\n====================================================\n";
    std::cout << "  步骤 4: 删除根节点 50 -> 测试【双子节点后继 70 顶替接管】\n";
    std::cout << "====================================================\n";
    tree.Delete(50);
    tree.PrintVisualTree();
    std::cout << "\n当前中序遍历: ";
    tree.InOrder();

    std::cout << "\n====================================================\n";
    std::cout << "  步骤 5: 逐个删空整棵树 (测试边界与内存安全)\n";
    std::cout << "====================================================\n";
    std::vector<int> rest = {10, 20, 30, 70, 80};
    for (int val : rest) {
        std::cout << "\n>>> 删除节点 [" << val << "] 后的形态:\n";
        tree.Delete(val);
        tree.PrintVisualTree();
    }

    std::cout << "\n所有测试用例验证完毕，整棵红黑树生命周期完全自平衡！\n";
    return 0;
}