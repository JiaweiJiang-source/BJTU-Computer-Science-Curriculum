# scripts/inserter.py
# 数据插入脚本，记录耗时

import mysql.connector
import time
import csv
import os
import threading # 用于模拟并发
from datetime import datetime

from config import DB_CONFIG, INSERTER_CONFIG
from data_generator import generate_batch_passenger_data

# 全局变量，用于在多线程环境下安全地获取和增加 passenger_id
# 注意：如果 Passenger 表的 passenger_id 是 AUTO_INCREMENT, 则不需要脚本层面管理ID
# 脚本假设 passenger_id 是 AUTO_INCREMENT，因此 generate_batch_passenger_data 的 id 参数会被忽略
# 如果不是自增，需要修改 data_generator 和这里的逻辑来传递和管理ID

# 确保 results 目录存在
os.makedirs('../results', exist_ok=True)

current_total_rows = 0
lock = threading.Lock()

def get_table_row_count(cursor, table_name="Passenger"):
    """获取当前表的总行数"""
    try:
        cursor.execute(f"SELECT COUNT(*) FROM {table_name}")
        return cursor.fetchone()[0]
    except mysql.connector.Error as err:
        print(f"Error getting row count: {err}")
        return -1 # 表示获取失败

def insert_worker(worker_id, num_batches_to_insert, log_file_path, use_index_scenario):
    """单个插入工作线程/进程"""
    global current_total_rows
    
    try:
        cnx = mysql.connector.connect(**DB_CONFIG)
        cursor = cnx.cursor()
        print(f"Inserter Worker {worker_id} (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Connected to MySQL.")

        # 准备CSV日志文件
        file_exists = os.path.isfile(log_file_path)
        with open(log_file_path, 'a', newline='', encoding='utf-8') as csvfile:
            csv_writer = csv.writer(csvfile)
            if not file_exists: # 如果文件是新建的，写入表头
                csv_writer.writerow(['timestamp', 'worker_id', 'batch_size', 'duration_ms', 'total_rows_after_insert_approx'])
            
            for batch_num in range(num_batches_to_insert):
                # passenger_id 由数据库自增处理，data_generator 中的 id 参数仅作占位
                # 如果 passenger_id 不是自增，需要在这里计算唯一的 passenger_id
                passenger_batch = generate_batch_passenger_data(0, INSERTER_CONFIG['batch_size']) 
                
                # 记录插入前时间
                start_time = time.perf_counter()
                
                insert_query = """
                INSERT INTO Passenger (name, id_card_number, phone_number, email, frequent_flyer_number) 
                VALUES (%s, %s, %s, %s, %s) 
                """ # passenger_id 是自增的，所以不插入它
                
                # 修改数据以匹配查询 (移除passenger_id)
                data_to_insert = [(p[1], p[2], p[3], p[4], p[5]) for p in passenger_batch]

                try:
                    cursor.executemany(insert_query, data_to_insert)
                    cnx.commit()
                except mysql.connector.Error as err:
                    print(f"Worker {worker_id} - Error inserting batch: {err}")
                    cnx.rollback() # 出错时回滚
                    continue # 继续下一个批次

                # 记录插入后时间
                end_time = time.perf_counter()
                duration_ms = (end_time - start_time) * 1000
                
                with lock:
                    current_total_rows += INSERTER_CONFIG['batch_size']
                    rows_after_this_batch = current_total_rows 
                
                # 写入日志
                log_entry = [
                    datetime.now().isoformat(),
                    worker_id,
                    INSERTER_CONFIG['batch_size'],
                    f"{duration_ms:.2f}",
                    rows_after_this_batch
                ]
                csv_writer.writerow(log_entry)
                
                if batch_num % 10 == 0: # 每10个批次打印一次进度
                    print(f"Worker {worker_id} (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Batch {batch_num+1}/{num_batches_to_insert} inserted. Approx total rows: {rows_after_this_batch}. Last batch time: {duration_ms:.2f} ms.")

                # 控制插入频率
                if INSERTER_CONFIG['insert_interval_ms'] > 0:
                    time.sleep(INSERTER_CONFIG['insert_interval_ms'] / 1000.0)
            
            print(f"Inserter Worker {worker_id} (Scenario: {'With Index' if use_index_scenario else 'No Index'}): Finished inserting {num_batches_to_insert} batches.")

    except mysql.connector.Error as err:
        print(f"Worker {worker_id} - Database connection error: {err}")
    finally:
        if 'cnx' in locals() and cnx.is_connected():
            cursor.close()
            cnx.close()
            print(f"Inserter Worker {worker_id} (Scenario: {'With Index' if use_index_scenario else 'No Index'}): MySQL connection closed.")

def run_inserters(use_index_scenario=False):
    """主函数，启动并发插入器"""
    global current_total_rows
    current_total_rows = 0 # 重置计数器

    # 获取初始行数 (可选，用于更精确的 total_rows_after_insert)
    try:
        cnx_check = mysql.connector.connect(**DB_CONFIG)
        cursor_check = cnx_check.cursor()
        current_total_rows = get_table_row_count(cursor_check)
        print(f"Initial rows in Passenger table: {current_total_rows}")
        cursor_check.close()
        cnx_check.close()
    except mysql.connector.Error as err:
        print(f"Could not get initial row count: {err}")
        # current_total_rows 保持为0

    log_file = INSERTER_CONFIG['log_file_insert_with_index'] if use_index_scenario else INSERTER_CONFIG['log_file_insert_no_index']
    
    # 清理旧的日志文件
    if os.path.exists(log_file):
        os.remove(log_file)
        print(f"Removed old log file: {log_file}")

    total_rows_to_insert = INSERTER_CONFIG['total_rows_to_insert']
    batch_size = INSERTER_CONFIG['batch_size']
    num_concurrent = INSERTER_CONFIG['num_concurrent_inserters']

    if total_rows_to_insert == 0 or batch_size == 0 :
        print("total_rows_to_insert or batch_size is zero. Exiting inserter.")
        return

    total_batches = (total_rows_to_insert + batch_size -1) // batch_size # 向上取整
    batches_per_worker = (total_batches + num_concurrent - 1) // num_concurrent

    print(f"Starting inserters (Scenario: {'With Index' if use_index_scenario else 'No Index'})...")
    print(f"Target total rows: {total_rows_to_insert}, Batch size: {batch_size}, Concurrent workers: {num_concurrent}")
    print(f"Total batches: {total_batches}, Batches per worker: {batches_per_worker}")

    threads = []
    for i in range(num_concurrent):
        thread = threading.Thread(target=insert_worker, args=(i + 1, batches_per_worker, log_file, use_index_scenario))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    print(f"All inserter workers (Scenario: {'With Index' if use_index_scenario else 'No Index'}) have finished.")
    
    # 最终行数确认
    try:
        cnx_check = mysql.connector.connect(**DB_CONFIG)
        cursor_check = cnx_check.cursor()
        final_rows = get_table_row_count(cursor_check)
        print(f"Final rows in Passenger table after insertion: {final_rows}")
        cursor_check.close()
        cnx_check.close()
    except mysql.connector.Error as err:
        print(f"Could not get final row count: {err}")


if __name__ == '__main__':
    # 运行示例:
    # 1. 无索引场景
    # print("--- RUNNING INSERTER FOR NO INDEX SCENARIO ---")
    # 假设此时数据库中 Passenger.name 没有额外索引
    # 需要手动清空 Passenger 表: TRUNCATE TABLE Passenger;
    # run_inserters(use_index_scenario=False) 
    
    # 2. 有索引场景
    print("\n--- RUNNING INSERTER FOR WITH INDEX SCENARIO ---")
    # 假设此时 Passenger.name 已创建索引 (idx_passenger_name)
    # 需要手动清空 Passenger 表: TRUNCATE TABLE Passenger;
    run_inserters(use_index_scenario=True)

    print("\nInserter script finished. Choose a scenario to run by uncommenting.")
    print("Remember to TRUNCATE Passenger table before each run and manage indexes accordingly.")
