#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <cassert>
#include <iomanip>

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
    size_t i = 0;
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
  size_t i = 0;
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

  for(size_t i = mid_idx + 1; i < node->keys.size(); i++) {
    right_node->keys.emplace_back(node->keys[i]);
  } 
  node->keys.erase(node->keys.begin() + mid_idx, node->keys.end());

  if(!node->is_leaf) {
    for(size_t i = mid_idx + 1; i < node->children.size(); i++) {
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



// 插入操作
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




// 在树中找到包含val的节点指针并返回其下标idx
BTreeNode* FindNode(BTreeNode* root, int val, int& idx) {
  if(root == nullptr) return nullptr;

  BTreeNode* cur = root;
  while(cur != nullptr) {
    size_t i = 0;
    while(i < cur->keys.size()) {
      if(cur->keys[i] > val) break;
      // 找到了
      if(cur->keys[i] == val) {
        idx = i;
        return cur;
      }
      i++;
    }
    if(cur->is_leaf) break;
  
    cur = cur->children[i];
  }
  return nullptr;

}


// 找直接后继
BTreeNode* GetSuccessorLeaf(BTreeNode* node, int idx, int& succ_val) {

  BTreeNode* cur = node->children[idx + 1];
  while(cur->is_leaf != true) {
    cur = cur->children[0];
  }
  succ_val = cur->keys[0];
  return cur;

}


// 找到node在父结点children数组的下标
int GetChildIndexInParent(BTreeNode* node) {
  BTreeNode* parent = node->parent;
  for(size_t i = 0; i < parent->children.size(); i++) {
    if(parent->children[i] == node) {
      return i;
    }
  }
  return -1;
}

// 向左兄弟借
void BorrowFromLeft(BTreeNode* node, int p_idx) {
  BTreeNode* parent = node->parent;
  BTreeNode* left_brother = parent->children[p_idx - 1];
  // 父值下
  node->keys.insert(node->keys.begin(), parent->keys[p_idx - 1]);
  parent->keys[p_idx - 1] = left_brother->keys.back();
  left_brother->keys.pop_back();
  // 兄值上
  if(!node->is_leaf) {
    node->children.insert(node->children.begin(), left_brother->children.back());
    left_brother->children.back()->parent = node;
    left_brother->children.pop_back();
  }

}


// 向右兄弟借
void BorrowFromRight(BTreeNode* node, int p_idx) {
  BTreeNode* parent = node->parent;
  BTreeNode* right_brother = parent->children[p_idx + 1];
  // 父值下
  node->keys.insert(node->keys.end(), parent->keys[p_idx]);
  parent->keys[p_idx] = right_brother->keys.front();
  right_brother->keys.erase(right_brother->keys.begin());
  // 兄值上
  if(!node->is_leaf) {
    node->children.insert(node->children.end(), right_brother->children.front());
    right_brother->children.front()->parent = node;
    right_brother->children.erase(right_brother->children.begin());
  }

}

// 将parent下标为idx和idx+1的孩子进行合并
void Merge(BTreeNode* parent, int idx) {
  BTreeNode* left_node = parent->children[idx];
  BTreeNode* right_node = parent->children[idx + 1];

  // 父结点值下移，右兄弟合并过去，右兄弟的孩认新爹，销毁右兄弟
  left_node->keys.push_back(parent->keys[idx]);
  parent->keys.erase(parent->keys.begin() + idx);

  for(size_t i = 0; i < right_node->keys.size(); i++) {
    left_node->keys.push_back(right_node->keys[i]);
  }

  for(size_t i = 0; i < right_node->children.size(); i++) {
    left_node->children.push_back(right_node->children[i]);
    right_node->children[i]->parent = left_node;
  }
  parent->children.erase(parent->children.begin() + idx + 1);
  delete right_node;


}


// 出现下溢出后的修复函数
void FixUnderflow(BTreeNode*& root, BTreeNode* cur) {
  // 先判断是否为根节点发生了下溢出
  if(cur == root) {
    // 根结点不是叶子结点，说明他还有孩子，让孩子继任
    if(cur->is_leaf != true) {
      BTreeNode* old_root = root;
      root = root->children[0];
      root->parent = nullptr;
      delete old_root;
    }
    // 根节点就是叶子节点说明树已经空了
    else {
      delete root;
      root = nullptr;
    }
    return ;
  }

  // 先找兄弟借
  BTreeNode* parent = cur->parent;
  int idx = GetChildIndexInParent(cur);
  
  // 左兄弟够借
  if(idx > 0 && parent->children[idx - 1]->keys.size() > (M - 1) / 2) {
    BorrowFromLeft(cur, idx);  
    return ;
  } 
  // 右兄弟够借
  else if (idx < parent->children.size() - 1 && parent->children[idx + 1]->keys.size() > (M - 1) / 2) {
    BorrowFromRight(cur, idx);
    return ;
  } 
  // 左右都不够借
  else {
    // 拉着左兄弟合并
    if(idx > 0) {
      Merge(parent, idx - 1);
    }
    // 拉着右兄弟合并
    else {
      Merge(parent, idx);
    }

    // 合并完了检查父结点是否下溢出
    if((parent == root && parent->keys.empty()) ||
      (parent != root && parent->keys.size() < (M - 1) / 2)) {
      FixUnderflow(root, parent);
    }
  }



}





// 从树中删除指定的元素
void Delete(BTreeNode*& root, int val) {

  if(root == nullptr) return ;
  // 先转换为对其直接后继的删除
  int idx = -1;
  BTreeNode* node = FindNode(root, val, idx);
  if(node == nullptr) return ;
  // node不是叶子结点，替换当前结点的值，再把删除对象转换为其直接后继
  if(!node->is_leaf) {
    int succ_val;
    BTreeNode* succ_leaf = GetSuccessorLeaf(node, idx, succ_val);
    node->keys[idx] = succ_val;
    node = succ_leaf;
    // 直接后继是最左边的元素
    idx = 0;
  } 

  // 此时要删除的结点都处于叶子结点
  node->keys.erase(node->keys.begin() + idx);
  // 如果删除结点为根结点且空了或者其他结点发生了下溢出
  if((node == root && node->keys.empty()) || (node != root && node->keys.size() < (M - 1) / 2)) {
    FixUnderflow(root, node);
  }

}





// ---------------------- 以下为辅助测试代码 ----------------------





// =========================================================================
// 1. 直观的树形缩进打印器（自顶向下树枝图）
// =========================================================================
void PrintBTreeVisual(BTreeNode* node, const std::string& prefix = "", bool is_last = true) {
    if (node == nullptr) return;

    std::cout << prefix;
    std::cout << (is_last ? "└── " : "├── ");

    // 打印当前节点的关键字
    std::cout << "[";
    for (size_t i = 0; i < node->keys.size(); ++i) {
        std::cout << node->keys[i] << (i + 1 < node->keys.size() ? " | " : "");
    }
    std::cout << "]";

    // 附带显示节点属性信息
    if (node->parent == nullptr) {
        std::cout << " (Root)";
    }
    if (node->is_leaf) {
        std::cout << " [Leaf]";
    }
    std::cout << "\n";

    // 递归打印孩子分支
    if (!node->is_leaf) {
        for (size_t i = 0; i < node->children.size(); ++i) {
            bool child_is_last = (i + 1 == node->children.size());
            std::string child_prefix = prefix + (is_last ? "    " : "│   ");
            PrintBTreeVisual(node->children[i], child_prefix, child_is_last);
        }
    }
}

// =========================================================================
// 2. 严格的 B 树数学公理验证器（一旦指针或容量出错直接捕获报错）
// =========================================================================
struct AuditResult {
    bool valid = true;
    int leaf_depth = -1; // 用于验证所有叶子必须处于同一深度
    std::string error_msg = "";
};

bool VerifyBTreeProperties(BTreeNode* node, BTreeNode* expected_parent, int current_depth, AuditResult& res, bool is_root = false) {
    if (node == nullptr) return true;

    // 1. 双向指针核验：检查我的 parent 是否真实指向传入的父指针
    if (node->parent != expected_parent) {
        res.valid = false;
        res.error_msg = "父指针断裂或反向绑定错误！";
        return false;
    }

    // 2. 单调有序性核验：keys 必须严格升序
    for (size_t i = 1; i < node->keys.size(); ++i) {
        if (node->keys[i] <= node->keys[i - 1]) {
            res.valid = false;
            res.error_msg = "节点内部关键字未严格保持升序！";
            return false;
        }
    }

    // 3. 关键字数量合法性核验
    if (!is_root) {
        size_t min_keys = (M - 1) / 2; // 下限
        size_t max_keys = M - 1;       // 上限
        if (node->keys.size() < min_keys || node->keys.size() > max_keys) {
            res.valid = false;
            res.error_msg = "非根节点的键数量超出 B 树合法范围 [min, max]！";
            return false;
        }
    } else {
        if (node->keys.size() > M - 1) {
            res.valid = false;
            res.error_msg = "根节点的键数量超过了最大上限 M-1！";
            return false;
        }
    }

    // 4. 孩子与键数量对应关系核验
    if (!node->is_leaf) {
        if (node->children.size() != node->keys.size() + 1) {
            res.valid = false;
            res.error_msg = "非叶节点的 children 数量不等于 keys 数量 + 1！";
            return false;
        }
    } else {
        if (!node->children.empty()) {
            res.valid = false;
            res.error_msg = "叶子节点的 children 容器不为空！";
            return false;
        }
    }

    // 5. 等高性核验：所有叶子必须在同一个深度
    if (node->is_leaf) {
        if (res.leaf_depth == -1) {
            res.leaf_depth = current_depth;
        } else if (res.leaf_depth != current_depth) {
            res.valid = false;
            res.error_msg = "B 树平衡性被破坏：不同叶子节点深度不一致！";
            return false;
        }
        return true;
    }

    // 6. 递归核验下层孩子
    for (size_t i = 0; i < node->children.size(); ++i) {
        if (!VerifyBTreeProperties(node->children[i], node, current_depth + 1, res, false)) {
            return false;
        }
    }

    return true;
}

void RunBTreeAudit(BTreeNode* root) {
    if (root == nullptr) {
        std::cout << "  [检验结果]: 空树，结构合法。\n";
        return;
    }
    AuditResult res;
    VerifyBTreeProperties(root, nullptr, 0, res, true);
    if (res.valid) {
        std::cout << "  [检验结果]: √ 结构完备！叶子深度一致（Height=" << res.leaf_depth + 1 << "），指针双向绑定无异常。\n";
    } else {
        std::cout << "  [检验结果]: × 发现结构异常: " << res.error_msg << "\n";
    }
}

// =========================================================================
// 3. 场景测试驱动器：演示关键操作并可视化推演
// =========================================================================
void ExecInsertStep(BTreeNode*& root, int val) {
    std::cout << "\n======================================================\n";
    std::cout << ">>> [操作]: 插入值 " << val << "\n";
    std::cout << "======================================================\n";
    Insert(root, val);
    PrintBTreeVisual(root);
    RunBTreeAudit(root);
}

void ExecDeleteStep(BTreeNode*& root, int val, const std::string& expect_scene) {
    std::cout << "\n======================================================\n";
    std::cout << ">>> [操作]: 删除值 " << val << " (" << expect_scene << ")\n";
    std::cout << "======================================================\n";
    Delete(root, val);
    PrintBTreeVisual(root);
    RunBTreeAudit(root);
}

int main() {
    BTreeNode* root = nullptr;

    std::cout << "######################################################\n";
    std::cout << "#             4 阶 B 树增删完整场景全真演练          #\n";
    std::cout << "######################################################\n";

    // 阶段 1：逐步插入填满叶子并触发首次分裂（根分裂增高）
    ExecInsertStep(root, 10);
    ExecInsertStep(root, 20);
    ExecInsertStep(root, 30); // 此时叶子有 [10, 20, 30]
    ExecInsertStep(root, 40); // 触发分裂！中间数 20 升为新根，树高变 2

    // 阶段 2：继续插入触发下层分裂与叶子扩充
    ExecInsertStep(root, 50);
    ExecInsertStep(root, 60);
    ExecInsertStep(root, 70);
    ExecInsertStep(root, 80);

    // 阶段 3：测试删除场景 A —— 普通叶子删除（无下溢）
    ExecDeleteStep(root, 80, "叶子还有余粮，直接删除");

    // 阶段 4：测试删除场景 B —— 触发借调（向左兄弟借调）
    ExecDeleteStep(root, 70, "引发叶子欠费，向左兄弟借调平账");

    // 阶段 5：测试删除场景 C —— 触发后继替换（删除内部节点）
    ExecDeleteStep(root, 40, "删除内部骨架节点，后继节点值上升替换");

    // 阶段 6：测试删除场景 D —— 触发兄弟贫困合并（Merge）
    ExecDeleteStep(root, 60, "左右皆穷无法借调，拉下父节点触发 Merge");

    // 阶段 7：测试删除场景 E —— 一路合并导致根节点掏空，整树降高
    ExecDeleteStep(root, 50, "触发合并波及老根，独生子登基，整树变矮");

    return 0;
}