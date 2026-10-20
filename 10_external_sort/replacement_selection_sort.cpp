#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>


struct Element {
  int value;
  int run_id;

  bool operator<(const Element& other) const {
    if(run_id != other.run_id) {
      return run_id < other.run_id;
    }
    return value < other.value;
  }
  bool operator>(const Element& other) const {
    if(run_id != other.run_id) {
      return run_id > other.run_id;
    }
    return value > other.value;
  }
};


class ReplacementSelectionTree {
private:
  int w;                  // 工作区容量，败者树归并路数
  std::vector<int> ls;
  std::vector<Element> leaves;



public:

  void adjust(int s) {
    int parent = (s + w) / 2;
    while(parent > 0) {
      if(leaves[s] > leaves[ls[parent]]) {
        std::swap(s, ls[parent]);
      }
      parent /= 2;
    }
    ls[0] = s;
  }


  ReplacementSelectionTree(std::vector<Element>& init_elements) {
    w = static_cast<int>(init_elements.size());
    ls.resize(w);
    leaves.resize(w + 1);
    for(int i = 0; i < w; i++) {
      leaves[i] = init_elements[i];
    }
    leaves[w] = {INT_MIN, INT_MIN};
    for(int i = 0; i < w; i++) {
      ls[i] = w;
    }
    for(int i = w - 1; i >= 0; i--) {
      adjust(i);
    }
  }

  int GetWinnerIndex() const {
    return ls[0];
  }

  Element GetWinner() const {
    return leaves[ls[0]];
  }

  void ReplaceWinner(const Element& new_ele) {
    int winner = GetWinnerIndex();
    leaves[winner] = new_ele;
    adjust(winner);
  }

};



std::vector<std::vector<int>> ReplacementSelectionSort(const std::vector<int>& input, int w) {
  int n = static_cast<int>(input.size());

  std::vector<std::vector<int>> res;
  std::vector<Element> init_elements;

  // 初始化败者树
  int input_idx = 0;
  for(; input_idx < n && input_idx < w; input_idx++) {
    init_elements.push_back({input[input_idx], 1});
  }
  ReplacementSelectionTree tree(init_elements);

  int cur_runid = 1;          // 当前段id
  std::vector<int> cur_run;   // 当前段元素

  while(true) {
    Element winner = tree.GetWinner();
    // 都处理完了
    if(winner.run_id == INT_MAX) {
      // 把剩下的段元素加进去
      if(!cur_run.empty()) res.emplace_back(cur_run);
      break;
    }

    // 到下一个段了，收尾
    if(winner.run_id > cur_runid) {
      res.emplace_back(cur_run);
      cur_run.clear();
      cur_runid = winner.run_id;
    }

    cur_run.emplace_back(winner.value);
    // input里还有元素
    if(input_idx < n) {
      int new_val = input[input_idx];
      input_idx++;
      // 新加进来的元素如果比弹出去的元素还要小就没机会比他更大了，就是下一个段的元素了
      int new_id = (new_val >= winner.value) ? winner.run_id : winner.run_id + 1;
      tree.ReplaceWinner({new_val, new_id});
    } 
    // input的元素都处理完了，只剩树里剩下的元素
    else {
      tree.ReplaceWinner({INT_MAX, INT_MAX});
    }

  }


  return res;
}



//----------------------测试代码-------------------------



int main() {

  std::vector<int> data = {17, 21, 5, 44, 10, 12, 56, 32};
  int w = 3;

  std::cout << "输入序列: ";
  for (int x : data) std::cout << x << " ";
  std::cout << "\n工作区容量 w = " << w << "\n\n";

  auto runs = ReplacementSelectionSort(data, w);

  std::cout << "=== 置换-选择排序生成结果 ===\n";
  std::cout << "共生成了 " << runs.size() << " 个初始归并段:\n";
  for (size_t i = 0; i < runs.size(); ++i) {
    std::cout << "段 R" << (i + 1) << " (长度 " << runs[i].size() << "): ";
    for (int x : runs[i]) {
      std::cout << x << " ";
    }
    std::cout << "\n";
  }

  return 0;
}