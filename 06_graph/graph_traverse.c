#include <stdio.h>
#include <stdbool.h>

#define MAX_VERTEX_NUM 100

// 1. 408 考卷标准邻接矩阵结构体
typedef struct {
    int vexnum;                              // 顶点数
    int arcnum;                              // 边数
    int Edge[MAX_VERTEX_NUM][MAX_VERTEX_NUM];// 邻接矩阵（0/1 表示连通性）
} MGraph;

// 全局访问标记数组
bool visited[MAX_VERTEX_NUM];

// 2. 408 考场标准顺序循环队列（考场不提供 STL，必须手写简易队列）
typedef struct {
    int data[MAX_VERTEX_NUM];
    int front, rear;
} SqQueue;

void InitQueue(SqQueue *Q) {
    Q->front = Q->rear = 0;
}

bool IsEmpty(SqQueue *Q) {
    return Q->front == Q->rear;
}

void EnQueue(SqQueue *Q, int x) {
    Q->data[Q->rear++] = x;
}

int DeQueue(SqQueue *Q) {
    return Q->data[Q->front++];
}

// ============================================================================
//                          【408 考场核心手撕区】
// ============================================================================

// 1. 深度优先搜索（DFS）
// 参数：
//   - G: 邻接矩阵图
//   - v: 当前访问的顶点下标
//   - order: 记录访问顶点的先后顺序数组（考场通常为 printf 输出）
//   - count: 当前已访问的顶点计数指针
void DFS(MGraph G, int v, int order[], int *count) {
    // 请在此编写你的代码：
    visited[v] = true;
    order[(*count)++] = v;
    for(int i = 0; i < G.vexnum; i++) {
      if(G.Edge[v][i] == 1 && !visited[i]) {
        DFS(G, i, order, count);
      }
    }
}

// 2. 广度优先搜索（BFS）
// 参数：
//   - G: 邻接矩阵图
//   - start: 起始顶点下标
//   - order: 记录访问顶点的先后顺序数组
//   - count: 当前已访问的顶点计数指针
// 核心考点：标记 visited[w] = true 必须在入队时立即执行！
void BFS(MGraph G, int start, int order[], int *count) {
    // 请在此编写你的代码：
    SqQueue q;
    InitQueue(&q);
    EnQueue(&q, start);
    visited[start] = true;
    while(!IsEmpty(&q)) {
      int cur = DeQueue(&q);
      order[(*count)++] = cur;
      for(int i = 0; i < G.vexnum; i++) {
        if(G.Edge[cur][i] == 1 && !visited[i]) {
          EnQueue(&q, i);
          visited[i] = true;
        }
      }
    }
}

// 3. 计算无向图的连通分量个数（408 经典大题）
// 核心考点：外层循环遍历所有顶点，结合 visited 数组防漏扫独立连通块
int CountComponents(MGraph G) {
    int comp_count = 0;
    // 请在此编写你的代码：
    int order[MAX_VERTEX_NUM];
    int count = 0;
    for(int i = 0; i < G.vexnum; i++) {
      visited[i] = false;
    }
    for(int i = 0; i < G.vexnum; i++) {
      if(!visited[i]) {
        BFS(G, i, order, &count);
        comp_count++;
      }
    }
    return comp_count;
}

// ============================================================================
//                          【自动化校验驱动区（无需修改）】
// ============================================================================

int main() {
    printf("============= 考研 408 标准【图遍历与连通分量】自测开始 =============\n\n");

    // 构造测试图（包含 2 个独立连通块，共 6 个顶点 0 ~ 5）：
    // 连通块 1: (0)-(1), (0)-(2), (1)-(3), (2)-(3)
    // 连通块 2: (4)-(5)
    MGraph G;
    G.vexnum = 6;
    G.arcnum = 5;

    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.Edge[i][j] = 0;
        }
    }

    // 连通块 1 边
    G.Edge[0][1] = G.Edge[1][0] = 1;
    G.Edge[0][2] = G.Edge[2][0] = 1;
    G.Edge[1][3] = G.Edge[3][1] = 1;
    G.Edge[2][3] = G.Edge[3][2] = 1;

    // 连通块 2 边
    G.Edge[4][5] = G.Edge[5][4] = 1;

    // ----------------- 测试 1：DFS 深度优先遍历 -----------------
    for (int i = 0; i < G.vexnum; i++) visited[i] = false;
    int dfs_order[MAX_VERTEX_NUM];
    int dfs_count = 0;
    DFS(G, 0, dfs_order, &dfs_count);

    // 邻接矩阵升序扫描顺序预期：0 -> 1 -> 3 -> 2
    bool dfs_ok = (dfs_count == 4 && 
                   dfs_order[0] == 0 && dfs_order[1] == 1 && 
                   dfs_order[2] == 3 && dfs_order[3] == 2);
    printf("测试 1 [DFS 深度遍历顺序]: %s (序列: ", dfs_ok ? "PASS" : "FAIL");
    for (int i = 0; i < dfs_count; i++) printf("%d ", dfs_order[i]);
    printf(")\n");

    // ----------------- 测试 2：BFS 广度优先遍历 -----------------
    for (int i = 0; i < G.vexnum; i++) visited[i] = false;
    int bfs_order[MAX_VERTEX_NUM];
    int bfs_count = 0;
    BFS(G, 0, bfs_order, &bfs_count);

    // 邻接矩阵升序扫描顺序预期：0 -> 1 -> 2 -> 3
    bool bfs_ok = (bfs_count == 4 && 
                   bfs_order[0] == 0 && bfs_order[1] == 1 && 
                   bfs_order[2] == 2 && bfs_order[3] == 3);
    printf("测试 2 [BFS 广度遍历顺序]: %s (序列: ", bfs_ok ? "PASS" : "FAIL");
    for (int i = 0; i < bfs_count; i++) printf("%d ", bfs_order[i]);
    printf(")\n");

    // ----------------- 测试 3：连通分量计数 -----------------
    int comp = CountComponents(G);
    printf("测试 3 [全图连通分量统计]: %s (期望: 2, 实际: %d)\n", 
           comp == 2 ? "PASS" : "FAIL", comp);

    printf("\n============= 自测流程全部结束 =============\n");
    return 0;
}