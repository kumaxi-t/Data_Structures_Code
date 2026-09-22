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

// 两两交换前后元素把大的元素放到后面，每次确定待排序队列中最大的元素放在后面
// 如果一趟下来发现没有交换元素说明数组已经有序直接退出
void BubbleSort(int arr[], int n) {
  for(int i = 0; i < n; i++) {
    bool swapped = false;
    for(int j = 0; j < n - i -1; j++) {
      if(arr[j + 1] < arr[j]) {
        int tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
        swapped = true;
      }
    }
    if(!swapped) break;
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
    PrintHeader("408 数据结构 冒泡排序 (BubbleSort) 自动化测试");

    // 测试 1: 普通无序测试
    int tc1[] = {64, 34, 25, 12, 22, 11, 90};
    int exp1[] = {11, 12, 22, 25, 34, 64, 90};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    BubbleSort(tc1, n1);
    AssertArray("测试 1: 普通无序序列", tc1, exp1, n1);

    // 测试 2: 完全升序 (检验 swapped 优化提前退出)
    int tc2[] = {1, 2, 3, 4, 5, 6};
    int exp2[] = {1, 2, 3, 4, 5, 6};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    BubbleSort(tc2, n2);
    AssertArray("测试 2: 已有序最好情况", tc2, exp2, n2);

    // 测试 3: 逆序最坏情况
    int tc3[] = {10, 9, 8, 7, 6};
    int exp3[] = {6, 7, 8, 9, 10};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    BubbleSort(tc3, n3);
    AssertArray("测试 3: 完全逆序最坏情况", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}