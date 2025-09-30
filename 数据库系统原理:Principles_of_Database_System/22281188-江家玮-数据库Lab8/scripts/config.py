# scripts/config.py
# 数据库连接配置和测试参数

# --- 数据库配置 ---
DB_CONFIG = {
    'user': 'root',
    'password': 'Aa585891', 
    'host': 'localhost',
    'database': 'AirlineDB',
    'raise_on_warnings': True
}


# --- 插入器配置 ---
INSERTER_CONFIG = {
    'total_rows_to_insert': 1000000,  # 目标总插入行数 
    'batch_size': 1000,               # 每批次插入的行数
    'num_concurrent_inserters': 2,    # 模拟并发插入的线程/进程数 (如果用threading/multiprocessing)
                                      # 或者表示同时运行多少个inserter.py实例
    'insert_interval_ms': 10,         # 每个插入线程/进程内，每批次插入后的休眠时间（毫秒），用于控制频率
                                      # 100条/秒 大约是 1000ms / (100条/批 * N个并发) = X ms/批
                                      # 如果 batch_size=100, 1个并发, 100条/秒 -> 1000ms/批
                                      # 如果 batch_size=1000, 1个并发, 100条/秒 -> 10000ms/批
                                      # 如果 batch_size=100, 2个并发, 总共100条/秒 -> 每个并发50条/秒 -> 2000ms/批
    'log_file_insert_no_index': '../results/no_index_insert_log.csv',
    'log_file_insert_with_index': '../results/with_index_insert_log.csv',
}

# --- 查询器配置 ---
QUERIER_CONFIG = {
    'num_queries_to_run': 1000,       # 总共执行的查询次数 
    'query_interval_seconds': 1,      # 每秒执行一次查询
    'max_runtime_seconds': 60 * 30,   # 查询器最大运行时间（秒），例如30分钟
    'num_distinct_names_to_query': 100, # 从数据库中选取多少个不同的name进行查询
    'log_file_query_no_index': '../results/no_index_query_log.csv',
    'log_file_query_with_index': '../results/with_index_query_log.csv',
}

# --- 数据生成配置 ---
DATA_GENERATOR_CONFIG = {
    'base_passenger_id_start': 1, # 用于非自增主键时的起始ID
}
