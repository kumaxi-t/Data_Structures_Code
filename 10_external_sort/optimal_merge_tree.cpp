#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>



struct MergeTreeNode {
  int weight;
  std::vector<MergeTreeNode*> children;

  MergeTreeNode(int w) : weight(w) {}
};


struct CompareNode {
  bool operator()(const MergeTreeNode* a, const MergeTreeNode* b) const {
    return a->weight > b->weight;
  }
};

MergeTreeNode* BuildOptimalMergeTree(const std::vector<int>& runs_weight, int k, int& total_cost) {

  total_cost = 0;
  if(runs_weight.empty() || k < 2) return nullptr;

  int n = static_cast<int>(runs_weight.size());

  int dummy_count = 0;
  int remainder = (n - 1) % (k - 1);
  if(remainder != 0) {
    dummy_count = (k - 1) - remainder;
  }
  std::priority_queue<MergeTreeNode*, std::vector<MergeTreeNode*>, CompareNode> min_pq;


  for(int w : runs_weight) {
    min_pq.push(new MergeTreeNode(w));
  }
  for(int i = 0; i < dummy_count; i++) {
    min_pq.push(new MergeTreeNode(0));
  }


  while(min_pq.size() > 1) {
    int merge_weight = 0;

    MergeTreeNode* parent = new MergeTreeNode(0);

    for(int i = 0; i < k; i++) {
      auto child = min_pq.top();
      min_pq.pop();
      merge_weight += child->weight;
      parent->children.emplace_back(child);
    }
    parent->weight = merge_weight;

    total_cost += merge_weight;
    min_pq.emplace(parent);
  }

  return min_pq.top();

}






// ---------------------以下为辅助验证代码-------------------------------









// 递归打印归并树形态（根据缩进层级直观展示）
void PrintMergeTree(MergeTreeNode* root, int depth = 0) {
  if (!root) return;

  // 根据当前深度打印缩进
  for (int i = 0; i < depth; ++i) {
    std::cout << "    ";
  }

  // 如果没有孩子，说明是初始段（叶子节点）
  if (root->children.empty()) {
    if (root->weight == 0) {
      std::cout << "[虚段: 0]\n";
    } else {
      std::cout << "段长度: " << root->weight << "\n";
    }
  } else {
    // 内部节点（归并产生的段）
    std::cout << "归并节点(权值 " << root->weight << "):\n";
    for (auto* child : root->children) {
      PrintMergeTree(child, depth + 1);
    }
  }
}

// 递归释放整棵树开辟的内存，防止内存泄漏
void FreeMergeTree(MergeTreeNode* root) {
  if (!root) return;
  for (auto* child : root->children) {
    FreeMergeTree(child);
  }
  delete root;
}



int main() {
  // ---------------- 测试 1: 5 个初始归并段，3 路归并 ----------------
  std::cout << "=== 测试 1: 5 个初始段，3 路归并 ===\n";
  std::vector<int> runs1 = {2, 3, 5, 7, 9};
  int k1 = 3;
  int total_cost1 = 0;

  MergeTreeNode* root1 = BuildOptimalMergeTree(runs1, k1, total_cost1);
  std::cout << "最佳归并树结构:\n";
  PrintMergeTree(root1);
  std::cout << "总 I/O 读写代价 (WPL): " << total_cost1 << "\n\n";
  FreeMergeTree(root1);

  // ---------------- 测试 2: 8 个初始归并段，3 路归并 ----------------
  std::cout << "=== 测试 2: 8 个初始段，3 路归并（触发虚段）===\n";
  std::vector<int> runs2 = {2, 3, 6, 9, 12, 17, 25, 30};
  int k2 = 3;
  int total_cost2 = 0;

  MergeTreeNode* root2 = BuildOptimalMergeTree(runs2, k2, total_cost2);
  std::cout << "最佳归并树结构:\n";
  PrintMergeTree(root2);
  std::cout << "总 I/O 读写代价 (WPL): " << total_cost2 << "\n";
  FreeMergeTree(root2);

  return 0;
}
