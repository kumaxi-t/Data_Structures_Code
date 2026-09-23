#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <string>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_YELLOW  "\033[1;33m"

// ============================================================================
//                     【流派一：10 个塑料桶直觉手撕区】
// ============================================================================

/**
 * 基数排序（直觉桶分配流派）
 * 核心动作：
 * 1. 找到全局最小值与最大值；若有负数，全员平移为非负数
 * 2. exp 从 1 开始按位递增 (exp = 1, 10, 100...)：
 *    - 准备 10 个桶：std::vector<int> buckets[10];
 *    - 分配：遍历当前数组，计算当前位 digit = (x / exp) % 10，扔进 buckets[digit]
 *    - 收集：从 0 号桶扫到 9 号桶，把数依次倒回原数组
 * 3. 若之前平移过负数，全员还原原始数值
 * 
 * @param arr 待排序的动态数组引用
 */
void RadixSortWithBuckets(std::vector<int>& arr) {
    int maxv = *max_element(arr.begin(), arr.end());
    int minv = *min_element(arr.begin(), arr.end());
    // 如果有负数就把全体往右偏移这么多单位使得全体都大于0，最后再减回来就行
    if(minv < 0) {
      for(auto &x : arr) {
        x -= minv;
      }
      maxv -= minv;
    }

    for(int exp = 1; maxv / exp > 0; exp *= 10) {
      std::vector<int> buckets[10];
      // 把数依次从后往前按位加入到桶中
      for(auto x : arr) {
        int digit = (x / exp) % 10;
        buckets[digit].emplace_back(x);
      }
      // 再把元素依次从桶中取出，此时后面的位就是有序的
      int pos = 0;
      for(int i = 0; i < 10; i++) {
        for(auto x : buckets[i]) {
          arr[pos++] = x;
        }
      }
    }

    // 如果最小值是负数前面就发生了偏移要减回来
    if(minv < 0) {
      for(auto& x : arr) {
        x += minv;
      }
    }
}

// ============================================================================
//                       【自动化校验驱动与测试页面】
// ============================================================================

void PrintHeader(const std::string& title) {
    std::cout << COLOR_CYAN "======================================================================\n" COLOR_RESET;
    std::cout << COLOR_CYAN "  " << title << "\n" COLOR_RESET;
    std::cout << COLOR_CYAN "======================================================================\n\n" COLOR_RESET;
}

void AssertVector(const std::string& test_name, const std::vector<int>& actual, const std::vector<int>& expect) {
    bool passed = (actual == expect);

    std::cout << "▶ " << test_name << " | 状态: "
              << (passed ? COLOR_GREEN "[PASS]" COLOR_RESET : COLOR_RED "[FAIL]" COLOR_RESET) << "\n";

    std::cout << "  ├─ 期望序列: [ ";
    for (int x : expect) std::cout << x << " ";
    std::cout << "]\n";

    std::cout << "  └─ 实际输出: [ ";
    for (size_t i = 0; i < actual.size(); ++i) {
        if (i < expect.size() && actual[i] == expect[i]) {
            std::cout << actual[i] << " ";
        } else {
            std::cout << COLOR_RED << actual[i] << " " COLOR_RESET;
        }
    }
    std::cout << "]\n\n";
}

int main() {
    PrintHeader("现代 C++ 基数排序 (流派一：真实 10 桶模拟) 自动化测试");

    // 测试 1: 经典跨数量级正整数用例 (1位 ~ 3位数)
    std::vector<int> tc1 = {170, 45, 75, 90, 802, 24, 2, 66};
    std::vector<int> exp1 = {2, 24, 45, 66, 75, 90, 170, 802};
    RadixSortWithBuckets(tc1);
    AssertVector("测试 1: 跨数量级正整数用例", tc1, exp1);

    // 测试 2: 工业级高频考察——负数与零混合排序
    std::vector<int> tc2 = {-5, 10, -100, 0, 23, -50, 8};
    std::vector<int> exp2 = {-100, -50, -5, 0, 8, 10, 23};
    RadixSortWithBuckets(tc2);
    AssertVector("测试 2: 负数与零混合排序", tc2, exp2);

    // 测试 3: 包含重复值乱序
    std::vector<int> tc3 = {321, 12, 321, 5, 88, 12, 0};
    std::vector<int> exp3 = {0, 5, 12, 12, 88, 321, 321};
    RadixSortWithBuckets(tc3);
    AssertVector("测试 3: 包含重复值乱序", tc3, exp3);

    std::cout << COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET;
    return 0;
}