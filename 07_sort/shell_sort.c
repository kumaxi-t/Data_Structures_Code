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

void ShellSort(int arr[], int n) {
    // 选取分割长度把原数组划分为div组
    for(int div = n / 2; div >= 1; div /= 2) {
      // 再对分割后的数组进行插入排序
      for(int j = div; j < n; j += div) {
        if(arr[j - div] > arr[j]) {
          int tmp = arr[j];
          int k = j - div;
          while(k >= 0 && arr[k] > tmp) {
            arr[k + div] = arr[k];
            k -= div;
          }
          arr[k + div] = tmp;
        }
      }
    }
    // for(int div = n / 2; div >= 1; div /= 2) {
    //   // 没必要再加一层i
    //   for(int i = 0; i < div; i++) {
    //     for(int j = i + div; j < n; j += div) {
    //       if(arr[j - div] > arr[j]) {
    //         int tmp = arr[j];
    //         int k = j - div;
    //         while(k >= 0 && arr[k] > tmp) {
    //           arr[k + div] = arr[k];
    //           k -= div;
    //         }
    //         arr[k + div] = tmp;
    //       }
    //     }
    //   }
    // }
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
    PrintHeader("408 数据结构 希尔排序 (ShellSort) 自动化测试");

    // 测试 1: 408 经典手推 10 元素序列
    int tc1[] = {50, 26, 38, 80, 70, 90, 8, 30, 40, 20};
    int exp1[] = {8, 20, 26, 30, 38, 40, 50, 70, 80, 90};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    ShellSort(tc1, n1);
    AssertArray("测试 1: 经典 10 元素用例", tc1, exp1, n1);

    // 测试 2: 逆序跨步长测试
    int tc2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int exp2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    ShellSort(tc2, n2);
    AssertArray("测试 2: 逆序数组测试", tc2, exp2, n2);

    // 测试 3: 重复且乱序
    int tc3[] = {3, 5, 2, 3, 8, 5, 1};
    int exp3[] = {1, 2, 3, 3, 5, 5, 8};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    ShellSort(tc3, n3);
    AssertArray("测试 3: 包含重复值乱序", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}