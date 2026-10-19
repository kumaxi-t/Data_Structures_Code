#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

class LoserTree {
private:
  int k;                    // k路归并
  std::vector<int> ls;      // 内部结点
  std::vector<int> leaves;  // 叶子结点
  
public:

  LoserTree(std::vector<int>& initial_keys) {
    k = static_cast<int>(initial_keys.size());
    ls.resize(k);
    leaves.resize(k + 1);
    for(int i = 0; i < k; i++) {
      leaves[i] = initial_keys[i];
    }
    leaves[k] = INT_MIN;
    for(int i = 0; i < k; i++) {
      ls[i] = k;
    }
    // 从后往前，不然会顶峰冲突丢失，要先让兄弟上位再去比试
    for(int i = k - 1; i >= 0; i--) {
      adjust(i);
    }
  }

  void adjust(int s) {
    int parent = (s + k) / 2;         // 父亲的下标
    while(parent > 0) {
      // s比守门员还大，把s留下守门员代替s向上挑战
      if(leaves[s] > leaves[ls[parent]]) {
        std::swap(s, ls[parent]);
      }
      parent = parent / 2;
    }
    ls[0] = s;
  }

  int GetWinnerIndex() const {
    return ls[0];
  }

  int GetWinnerValue() const {
    return leaves[ls[0]];
  }

  void ReplaceWinner(int new_val) {
    int winner = GetWinnerIndex();
    leaves[winner] = new_val;
    adjust(winner);
  }



};




// ----------------------测试代码----------------------------

int main() {
    // 1. 对应我们手动推演的 4 位选手：0号=9, 1号=30, 2号=15, 3号=20
    std::vector<int> init_vals = {9, 30, 15, 20};
    LoserTree tree(init_vals);

    std::cout << "初始冠军: " << tree.GetWinnerIndex() 
              << "号选手 (值: " << tree.GetWinnerValue() << ")\n";

    // 2. 模拟第一轮：0号出局，换成 25（0号输给1号留在ls[1]，2号晋级登顶）
    tree.ReplaceWinner(25);
    std::cout << "换入25后新冠军: " << tree.GetWinnerIndex() 
              << "号选手 (值: " << tree.GetWinnerValue() << ")\n";

    // 3. 模拟第二轮：2号出局，换成 18（一路高歌杀到0）
    tree.ReplaceWinner(18);
    std::cout << "换入18后新冠军: " << tree.GetWinnerIndex() 
              << "号选手 (值: " << tree.GetWinnerValue() << ")\n";

    return 0;
}