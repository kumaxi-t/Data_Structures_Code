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


// 从前往后依次选取元素塞到前面有序的队列当中去
void InsertSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
      // 如果前面的元素比当前元素大说明要在前面找位置插进去，否则不用动因为已经有序了
      if(arr[i - 1] > arr[i]) {
        int tmp = arr[i];
        int j = i - 1;
        while(j >= 0) {
          // 如果比要插入的元素要大就往后挪（覆盖掉后面的元素）
          // 否则就找到了比他小的元素的位置，插入到该元素后面
          if(arr[j] > tmp) {
            arr[j + 1] = arr[j];
            j--;
          }else break;
        }
        arr[j + 1] = tmp;
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
    PrintHeader("408 数据结构 直接插入排序 (InsertSort) 自动化测试");

    // 测试 1: 408 经典无序序列
    int tc1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int exp1[] = {13, 27, 38, 49, 49, 65, 76, 97};
    int n1 = sizeof(tc1) / sizeof(tc1[0]);
    InsertSort(tc1, n1);
    AssertArray("测试 1: 408 经典真题序列", tc1, exp1, n1);

    // 测试 2: 逆序最坏情况 (移动与比较次数最多)
    int tc2[] = {5, 4, 3, 2, 1};
    int exp2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(tc2) / sizeof(tc2[0]);
    InsertSort(tc2, n2);
    AssertArray("测试 2: 逆序最坏情况", tc2, exp2, n2);

    // 测试 3: 已有序最好情况
    int tc3[] = {10, 20, 30, 40, 50};
    int exp3[] = {10, 20, 30, 40, 50};
    int n3 = sizeof(tc3) / sizeof(tc3[0]);
    InsertSort(tc3, n3);
    AssertArray("测试 3: 完全有序最好情况", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}