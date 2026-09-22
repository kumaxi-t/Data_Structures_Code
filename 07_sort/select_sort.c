#include <stdio.h>
#include <stdbool.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_YELLOW  "\033[1;33m"

// ============================================================================
//                          【408 核心算法手撕区】
// ============================================================================

// 从第一个元素开始往后找最小的数放在该位置，后续位置往后依此类推
void SelectSort(int arr[], int n) {
  for(int i = 0; i < n; i++) {
    int k = i;
    for(int j = i + 1; j < n; j++) {
      if(arr[j] < arr[k]) {
        k = j;
      }
    }
    int tmp = arr[k];
    arr[k] = arr[i];
    arr[i] = tmp;
  }
}

// ============================================================================
//                       【自动化校验驱动与测试页面】
// ============================================================================

void PrintHeader(const char *title) {
    printf(COLOR_CYAN "======================================================================\n" COLOR_RESET);
    printf(COLOR_CYAN "  %s\n" COLOR_RESET, title);
    printf(COLOR_CYAN "======================================================================\n\n" COLOR_RESET);
}

void AssertArray(const char *test_name, int actual[], int expect[], int n) {
    bool passed = true;
    for (int i = 0; i < n; i++) {
        if (actual[i] != expect[i]) {
            passed = false;
            break;
        }
    }

    printf("▶ %-26s | 状态: %s\n", test_name, 
           passed ? COLOR_GREEN "[PASS]" COLOR_RESET : COLOR_RED "[FAIL]" COLOR_RESET);

    printf("  ├─ 期望序列: [ ");
    for (int i = 0; i < n; i++) printf("%d ", expect[i]);
    printf("]\n");

    printf("  └─ 实际输出: [ ");
    for (int i = 0; i < n; i++) {
        if (actual[i] == expect[i]) {
            printf("%d ", actual[i]);
        } else {
            printf(COLOR_RED "%d " COLOR_RESET, actual[i]);
        }
    }
    printf("]\n\n");
}

int main(void) {
    PrintHeader("408 数据结构 简单选择排序 (SelectSort) 自动化测试");

    // 测试 1: 408 经典无序序列
    int tc1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int exp1[] = {13, 27, 38, 49, 49, 65, 76, 97};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    SelectSort(tc1, n1);
    AssertArray("测试 1: 408 经典无序序列", tc1, exp1, n1);

    // 测试 2: 逆序测试
    int tc2[] = {5, 4, 3, 2, 1};
    int exp2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    SelectSort(tc2, n2);
    AssertArray("测试 2: 逆序最坏情况", tc2, exp2, n2);

    // 测试 3: 双元素边界
    int tc3[] = {8, 3};
    int exp3[] = {3, 8};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    SelectSort(tc3, n3);
    AssertArray("测试 3: 双元素反序边界", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}