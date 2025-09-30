

#### <img src="/Users/bananapig/Desktop/学习/交大高清标示--透明背景（校徽等）/交大高清标示--透明背景-03.png" alt="交大高清标示--透明背景-03" style="zoom:50%;" />

<h1 align = "center" style="font-size:40px">北京交通大学</h1>

<center style="font-size:18px">		课程名称：计算机网络原理</center>
<center style="font-size:18px">实验题目     ：Lab4_编程实现路由算法 (Dijkstra)</center>
<center style="font-size:18px">学号    ：22281188</center>
<center style="font-size:18px">姓名 ： 江家玮</center>
<center style="font-size:18px">班级 ： 计科2204班</center>
<center style="font-size:18px">指导老师 ： 张志飞老师</center>
<center style="font-size:18px">报告日期 ：2025-05-07</center>
------

[TOC]

> 22281188-江家玮-Lab4/
> |-- Lab4实验报告.pdf
> |-- Lab4实验报告.pdf
> |-- Lab4.py



<div style="page-break-after: always;"></div>

# Lab4 编程实现路由算法 (Dijkstra)

## 一、实验时间

实验时间：2025-05-05 08:00-09:50

## 二、实验地点

实验地点：北京交通⼤学九教北402外

## 三、实验目的

1.  运用各种编程语言（本实验选用 Python）实现基于 Dijkstra 算法的路由软件。
2.  通过本实验，能够对路由原理和 Dijkstra 路由算法有进一步的理解和掌握。
3.  学会根据学号生成特定网络拓扑的链路代价，并测试算法的正确性。

## 四、实验内容与过程

### 4.1 实验任务概述

本次实验的核心任务是编程实现 Dijkstra 算法，用于计算给定网络拓扑中从单一源点到所有其他节点的最短路径。要求根据个人学号确定网络中部分链路的代价，然后运行实现的算法，输出最短路径的代价以及具体的逐跳路径。

### 4.2 Dijkstra 算法原理

Dijkstra 算法是一种经典的单源最短路径算法，用于计算带权有向图或无向图中一个节点到其他所有节点的最短路径。基本思想就是：

* **初始化**：
    
    * `c(i,j)`: 节点 `i` 至节点 `j` 之间链路的代价，若 `i,j` 不直接相连，则为无穷大。
    * `D(v)`: 当前从源结点至目的结点 `V` 之间路由的代价。
    * `p(v)`: 从源结点至目的结点 `V` 之间路由中 `V` 之前的结点。
    * `N`: 已经知道最优路径的结点集合。
    
    ------
    
    1.  `N = {A}` (A为源节点)
    2.  `for all nodes v`
    3.  `  if v adjacent to A`
    4.  `    then D(v) = c(A,v)`
    5.  `    else D(v) = ∞`
    
* **循环 (Loop)**：
    
    1.  `find w not in N such that D(w) is a minimum` (找到一个不在N中且D(w)最小的节点w)
    2.  `add w to N` (将w加入N)
    3.  `update D(v) for all v adjacent to w and not in N:` (更新所有与w相邻且不在N中的节点v的D(v))
        `D(v) = min( D(v), D(w) + c(w,v) )`
        *(新代价是旧代价与通过w到v的代价中的较小者)*
    4.  `until all nodes in N` (直到所有节点都在N中)

==通过不断选择当前已知路径最短的节点加入集合 $$N$$，并更新其邻居节点的路径信息，逐步构建出从源点到所有节点的最短路径树。==

### 4.3 网络拓扑与链路代价生成

#### 4.3.1 实验指定拓扑图

实验使用下图所示的网络拓扑结构：

<img src="/Users/bananapig/Library/Application Support/typora-user-images/image-20250507200510843.png" alt="image-20250507200510843" style="zoom:50%;" />

上图显示了节点 u, v, w, x, y, z 及其连接关系。部分链路的代价 `X_i` 需要根据学号生成（我的学号为：`22281188`)。

#### 4.3.2 根据学号生成链路代价

链路代价 `X_i` 的生成规则为：`X_i = (学号对应位 == 0) ? 10 : 学号对应位`。
我的学号是 **22281188**。

根据此规则，那么我生成的链路代价如下：
* `X_1` (对应 u-v): 学号第1位是 `2`  => $$X_1 = 2$$
* `X_2` (对应 v-x): 学号第2位是 `2`  => $$X_2 = 2$$
* `X_3` (对应 v-w): 学号第3位是 `2`  => $$X_3 = 2$$
* `X_4` (对应 x-w): 学号第4位是 `8`  => $$X_4 = 8$$
* `X_5` (对应 x-y): 学号第5位是 `1`  => $$X_5 = 1$$
* `X_6` (对应 w-y): 学号第6位是 `1`  => $$X_6 = 1$$
* `X_7` (对应 w-z): 学号第7位是 `8`  => $$X_7 = 8$$
* `X_8` (对应 y-z): 学号第8位是 `8`  => $$X_8 = 8$$

固定代价：
* 链路 (u, x): 2
* 链路 (u, w): 3 
* <img src="/Users/bananapig/Library/Application Support/typora-user-images/image-20250507200746693.png" alt="image-20250507200746693" style="zoom:30%;" />

#### 4.3.3 最终网络邻接表及代价

综合以上信息，得到本实验使用的网络图的邻接表示（无向图，双向代价一致）：

```python
graph_data = {
'u': {'v': 2, 'x': 2, 'w': 3},
'v': {'u': 2, 'x': 2, 'w': 2},
'w': {'u': 3, 'v': 2, 'x': 8, 'y': 1, 'z': 8},
'x': {'u': 2, 'v': 2, 'w': 8, 'y': 1},
'y': {'x': 1, 'w': 1, 'z': 8},
'z': {'w': 8, 'y': 8}
}
```

### 4.4 Python 实现 Dijkstra 算法

#### 4.4.1 开发环境

* **操作系统:** macOS
* **编程语言:** Python 3.10
* **主要库:** `heapq` (用于实现优先队列，高效选取最小代价节点)

#### 4.4.2 核心代码 (`dijkstra_routing.py`)

```python
import heapq # 用于实现优先队列，高效找到最小代价的节点

def dijkstra(graph, start_node):
    """
    实现 Dijkstra 算法

    参数:
        graph (dict): 图的邻接列表表示。
                      格式: {'node1': {'neighbor1': cost1, 'neighbor2': cost2}, ...}
        start_node (str): 源节点名称

    返回:
        distances (dict): 从源节点到所有其他节点的最短路径代价
                          格式: {'node': cost}
        previous_nodes (dict): 在最短路径中，每个节点的前一个节点
                               格式: {'node': 'previous_node'}
    """
    # 1. 初始化
    distances = {node: float('inf') for node in graph} # D(v)
    previous_nodes = {node: None for node in graph}   # p(v)
    distances[start_node] = 0
    # N 集合通过已访问或已从优先队列中永久取出的节点来隐式表示

    # 使用优先队列存储 (cost, node)，以便快速找到 D(w) 最小的 w
    priority_queue = [(0, start_node)] # (D(v), v)

    while priority_queue:
        # 9. find w not in N such that D(w) is a minimum
        current_distance, current_node = heapq.heappop(priority_queue)

        # 如果这条路径的代价大于已记录的到当前节点的最小代价，则跳过
        if current_distance > distances[current_node]:
            continue

        # 10. add w to N (隐式)

        # 11. update D(v) for all v adjacent to w and not in N:
        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight
            # 12. D(v) = min( D(v), D(w) + c(w,v) )
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                previous_nodes[neighbor] = current_node
                heapq.heappush(priority_queue, (distance, neighbor))

    return distances, previous_nodes

def get_path(previous_nodes, start_node, end_node):
    """
    根据 previous_nodes字典回溯路径
    """
    path = []
    current_node_in_path = end_node 
    while current_node_in_path is not None and current_node_in_path != start_node:
        path.append(current_node_in_path)
        current_node_in_path = previous_nodes[current_node_in_path]
    if current_node_in_path == start_node: # 确保路径可达
        path.append(start_node)
        return "".join(path[::-1]) # 反转路径并连接成字符串
    return None # 如果无法到达目标节点

# ---- 主程序 ----
if __name__ == "__main__":
    # 根据学号 22281188 和拓扑生成的链路代价
    # X1=2, X2=2, X3=2, X4=8, X5=1, X6=1, X7=8, X8=8
    # 固定链路: u-x:2, u-w:3
    graph_data = {
        'u': {'v': 2, 'x': 2, 'w': 3},
        'v': {'u': 2, 'x': 2, 'w': 2},
        'w': {'u': 3, 'v': 2, 'x': 8, 'y': 1, 'z': 8},
        'x': {'u': 2, 'v': 2, 'w': 8, 'y': 1},
        'y': {'x': 1, 'w': 1, 'z': 8},
        'z': {'w': 8, 'y': 8}
    }

    # 选择一个源节点，'u'
    source_node_u = 'u'
    all_distances_u, all_previous_nodes_u = dijkstra(graph_data, source_node_u)

    print(f"学号: 22281188, 姓名: 江家玮")
    print(f"-------------------------------------------")
    print(f"源节点: {source_node_u}")
    print(f"-------------------------------------------")
    # print(f"从源节点 {source_node_u} 出发的最短路径代价：")
    # for node, dist in all_distances_u.items():
    #     print(f"到节点 {node}: {dist}")

    print(f"\n逐跳路径 = 路径代价 (从 {source_node_u} 出发):")
    # 输出格式类似 "ux=2"
    sorted_nodes = sorted(graph_data.keys()) # 按节点名称排序输出
    for destination_node in sorted_nodes:
        if destination_node != source_node_u:
            path_str = get_path(all_previous_nodes_u, source_node_u, destination_node)
            if path_str:
                print(f"{path_str}={all_distances_u[destination_node]}")
            else:
                print(f"从 {source_node_u} 无法到达 {destination_node}")
    print(f"-------------------------------------------\n")


    # 可以更改 source_node 并重新运行以查看从不同源节点开始的路径
    # liru: 计算从 'x' 出发到所有其他节点的路径
    source_node_x = 'x'
    all_distances_x, all_previous_nodes_x = dijkstra(graph_data, source_node_x)

    print(f"源节点: {source_node_x}")
    print(f"-------------------------------------------")
    # print(f"从源节点 {source_node_x} 出发的最短路径代价：")
    # for node, dist in all_distances_x.items():
    #     print(f"到节点 {node}: {dist}")

    print(f"\n逐跳路径 = 路径代价 (从 {source_node_x} 出发):")
    for destination_node in sorted_nodes:
        if destination_node != source_node_x:
            path_str = get_path(all_previous_nodes_x, source_node_x, destination_node)
            if path_str:
                print(f"{path_str}={all_distances_x[destination_node]}")
            else:
                print(f"从 {source_node_x} 无法到达 {destination_node}")
    print(f"-------------------------------------------")
```

#### 4.4.3 代码逻辑说明

1. `dijkstra(graph, start_node)` 函数:
   - 使用 `distances` 字典存储从 `start_node` 到各节点的最短代价估计，初始化为无穷大，源点为0。
   - 使用 `previous_nodes` 字典存储最短路径上前一个节点，用于后续路径回溯。
   - `priority_queue` (最小堆) 存储 `(cost, node)` 元组，使得每次能高效取出当前代价最小的未访问节点。
   - 主循环不断从优先队列中取出节点 `current_node`，遍历其所有邻居 `neighbor`。
   - 如果通过 `current_node` 到达 `neighbor` 的路径代价 (`current_distance + weight`) 小于之前记录的到 `neighbor` 的代价，则更新 `distances[neighbor]` 和 `previous_nodes[neighbor]`，并将新的 `(cost, neighbor)` 推入优先队列。
2. `get_path(previous_nodes, start_node, end_node)` 函数:
   - 从 `end_node` 开始，通过 `previous_nodes` 字典反向回溯，直到 `start_node`，构建出完整路径。
   - 路径列表反转后连接成字符串。
3. 主程序块 (`if __name__ == "__main__":`):
   - 定义 `graph_data`，即根据我的学号（22281188）生成的网络拓扑及代价。
   - 例如，我分别以 'u' 和 'x' 为源节点调用 `dijkstra` 函数。
   - 按照实验要求的格式（例如 `ux=2`）打印从源节点到其他所有目标节点的逐跳路径及总代价。输出结果按目标节点名称排序，以便观察。

### 4.5 测试与验证

实验步骤要求：

1. 选择合适的编程语言实现基于 Dijkstra 算法的路由软件。（使用 Python）
2. 根据学号生成不同的链路代价，测试和验证自己的路由软件。（见 4.3 和 4.4）

程序运行时，会输出以 'u' 为源节点和以 'x' 为源节点到图中所有其他节点的最短路径及其代价。输出格式遵循 "逐跳路径=路径代价" 的形式。

## 五、实验结果

运行上述 Python 代码，得到如下输出结果：

<img src="/Users/bananapig/Library/Application Support/typora-user-images/image-20250507201257274.png" alt="image-20250507201257274" style="zoom:50%;" />

- **u -> v**: 直接连接，`uv = 2`。
- **u -> x**: 直接连接，`ux = 2`。
- **u -> w:**
  - u-w (直接): 3
  - u-v-w: $$D(u,v) + c(v,w) = 2 + 2 = 4$$
  - u-x-w: $$D(u,x) + c(x,w) = 2 + 8 = 10$$
  - 最短的是 `uw=3`
- **u -> y:**
  - u-w-y: $$D(u,w) + c(w,y) = 3 + 1 = 4 \text{ } (uw=3)$$
  - u-v-w-y: $$D(u,v) + c(v,w) + c(w,y) = 2 + 2 + 1 = 5$$
  - u-x-y: $$D(u,x) + c(x,y) = 2 + 1 = 3$$
  - 最短的是 `uxy=3`。 
- **u -> z:**
  - u-w-z: $D(u,w) + c(w,z) = 3 + 8 = 11$
  - u-y-z: $D(u,y) + c(y,z) = 3 + 8 = 11$
  - 其他路径如 `uvwz` ($2+2+8=12$)
  - ······
  - 最短的是，可能的路径有 `uwz=11` & `uxyz=11`。

## 六、实验心得

通过本次基于 Dijkstra 算法的路由软件编程实验，我将课程中学习到的路由算法理论知识有效地应用于实践中。

首先，我对 Dijkstra 算法的工作原理有了更为深刻和直观的认识。在实验之前，我对算法的理解主要停留在伪代码和概念层面。通过亲手编写代码，我深入理解了算法的每一步，包括如何初始化距离、如何使用优先队列（`heapq`）高效地选择当前路径最短的节点，以及如何松弛边（更新相邻节点的距离）。特别是 `D(v) = min( D(v), D(w) + c(w,v) )` 这一核心更新步骤，在编码过程中得到了反复应用和体会。

其次，实验让我掌握了如何将抽象的图结构用编程语言中的数据结构（如字典表示的邻接表）进行表示，并在此基础上实现复杂的图算法。根据学号动态生成链路代价的环节，也让我体会到了实际网络配置中参数化和定制化的需求。

在代码实现过程中，我注意到了一些细节：

1. **路径回溯**：Dijkstra 算法本身只计算最短路径的长度，为了得到具体的路径，需要额外的数据结构（如 `previous_nodes` 字典）来记录路径信息，并通过回溯来重构路径。
2. **优先队列的重要性**：使用最小优先队列可以显著提高 Dijkstra 算法的效率，从 $O(V^2)$（朴素实现）优化到 $O(E \log V)$ （使用二叉堆）。Python 的 `heapq` 模块提供了基于二叉堆的实现。

调试过程中，我遇到的主要挑战是确保路径回溯的逻辑正确无误，以及输出格式符合实验要求。通过分步测试和打印中间状态（例如各个节点的 `distances` 和 `previous_nodes`），我逐步定位并解决了问题。例如，一开始我的 `get_path` 函数在处理无法到达的节点时或者回溯起点时存在一些边界条件问题，通过仔细分析得以修正。