#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

// ============================================================================
//                          【关卡 7：并查集 408 考场标准手撕区】
// ============================================================================

// 1. 初始化并查集（408 统编规范：负数表示根，绝对值代表结点规模）
// 目标：将 parent 数组前 n 个元素全部初始化为 -1
void Initial(int parent[], int n) {
    // 请在此编写你的代码：
    if(n <= 0) return ;
    for(int i = 0; i < n; i++) {
      parent[i] = -1;
    }
}

// 2. 查操作（必须带路径压缩，408 满分核心）
// 目标：找到 x 所在集合的根结点编号；在回溯过程中，将沿途结点的 parent 直接改为根
int Find(int parent[], int x) {
    // 请在此编写你的代码：
    if(x < 0) return -1;
    // 该节点的父节点是否为负数（根节点-绝对值为结点总数）
    // 如果是就返回该结点x
    // 否则找到该节点父节点的父节点再做判断，顺便路径压缩
    return parent[x] < 0 ? x : (parent[x] = Find(parent, parent[x]));
}

// 3. 并操作（带小树合并到大树的按秩/按规模优化）
// 目标：
//   1) 找到 a 和 b 的根结点 rootA 与 rootB；
//   2) 若已在同一集合，返回 false；
//   3) 比较两集合规模（注意：负数越小，绝对值越大，规模越大）；
//   4) 将小树合并到大树，更新大树根的规模计数，小树根指向大树根，返回 true
bool Union(int parent[], int a, int b) {
    // 请在此编写你的代码：
    if(a < 0 || b < 0 || a == b) return false;
    int rootA = Find(parent, a);
    int rootB = Find(parent, b);
    if(rootA == rootB) return false;
    if(abs(parent[rootA]) >= abs(parent[rootB])) {
      parent[rootA] += parent[rootB];
      parent[rootB] = rootA;
    }else {
      parent[rootB] += parent[rootA];
      parent[rootA] = rootB;
    }
    return true;
}

// 4. 统计图中连通分量的数量（408 常见真题设问原型）
// 目标：遍历 0 到 n-1，统计根结点的总个数
int CountComponents(int parent[], int n) {
    // 请在此编写你的代码：
    if(n < 0) return 0;
    int sum = 0;
    for(int i = 0; i < n; i++) {
      if(parent[i] < 0) sum++;
    }
    return sum;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 核心算法【关卡7：并查集（考场标准版）】自测开始 =============\n\n");

    int n = 8; // 8 个元素：0 到 7
    int parent[MAX_SIZE];

    // 1. 测试初始化
    Initial(parent, n);
    bool init_ok = true;
    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) init_ok = false;
    }
    printf("测试 1 [Initial 全部置为 -1]: %s\n", init_ok ? "PASS" : "FAIL");

    // 2. 测试基本合并与连通分量计数
    // 分量 1: {0, 1, 2, 3}
    Union(parent, 0, 1);
    Union(parent, 2, 3);
    Union(parent, 1, 2);

    // 分量 2: {4, 5}
    Union(parent, 4, 5);

    // 此时集合分布：{0, 1, 2, 3}, {4, 5}, {6}, {7} -> 共 4 个连通分量
    int comps = CountComponents(parent, n);
    printf("测试 2 [连通分量数量统计]: %s (期望: 4, 实际: %d)\n", 
           comps == 4 ? "PASS" : "FAIL", comps);

    // 3. 测试连通性判定
    bool conn_0_3 = (Find(parent, 0) == Find(parent, 3));
    bool conn_0_4 = (Find(parent, 0) == Find(parent, 4));
    bool find_ok = (conn_0_3 == true && conn_0_4 == false);
    printf("测试 3 [连通性正确性判断]: %s\n", find_ok ? "PASS" : "FAIL");

    // 4. 测试小树合并到大树与规模累加
    // 分量 1 规模为 4，分量 2 规模为 2，合并后大树根应保持为分量 1 的根，规模为 -6
    int root1 = Find(parent, 0);
    int root2 = Find(parent, 4);
    Union(parent, 0, 4);
    int merged_root = Find(parent, 0);
    bool size_ok = (parent[merged_root] == -6 && merged_root == root1);
    printf("测试 4 [小树并入大树与规模累加]: %s (合并后新根规模: %d)\n", 
           size_ok ? "PASS" : "FAIL", parent[merged_root]);

    // 5. 测试路径压缩树高拍平效果
    // 强制触发一次查找后，非根结点的 parent 应当直接指向 merged_root
    Find(parent, 3);
    Find(parent, 5);
    bool compress_ok = (parent[3] == merged_root && parent[5] == merged_root);
    printf("测试 5 [路径压缩树高拍平]: %s\n", compress_ok ? "PASS" : "FAIL");

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}