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

void BinaryInsertSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
      if(arr[i - 1] > arr[i]) {
        int tmp = arr[i];
        int left = 0;
        int right = i - 1;
        // 找到这一个比他小的元素，找到时位置为mid，最终left=mid+1也即要插入的位置
        while(left <= right) {
          int mid = left + (right - left) / 2;
          if(arr[mid] > tmp) {
            right = mid - 1;
          }else {
            left = mid + 1;
          }
        }
        // 把后面的元素顺移一位
        for(int j = i - 1; j >= left; j--) {
          arr[j + 1] = arr[j];
        }
        // 插入到比他小的元素后面
        arr[left] = tmp;
      }
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
    PrintHeader("408 数据结构 折半插入排序 (BinaryInsertSort) 自动化测试");

    // 测试 1: 重复元素与一般无序数组
    int tc1[] = {73, 22, 91, 55, 22, 44, 88};
    int exp1[] = {22, 22, 44, 55, 73, 88, 91};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    BinaryInsertSort(tc1, n1);
    AssertArray("测试 1: 包含重复元素", tc1, exp1, n1);

    // 测试 2: 负数与零混合测试
    int tc2[] = {-3, 0, -8, 12, 5, -1};
    int exp2[] = {-8, -3, -1, 0, 5, 12};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    BinaryInsertSort(tc2, n2);
    AssertArray("测试 2: 包含负数与零", tc2, exp2, n2);

    // 测试 3: 双元素边界
    int tc3[] = {9, 2};
    int exp3[] = {2, 9};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    BinaryInsertSort(tc3, n3);
    AssertArray("测试 3: 双元素反序", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}