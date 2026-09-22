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

/**
 * 大根堆向下调整函数 (SiftDown)
 * 考研规范：有效数据存储在 arr[1 ... len]，左孩子 2*i，右孩子 2*i + 1
 * 
 * @param arr 数组首地址 (arr[0] 可作为暂存单元使用)
 * @param i   当前需要向下调整的子树根节点下标 (1 <= i <= len)
 * @param len 当前堆中有效元素的总个数
 */
void SiftDown(int arr[], int i, int len) {
    // arr[0]没有数据可以暂存
    // 暂存当前要调整的元素，等一切确定了再一次性放入应有的位置
    arr[0] = arr[i];
    for(int k = i * 2; k <= len; k *= 2) {
      // 找到左右最大的索引
      if(k < len && arr[k] < arr[k + 1]) {
        k++;
      }
      // 如果调整的元素已经比左右孩子都大了直接退出
      if(arr[0] >= arr[k]) break;

      // 把大的元素替代填入上面的坑
      arr[i] = arr[k];

      // 更新要调整的元素位置索引
      i = k;

    }
    arr[i] = arr[0];


    // if(i * 2 > len) return ;
    // if(i * 2 + 1 > len) {
    //   if(arr[i] > arr[i * 2]) return ;
    //   else {
    //     arr[0] = arr[i];
    //     arr[i] = arr[i * 2];
    //     arr[i * 2] = arr[0];
    //     SiftDown(arr, i * 2, len);
    //   }
    // } else {
    //   int k = i * 2;
    //   if(arr[i * 2] > arr[i * 2 + 1]) {
    //     k = i * 2;
    //   } else {
    //     k = i * 2 + 1;
    //   }
    //   if(arr[k] > arr[i]) {
    //     arr[0] = arr[k];
    //     arr[k] = arr[i];
    //     arr[i] = arr[0];
    //     SiftDown(arr, k, len);
    //   }
    // }
}

/**
 * 堆排序主体函数
 * 步骤：
 * 1. 建立初始大根堆：从最后一个非叶子节点 i = n / 2 倒序调整至 1
 * 2. 排序循环：将堆顶 arr[1] 与当前末尾 arr[i] 交换，随后调用 SiftDown(arr, 1, i - 1)
 * 
 * @param arr 待排序数组 (有效数据为 arr[1 ... n])
 * @param n   有效数据长度
 */
void HeapSort(int arr[], int n) {
    // 建堆，从中间结点开始，因为后面的都是叶子不需要调整
    // 从下而上依次把结点做向下调整
    for(int i = n / 2; i > 0; i--) {
      SiftDown(arr, i, n);
    }
    // 调整后的树每个结点的值都大于他的左右孩子
    for(int i = n; i > 1; i--) {
      // 交换堆顶元素和最后未排好序的元素
      arr[0] = arr[1];
      arr[1] = arr[i];
      arr[i] = arr[0];
      // 再把放入堆顶但未排好序的数做向下调整
      SiftDown(arr, 1, i - 1);
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

// 针对 408 规范从下标 1 开始打印与比对
void AssertArray1Based(const char *test_name, int actual[], int expect[], int n) {
    bool passed = true;
    for (int i = 1; i <= n; i++) {
        if (actual[i] != expect[i]) {
            passed = false;
            break;
        }
    }

    printf("▶ %-28s | 状态: %s\n", test_name, 
           passed ? COLOR_GREEN "[PASS]" COLOR_RESET : COLOR_RED "[FAIL]" COLOR_RESET);

    printf("  ├─ 期望序列 (1~%d): [ ", n);
    for (int i = 1; i <= n; i++) printf("%d ", expect[i]);
    printf("]\n");

    printf("  └─ 实际输出 (1~%d): [ ", n);
    for (int i = 1; i <= n; i++) {
        if (actual[i] == expect[i]) {
            printf("%d ", actual[i]);
        } else {
            printf(COLOR_RED "%d " COLOR_RESET, actual[i]);
        }
    }
    printf("]\n\n");
}

int main(void) {
    PrintHeader("408 考研标准 堆排序 (下标 1~n) 自动化测试");

    // 测试 1: 408 严蔚敏教材经典手推 8 元素序列 (下标 0 占位)
    int tc1[]  = {0, 53, 17, 78, 9, 45, 65, 87, 32};
    int exp1[] = {0, 9, 17, 32, 45, 53, 65, 78, 87};
    int n1 = 8;
    HeapSort(tc1, n1);
    AssertArray1Based("测试 1: 408 教材经典 8 元素案例", tc1, exp1, n1);

    // 测试 2: 包含多个重复元素与乱序 (10 个元素)
    int tc2[]  = {0, 4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    int exp2[] = {0, 1, 2, 3, 4, 7, 8, 9, 10, 14, 16};
    int n2 = 10;
    HeapSort(tc2, n2);
    AssertArray1Based("测试 2: 10 元素重复乱序用例", tc2, exp2, n2);

    // 测试 3: 逆序最坏情况建堆与调整
    int tc3[]  = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int exp3[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n3 = 9;
    HeapSort(tc3, n3);
    AssertArray1Based("测试 3: 完全逆序数组调整", tc3, exp3, n3);

    printf(COLOR_CYAN "======================== 测试流程执行完毕 ========================\n" COLOR_RESET);
    return 0;
}