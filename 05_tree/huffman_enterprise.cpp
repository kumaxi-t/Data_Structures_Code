#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <cassert>

using namespace std;

// 1. 哈夫曼树结点定义
struct HuffmanNode {
    char ch;              // 字符（非叶子结点可记为 '\0'）
    int freq;             // 字符出现的频次/权重
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// 2. 小顶堆比较仿函数（频次低的优先级高）
struct Compare {
    bool operator()(HuffmanNode *a, HuffmanNode *b) {
        return a->freq > b->freq;
    }
};

// 辅助安全释放整树内存
void DestroyHuffmanTree(HuffmanNode* &root) {
    if (!root) return;
    DestroyHuffmanTree(root->left);
    DestroyHuffmanTree(root->right);
    delete root;
    root = nullptr;
}

// ============================================================================
//                       【工业级实战：哈夫曼树手撕核心区】
// ============================================================================

// 1. 频次统计
// 目标：遍历原始字符串，统计每个字符出现的次数
unordered_map<char, int> CountFrequency(const string &text) {
    unordered_map<char, int> freqMap;
    // 请在此编写你的代码：
    for(auto ch : text) {
        freqMap[ch]++;
    }
    return freqMap;
}

// 2. 基于小顶堆贪心构建哈夫曼树
// 目标：
//   1) 将所有独立字符作为叶子结点推入 priority_queue；
//   2) 循环弹出频次最小的两个结点 left 和 right；
//   3) 生成父结点 parent（频次为两者之和），并重新压入堆；
//   4) 最终堆顶即为哈夫曼树的根结点。
HuffmanNode* BuildHuffmanTree(const unordered_map<char, int> &freqMap) {
    // 请在此编写你的代码：
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for(auto& [ch, freq] : freqMap) {
        pq.emplace(new HuffmanNode(ch, freq));
    }
    while(pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        // 选择两个小的造一个新的数，再把新数重新加里面去，更新新结点的左右子树
        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }
    // 如果不为空剩下的就是根节点
    return pq.empty() ?  nullptr : pq.top();
}

// 3. DFS 生成前缀编码表（左 0 右 1）
// 目标：从根递归遍历，遇到叶子结点时将路径生成的 0/1 字符串记录到 codeMap
void GenerateCodes(HuffmanNode *root, string code, unordered_map<char, string> &codeMap) {
    // 请在此编写你的代码：
    if(!root) return ;
    // 到了叶子结点把累加的code记录到映射的codeMap里
    if(!root->left && !root->right) {
        codeMap[root->ch] = code;
        return ;
    }
    GenerateCodes(root->left, code + '0', codeMap);
    GenerateCodes(root->right, code + '1', codeMap);
}

// 4. 文本编码
// 目标：根据 codeMap 将原字符串逐字符转换为 0/1 二进制字符串
// 把abc转为二进制01数
string Encode(const string &text, const unordered_map<char, string> &codeMap) {
    string encodedStr = "";
    // 请在此编写你的代码：
    for(auto& c : text) {
        encodedStr += codeMap.at(c);
    }
    return encodedStr;
}

// 5. 二进制串译码解码
// 目标：从根结点出发，遇到 '0' 走左分支，遇到 '1' 走右分支，走到叶子结点即可输出对应字符并重置回根
// 把01二进制串转为原来的字符串
string Decode(HuffmanNode *root, const string &encodedStr) {
    string decodedStr = "";
    // 请在此编写你的代码：
    if(!root) return decodedStr;
    if(!root->left && !root->right) {
        return string(encodedStr.size(), root->ch);
    }

    HuffmanNode* cur = root;
    for(auto &c : encodedStr) {
        if(c == '0') {
            cur = cur->left;
        }else {
            cur = cur->right;
        }
        if(!cur->left && !cur->right) {
            decodedStr += cur->ch;
            cur = root;
        }
    }

    return decodedStr;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    cout << "============= 工业级实战【哈夫曼编解码系统】自测开始 =============\n\n";

    string text = "this is an example of a huffman tree";

    // 1. 测试频次统计
    auto freqMap = CountFrequency(text);
    bool freq_ok = (freqMap['e'] == 4 && freqMap[' '] == 7);
    cout << "测试 1 [字符频次统计]: " << (freq_ok ? "PASS" : "FAIL") << "\n";

    // 2. 测试哈夫曼树构建
    HuffmanNode *root = BuildHuffmanTree(freqMap);
    bool build_ok = (root != nullptr && root->freq == (int)text.size());
    cout << "测试 2 [小顶堆贪心建树与总根权重]: " << (build_ok ? "PASS" : "FAIL") 
         << " (根频次: " << (root ? root->freq : -1) << ")\n";

    // 3. 测试编码表生成
    unordered_map<char, string> codeMap;
    GenerateCodes(root, "", codeMap);
    bool code_table_ok = (codeMap.size() == freqMap.size());
    cout << "测试 3 [前缀编码表完整性]: " << (code_table_ok ? "PASS" : "FAIL") << "\n";

    // 4. 测试文本编码
    string encodedStr = Encode(text, codeMap);
    bool encode_ok = (!encodedStr.empty());
    cout << "测试 4 [文本编码转换]: " << (encode_ok ? "PASS" : "FAIL") 
         << " (压缩后位串长度: " << encodedStr.size() << " bits)\n";

    // 5. 测试逆向解码与无损还原
    string decodedStr = Decode(root, encodedStr);
    bool decode_ok = (decodedStr == text);
    cout << "测试 5 [无二义性解码还原]: " << (decode_ok ? "PASS" : "FAIL") << "\n";

    // 内存安全清理
    DestroyHuffmanTree(root);
    bool destroy_ok = (root == nullptr);
    cout << "测试 6 [树结构安全释放]: " << (destroy_ok ? "PASS" : "FAIL") << "\n";

    cout << "\n============= 自测流程全部结束 =============\n";
    return 0;
}