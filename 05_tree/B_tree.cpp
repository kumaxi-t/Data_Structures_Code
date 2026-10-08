#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

static const int M = 4; 

struct BTreeNode {
  

  std::vector<int> keys;
  std::vector<BTreeNode*> children;
  bool is_leaf;
  BTreeNode* parent;

  BTreeNode(bool leaf = true) : is_leaf(leaf), parent(nullptr) {}

};

// 找到要插入的叶子节点
BTreeNode* FindLeaf(BTreeNode* root, int val) {
  BTreeNode* cur = root;
  while(cur->is_leaf != true) {
    int i = 0;
    while(i < cur->keys.size()) {
      if(cur->keys[i] > val) break;
      i++;
    }
    cur = cur->children[i];
  } 
  return cur;
}


// 底部分裂给父结点带来一个新节点和其附属的右孩子
// 把key和child插入到node当中去
void InsertKeyAndChild(BTreeNode* node, int key, BTreeNode* child = nullptr) {
  int i = 0;
  while(i < node->keys.size()) {
    if(key < node->keys[i]) break;
    i++;
  }
  node->keys.insert(node->keys.begin() + i, key);
  if(child != nullptr) {
    node->children.insert(node->children.begin() + i + 1, child);
    child->parent = node;
  }
}

// 元素满了要向上分裂
void Split(BTreeNode*& root, BTreeNode* node) {
  int mid_idx = (M / 2) - 1;
  int mid_key = node->keys[mid_idx];

  BTreeNode* right_node = new BTreeNode(node->is_leaf);

  for(int i = mid_idx + 1; i < node->keys.size(); i++) {
    right_node->keys.emplace_back(node->keys[i]);
  } 
  node->keys.erase(node->keys.begin() + mid_idx, node->keys.end());

  if(!node->is_leaf) {
    for(int i = mid_idx + 1; i < node->children.size(); i++) {
      right_node->children.emplace_back(node->children[i]);
      node->children[i]->parent = right_node;
    } 
    node->children.erase(node->children.begin() + mid_idx + 1, node->children.end());
  }

  if(node->parent == nullptr) {
    BTreeNode* new_root = new BTreeNode(false);
    new_root->keys.emplace_back(mid_key);
    new_root->children.emplace_back(node);
    new_root->children.emplace_back(right_node);

    node->parent = new_root;
    right_node->parent = new_root;
    root = new_root;
  } else {
    InsertKeyAndChild(node->parent, mid_key, right_node);
    if(node->parent->keys.size() == M) Split(root, node->parent);
  }

}




void Insert(BTreeNode*& root, int val) {
  if(root == nullptr) {
    root = new BTreeNode(true);
    root->keys.emplace_back(val);
    return ;
  }

  BTreeNode* node = FindLeaf(root, val);

  InsertKeyAndChild(node, val, nullptr);

  if(node->keys.size() == M) {
    Split(root, node);
  }
}







