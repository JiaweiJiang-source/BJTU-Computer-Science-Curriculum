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