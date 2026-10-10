#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cassert>
#include <iomanip>

static const int M = 4;




struct BPlusTreeNode {
  bool is_leaf;

  BPlusTreeNode* parent;
  BPlusTreeNode* pre;
  BPlusTreeNode* next;

  std::vector<int> keys;
  std::vector<BPlusTreeNode*> children;

  BPlusTreeNode(bool leaf = true) :
    is_leaf(leaf),
    parent(nullptr),
    pre(nullptr),
    next(nullptr){}
};

// 找到对应值的叶子结点
BPlusTreeNode* FindLeaf(BPlusTreeNode* root, int val) {
  if(!root) return nullptr;
  BPlusTreeNode* cur = root;
  while(cur && !cur->is_leaf) {
    size_t i = 0;
    while(i < cur->keys.size()) {
      if(val < cur->keys[i]) break;
      i++;
    }
    cur = cur->children[i];
  }
  if(!cur) return nullptr;
  return cur;
}

// 把key和child插入到对应结点中
void InsertKeyAndChild(BPlusTreeNode* node, int key, BPlusTreeNode* child = nullptr) {
  size_t i = 0;
  while(i < node->keys.size()) {
    if(key < node->keys[i]) break;
    i++;
  }
  node->keys.insert(node->keys.begin() + i, key);
  if(child) {
    node->children.insert(node->children.begin() + i + 1, child);
    child->parent = node;
  }
}

// 非叶子结点分裂
void SplitInternal(BPlusTreeNode*& root, BPlusTreeNode* node) {
  int mid_idx = (M - 1) / 2;
  int mid_key = node->keys[mid_idx];

  BPlusTreeNode* right_node = new BPlusTreeNode(false);
  for(size_t i = mid_idx + 1; i < node->keys.size(); i++) {
    right_node->keys.push_back(node->keys[i]);
  }
  node->keys.erase(node->keys.begin() + mid_idx, node->keys.end());

  for(size_t i = mid_idx + 1; i < node->children.size(); i++) {
    right_node->children.push_back(node->children[i]);
    node->children[i]->parent = right_node; 
  }
  node->children.erase(node->children.begin() + mid_idx + 1, node->children.end());
  right_node->parent = node->parent;

  if(node->parent == nullptr) {
    BPlusTreeNode* new_root = new BPlusTreeNode(false);
    new_root->keys.push_back(mid_key);
    new_root->children.push_back(node);
    new_root->children.push_back(right_node);
    node->parent = new_root;
    right_node->parent = new_root;
    root = new_root;
  } else {
    InsertKeyAndChild(node->parent, mid_key, right_node);
    if(node->parent->keys.size() > M - 1) {
      SplitInternal(root, node->parent);
    }
  }

}


// 叶子结点分裂
void SplitLeaf(BPlusTreeNode*& root, BPlusTreeNode* node) {
  // 先确定分裂点，把右边的元素挪到新结点中去再从原结点中删除
  BPlusTreeNode* right_node = new BPlusTreeNode(true);
  int mid_idx = M / 2;
  for(size_t i = mid_idx; i < node->keys.size(); i++) {
    right_node->keys.push_back(node->keys[i]);
  }
  node->keys.erase(node->keys.begin() + mid_idx, node->keys.end());

  // 更新双向链表的连接关系
  if(node->next) node->next->pre = right_node;
  right_node->pre = node;
  right_node->next = node->next;
  node->next = right_node;

  // 路标上移
  int split_key = right_node->keys[0];

  // node就是根叶子
  if(node->parent == nullptr) {
    // 创建新根，把结点挂上去
    BPlusTreeNode* new_root = new BPlusTreeNode(false);
    new_root->keys.push_back(split_key);
    new_root->children.push_back(node);
    new_root->children.push_back(right_node);
    node->parent = new_root;
    right_node->parent = new_root;
    root = new_root;
  } else {
    InsertKeyAndChild(node->parent, split_key, right_node);
    if(node->parent->keys.size() > M - 1) {
      SplitInternal(root, node->parent);
    }
  }

}



// 插入
void Insert(BPlusTreeNode*& root, int val) { 
  if(root == nullptr) {
    root = new BPlusTreeNode(true);
    root->keys.push_back(val);
    return ;
  }
  BPlusTreeNode* node = FindLeaf(root, val);
  if(!node) return ;
  InsertKeyAndChild(node, val, nullptr);
  if(node->keys.size() > M - 1) {
    SplitLeaf(root, node);
  }
}

// 获得孩子在父结点的下标
int GetChildIndexInParent(BPlusTreeNode* node) {
  if(!node || !node->parent) return -1;
  BPlusTreeNode* parent = node->parent;
  size_t i = 0;
  while(i < parent->children.size()) {
    if(parent->children[i] == node) return i;
    i++;
  }
  return -1;
}

// 向左叶兄弟借
void BorrowFromLeftLeaf(BPlusTreeNode* node, int idx) {
  // 左兄弟最后的值弹到队首并删除，更新上级路标
  BPlusTreeNode* parent = node->parent;
  BPlusTreeNode* left_brother = parent->children[idx - 1];
  node->keys.insert(node->keys.begin(), left_brother->keys.back());
  left_brother->keys.pop_back();
  parent->keys[idx - 1] = node->keys[0];
}


// 向右叶兄弟借
void BorrowFromRightLeaf(BPlusTreeNode* node, int idx) {
  BPlusTreeNode* parent = node->parent;
  BPlusTreeNode* right_brother = parent->children[idx + 1];
  node->keys.push_back(right_brother->keys.front());
  right_brother->keys.erase(right_brother->keys.begin());
  parent->keys[idx] = right_brother->keys[0];
}


// 将叶子结点的idx+1合并到idx
void MergeLeaf(BPlusTreeNode* parent, int idx) {
  BPlusTreeNode* left = parent->children[idx];
  BPlusTreeNode* right = parent->children[idx + 1];
  // 把right的值搬到left后面并抹去原位置，更新双向链表连接关系，删除parent对应right的下标，释放right内存
  for(size_t i = 0; i < right->keys.size(); i++) {
    left->keys.push_back(right->keys[i]);
  }
  right->keys.clear();

  if(right->next) right->next->pre = left;
  left->next = right->next;

  parent->keys.erase(parent->keys.begin() + idx);
  parent->children.erase(parent->children.begin() + idx + 1);

  delete right;
}



// 向左兄弟借
void BorrowFromLeftInternal(BPlusTreeNode* node, int p_idx) {
  // 父结点的值下来，左兄弟末尾的值上去，左兄弟末尾的孩子流过来
  BPlusTreeNode* parent = node->parent;
  BPlusTreeNode* left_brother = parent->children[p_idx - 1];

  node->keys.insert(node->keys.begin(), parent->keys[p_idx - 1]);
  parent->keys[p_idx - 1] = left_brother->keys.back();
  left_brother->keys.pop_back();

  node->children.insert(node->children.begin(), left_brother->children.back());
  left_brother->children.back()->parent = node;
  left_brother->children.pop_back();
}

// 向右兄弟借
void BorrowFromRightInternal(BPlusTreeNode* node, int p_idx) {
  BPlusTreeNode* parent = node->parent;
  BPlusTreeNode* right_brother = parent->children[p_idx + 1];

  node->keys.push_back(parent->keys[p_idx]);
  parent->keys[p_idx] = right_brother->keys.front();
  right_brother->keys.erase(right_brother->keys.begin());

  node->children.push_back(right_brother->children.front());
  right_brother->children.front()->parent = node;
  right_brother->children.erase(right_brother->children.begin());
}

// 将非叶结点的idx+1合并到idx
void MergeInternal(BPlusTreeNode* parent, int idx) {
  BPlusTreeNode* left = parent->children[idx];
  BPlusTreeNode* right = parent->children[idx + 1];
  // 父结点和右兄弟的值分别添加到末尾
  left->keys.push_back(parent->keys[idx]);
  parent->keys.erase(parent->keys.begin() + idx);
  for(size_t i = 0; i < right->keys.size(); i++) {
    left->keys.push_back(right->keys[i]);
  }
  right->keys.clear();
  // 领养右兄弟的孩子
  for(size_t i = 0; i < right->children.size(); i++) {
    left->children.push_back(right->children[i]);
    right->children[i]->parent = left;
  }
  right->children.clear();
  parent->children.erase(parent->children.begin() + idx + 1);
  delete right;
}

// 处理下溢出修正函数
void FixUnderFlow(BPlusTreeNode*& root, BPlusTreeNode* cur) {
  // 如果是根结点发生了下溢出
  if(cur == root) {
    // 根结点是叶子结点，说明整棵树都空了
    if(root->is_leaf) {
      delete root;
      root = nullptr;
    } else {
      // 否则让孩子成为新根
      BPlusTreeNode* old_root = root;
      root = root->children[0];
      root->parent = nullptr;
      delete old_root;
    }
    return ;
  }

  size_t idx = GetChildIndexInParent(cur);
  BPlusTreeNode* parent = cur->parent;

  // cur是叶子结点
  if(cur->is_leaf) {
    // 左兄弟够借
    if(idx > 0 && parent->children[idx - 1]->keys.size() > (M - 1) / 2) {
      BorrowFromLeftLeaf(cur, idx);
      return ;
    } 
    // 右兄弟够借
    else if (idx < parent->children.size() - 1 && parent->children[idx + 1]->keys.size() > (M - 1) / 2) {
      BorrowFromRightLeaf(cur, idx);
      return ;
    } 
    // 左右都不够借，合并
    else {
      if(idx > 0) {
        MergeLeaf(parent, idx - 1);
      } else {
        MergeLeaf(parent, idx);
      }
    }
  }
  // cur不是叶子结点
  else {
    // 左兄弟够借
    if(idx > 0 && parent->children[idx - 1]->keys.size() > (M - 1) / 2) {
      BorrowFromLeftInternal(cur, idx);
      return ;
    } 
    // 右兄弟够借
    else if (idx < parent->children.size() - 1 && parent->children[idx + 1]->keys.size() > (M - 1) / 2) {
      BorrowFromRightInternal(cur, idx);
      return ;
    } 
    // 左右都不够借，合并
    else {
      if(idx > 0) {
        MergeInternal(parent, idx - 1);
      } else {
        MergeInternal(parent, idx);
      }
    }
  }

  if((parent == root && root->keys.empty()) || (parent != root && parent->keys.size() < (M - 1) / 2)){
    FixUnderFlow(root, parent);
  }

  return ;
}

// 删除指定元素
void Delete(BPlusTreeNode*& root, int val) {
  if(!root) return;
  BPlusTreeNode* node = FindLeaf(root, val);
  if(!node) return ;

  size_t idx = 0;
  while(idx < node->keys.size()) {
    if(val == node->keys[idx]) break;
    idx++;
  }
  if(idx == node->keys.size()) return ;

  node->keys.erase(node->keys.begin() + idx);
  if((node == root && node->keys.empty()) || (node != root && node->keys.size() < (M - 1) / 2)) {
    FixUnderFlow(root, node);
  }

  return ;
}





// ----------------------------  以下为辅助验证代码    --------------------------------------------













// 递归直观打印树形结构（展示路由区间和父子归属）
void PrintPrettyTree(BPlusTreeNode* node, std::string prefix = "", bool is_last = true) {
    if (!node) {
        std::cout << "[空树 Empty Tree]" << std::endl;
        return;
    }

    std::cout << prefix;
    std::cout << (is_last ? "└── " : "├── ");

    // 1. 打印当前节点信息与键
    if (node->is_leaf) {
        std::cout << "【叶子 Leaf】[ ";
        for (size_t i = 0; i < node->keys.size(); i++) {
            std::cout << node->keys[i] << (i + 1 < node->keys.size() ? ", " : " ");
        }
        std::cout << "]";
        // 打印前后驱指针辅助验证
        if (node->pre)  std::cout << " (pre: " << node->pre->keys.back() << ")";
        if (node->next) std::cout << " (next: " << node->next->keys.front() << ")";
        std::cout << std::endl;
    } else {
        std::cout << "【内部 Internal】[ ";
        for (size_t i = 0; i < node->keys.size(); i++) {
            std::cout << node->keys[i] << (i + 1 < node->keys.size() ? " | " : " ");
        }
        std::cout << "]" << std::endl;

        // 2. 递归打印每一个孩子分支
        for (size_t i = 0; i < node->children.size(); i++) {
            bool last_child = (i == node->children.size() - 1);
            PrintPrettyTree(node->children[i], prefix + (is_last ? "    " : "│   "), last_child);
        }
    }
}

// 打印全量叶子链表（验证顺序和穿针引线）
void PrintLeafChain(BPlusTreeNode* root) {
    if (!root) return;
    BPlusTreeNode* cur = root;
    while (!cur->is_leaf) {
        cur = cur->children[0];
    }
    std::cout << "\n 底层双向链表全景:\n   ";
    while (cur) {
        std::cout << "[";
        for (size_t i = 0; i < cur->keys.size(); i++) {
            std::cout << cur->keys[i] << (i + 1 < cur->keys.size() ? " " : "");
        }
        std::cout << "]";
        if (cur->next) {
            std::cout << " <===> ";
        }
        cur = cur->next;
    }
    std::cout << " -> nullptr\n" << std::endl;
}


int main() {
    BPlusTreeNode* root = nullptr;

    std::cout << "=========================================" << std::endl;
    std::cout << "   1. 连续插入 10, 20, 30, 40 (触发叶子分裂)  " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::vector<int> init_vals = {10, 20, 30, 40};
    for (int v : init_vals) {
        Insert(root, v);
    }
    PrintPrettyTree(root);
    PrintLeafChain(root);

    std::cout << "=========================================" << std::endl;
    std::cout << "   2. 继续插入 50, 60, 70, 80 (多次分裂)      " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::vector<int> more_vals = {50, 60, 70, 80};
    for (int v : more_vals) {
        Insert(root, v);
    }
    PrintPrettyTree(root);
    PrintLeafChain(root);

    std::cout << "=========================================" << std::endl;
    std::cout << "   3. 删除 10 (触发叶子向右兄弟借调)          " << std::endl;
    std::cout << "=========================================" << std::endl;
    Delete(root, 10);
    PrintPrettyTree(root);
    PrintLeafChain(root);

    std::cout << "=========================================" << std::endl;
    std::cout << "   4. 删除 20 (欠费且兄弟不够借，触发叶子合并) " << std::endl;
    std::cout << "=========================================" << std::endl;
    Delete(root, 20);
    PrintPrettyTree(root);
    PrintLeafChain(root);

    return 0;
}























