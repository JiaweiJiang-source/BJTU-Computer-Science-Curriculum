# scripts/querier.py
# 数据查询脚本，记录耗时

import mysql.connector
import time
import csv
import os
import random
from datetime import datetime

from config import DB_CONFIG, QUERIER_CONFIG

# 确保 results 目录存在
os.makedirs('../results', exist_ok=True)

def get_random_names_from_db(cursor, num_names):
    """从数据库中随机获取一些乘客姓名用于查询"""
    names = []
    try:
        # 为了效率，如果表很大，避免 OFFSET，可以取随机ID范围内的name
        # 或者，如果 PassengerID 是连续的，可以随机选ID
        # 这里用一个较简单的方法，可能在大表上效率不高，但用于测试还可以
        cursor.execute("SELECT name FROM Passenger ORDER BY RAND() LIMIT %s", (num_names,))
        results = cursor.fetchall()
        names = [row[0] for row in results]
    except mysql.connector.Error as err:
        print(f"Error fetching random names: {err}")
    
    if not names: # 如果获取不到，用一些默认值
        names = [f"TestName{i}" for i in range(1, num_names + 1)]
        print(f"Could not fetch names from DB, using default test names: {names[:3]}...")
    return names

def get_table_row_count(cursor, table_name="Passenger"):
    """获取当前表的总行数"""
    try:
        cursor.execute(f"SELECT COUNT(*) FROM {table_name}")
        return cursor.fetchone()[0]
    except mysql.connector.Error as err:
        print(f"Error getting row count: {err}")
        return -1

def query_worker(log_file_path, use_index_scenario):
    """单个查询工作者"""
    try:
        cnx = mysql.connector.connect(**DB_CONFIG)
        cursor = cnx.cursor()
        print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Connected to MySQL.")

        query_names = get_random_names_from_db(cursor, QUERIER_CONFIG['num_distinct_names_to_query'])
        if not query_names:
            print("No names to query. Exiting querier.")
            return

        # 准备CSV日志文件
        file_exists = os.path.isfile(log_file_path)
        with open(log_file_path, 'a', newline='', encoding='utf-8') as csvfile:
            csv_writer = csv.writer(csvfile)
            if not file_exists:
                csv_writer.writerow(['timestamp', 'query_name_value', 'duration_ms', 'rows_found', 'current_table_rows_approx'])

            start_query_loop_time = time.time()
            queries_done = 0

            while True:
                # 检查是否超时或达到查询次数
                if QUERIER_CONFIG['max_runtime_seconds'] and \
                   (time.time() - start_query_loop_time) > QUERIER_CONFIG['max_runtime_seconds']:
                    print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Max runtime reached. Stopping.")
                    break
                
                if QUERIER_CONFIG['num_queries_to_run'] and queries_done >= QUERIER_CONFIG['num_queries_to_run']:
                    print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Target number of queries reached. Stopping.")
                    break

                name_to_query = random.choice(query_names)
                
                # 获取当前表的大致行数 (可选，用于日志)
                current_rows_approx = get_table_row_count(cursor)

                # 记录查询前时间
                query_start_time = time.perf_counter()
                
                select_query = "SELECT * FROM Passenger WHERE name = %s"
                
                rows_found_count = 0
                try:
                    cursor.execute(select_query, (name_to_query,))
                    results = cursor.fetchall() # 获取所有结果以确保查询完成
                    rows_found_count = len(results)
                except mysql.connector.Error as err:
                    print(f"Query error for name '{name_to_query}': {err}")
                    # 可以选择跳过或记录错误

                # 记录查询后时间
                query_end_time = time.perf_counter()
                duration_ms = (query_end_time - query_start_time) * 1000
                
                # 写入日志
                log_entry = [
                    datetime.now().isoformat(),
                    name_to_query,
                    f"{duration_ms:.2f}",
                    rows_found_count,
                    current_rows_approx
                ]
                csv_writer.writerow(log_entry)
                
                queries_done += 1
                if queries_done % 10 == 0: # 每10次查询打印一次进度
                     print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Query {queries_done} for '{name_to_query}' done. Time: {duration_ms:.2f} ms. Rows found: {rows_found_count}. Approx table rows: {current_rows_approx}")

                # 控制查询频率
                time.sleep(QUERIER_CONFIG['query_interval_seconds'])
            
            print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Finished querying.")

    except mysql.connector.Error as err:
        print(f"Querier - Database connection error: {err}")
    finally:
        if 'cnx' in locals() and cnx.is_connected():
            cursor.close()
            cnx.close()
            print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}): MySQL connection closed.")


def run_querier(use_index_scenario=False):
    """主函数，启动查询器"""
    log_file = QUERIER_CONFIG['log_file_query_with_index'] if use_index_scenario else QUERIER_CONFIG['log_file_query_no_index']
    
    # 清理旧的日志文件
    if os.path.exists(log_file):
        os.remove(log_file)
        print(f"Removed old log file: {log_file}")
        
    print(f"Starting querier (Scenario: {'With Index' if use_index_scenario else 'No Index'})...")
    query_worker(log_file, use_index_scenario)
    print(f"Querier (Scenario: {'With Index' if use_index_scenario else 'No Index'}) has finished.")


if __name__ == '__main__':
    # 运行示例:
    # 1. 无索引场景查询
    # print("--- RUNNING QUERIER FOR NO INDEX SCENARIO ---")
    # # 假设此时数据库中 Passenger.name 没有额外索引
    # # 并且 inserter.py (无索引场景) 正在运行或已填充数据
    # run_querier(use_index_scenario=False)
    
    # 2. 有索引场景查询
    print("\n--- RUNNING QUERIER FOR WITH INDEX SCENARIO ---")
    # 假设此时 Passenger.name 已创建索引 (idx_passenger_name)
    # 并且 inserter.py (有索引场景) 正在运行或已填充数据
    run_querier(use_index_scenario=True)

    print("\nQuerier script finished. Choose a scenario to run by uncommenting.")
    print("Ensure the Passenger table is populated and indexes are set correctly for each scenario.")

