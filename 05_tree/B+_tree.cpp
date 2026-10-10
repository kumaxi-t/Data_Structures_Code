#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

