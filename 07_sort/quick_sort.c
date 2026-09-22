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

int Partition(int arr[], int low, int high) {
  int pivot = arr[low];
  while(low < high) {
    // 找到比基准元素小的放左边的坑
    while(low < high && arr[high] >= pivot) high--;
    arr[low] = arr[high];
    // 找到比基准元素大的放右边的坑
    while(low < high && arr[low] <= pivot) low++;
    arr[high] = arr[low];
  }
  // 把基准元素填入他对应的位置，此时左边元素均比其小，右边元素均比其大
  arr[low] = pivot;
  // 返回确定好了的位置
  return low;
}

void QuickSort(int arr[], int low, int high) {
  if(low >= high) return ;
  // 找到确定好了的元素的位置
  int pos = Partition(arr, low, high);
  // 分治排序该元素左右边
  QuickSort(arr, low, pos - 1);
  QuickSort(arr, pos + 1, high);
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
    PrintHeader("408 数据结构 快速排序 (QuickSort) 自动化测试");

    // 测试 1: 408 经典真题序列
    int tc1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int exp1[] = {13, 27, 38, 49, 49, 65, 76, 97};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    QuickSort(tc1, 0, n1 - 1);
    AssertArray("测试 1: 408 经典真题序列", tc1, exp1, n1);

    // 测试 2: 大量重复元素 (检验相等元素跳出与边界处理)
    int tc2[] = {3, 1, 3, 2, 3, 0, 3};
    int exp2[] = {0, 1, 2, 3, 3, 3, 3};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    QuickSort(tc2, 0, n2 - 1);
    AssertArray("测试 2: 大量重复元素", tc2, exp2, n2);

    // 测试 3: 双元素逆序边界
    int tc3[] = {2, 1};
    int exp3[] = {1, 2};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    QuickSort(tc3, 0, n3 - 1);
    AssertArray("测试 3: 双元素逆序边界", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}