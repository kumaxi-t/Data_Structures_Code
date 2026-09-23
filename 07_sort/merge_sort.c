#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_YELLOW  "\033[1;33m"

// ============================================================================
//                          【408 核心算法手撕区】
// ============================================================================

// 全局/辅助数组指针，供 Merge 合并时暂存数据使用
int *B = NULL;

void Merge(int arr[], int low, int mid, int high) {
  int i, j, k;
  // 把要合并的两个有序队列连在一起
  for(int p = low; p <= high; p++) {
    B[p] = arr[p];
  }
  // 1 3 5 7 2 4 6 8
  // low    mid   high
  // 依次比较合并队列
  for(i = low, j = mid + 1, k = low; i <= mid && j <= high; k++) {
    if(B[i] <= B[j]) {
      arr[k] = B[i++];
    } else {
      arr[k] = B[j++];
    }
  }
  // 把剩余的直接加在末尾
  while(i <= mid) arr[k++] = B[i++];
  while(j <= high) arr[k++] = B[j++];
}

void MergeSort(int arr[], int low, int high) {
  if(low >= high) return ;
  int mid = low + (high - low) / 2;
  // 处理左边和右边使其有序
  MergeSort(arr, low, mid);
  MergeSort(arr, mid + 1, high);
  // 左边右边都有序了再把两个有序的队列合并
  Merge(arr, low, mid, high);

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
    PrintHeader("408 数据结构 2路归并排序 (MergeSort) 自动化测试");

    // 测试 1: 408 经典无序真题序列
    int tc1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int exp1[] = {13, 27, 38, 49, 49, 65, 76, 97};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    B = (int *)malloc(sizeof(int) * n1);
    MergeSort(tc1, 0, n1 - 1);
    AssertArray("测试 1: 408 经典真题序列", tc1, exp1, n1);
    free(B);

    // 测试 2: 逆序最坏情况测试
    int tc2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int exp2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    B = (int *)malloc(sizeof(int) * n2);
    MergeSort(tc2, 0, n2 - 1);
    AssertArray("测试 2: 逆序数组测试", tc2, exp2, n2);
    free(B);

    // 测试 3: 双元素与多重复元素测试
    int tc3[] = {3, 1, 3, 2, 3, 0, 3};
    int exp3[] = {0, 1, 2, 3, 3, 3, 3};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    B = (int *)malloc(sizeof(int) * n3);
    MergeSort(tc3, 0, n3 - 1);
    AssertArray("测试 3: 大量重复元素测试", tc3, exp3, n3);
    free(B);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}