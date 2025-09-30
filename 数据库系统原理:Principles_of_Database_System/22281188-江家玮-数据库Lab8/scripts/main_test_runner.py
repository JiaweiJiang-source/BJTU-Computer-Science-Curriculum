# scripts/main_test_runner.py (Conceptual Orchestration Script)

import subprocess
import time
import os
import mysql.connector
from config import DB_CONFIG

def execute_sql_script(script_path):
    """执行指定的SQL脚本文件"""
    # mysql -u your_user -p your_password db_name < script.sql
    
    try:
        cnx = mysql.connector.connect(**DB_CONFIG)
        cursor = cnx.cursor()
        print(f"Executing SQL script: {script_path}")
        with open(script_path, 'r', encoding='utf-8') as f:
            sql_commands = f.read().split(';')
            for command in sql_commands:
                if command.strip():
                    try:
                        cursor.execute(command)
                        print(f"Executed: {command[:50].strip()}...")
                    except mysql.connector.Error as err:
                        # 忽略 'Query was empty' 错误或其他可忽略的错误
                        if err.errno == 1065: # Query was empty
                            pass
                        else:
                            print(f"SQL Error in command '{command.strip()}': {err}")
            cnx.commit()
        print(f"Successfully executed SQL script: {script_path}")
        cursor.close()
        cnx.close()
    except mysql.connector.Error as err:
        print(f"Failed to connect or execute SQL script {script_path}: {err}")
    except FileNotFoundError:
        print(f"SQL script not found: {script_path}")


def clear_passenger_table():
    """清空Passenger表"""
    try:
        cnx = mysql.connector.connect(**DB_CONFIG)
        cursor = cnx.cursor()
        print("Clearing Passenger table...")
        cursor.execute("TRUNCATE TABLE Passenger;")
        # 或者 DELETE FROM Passenger; 如果有外键约束且TRUNCATE不行
        cnx.commit()
        print("Passenger table cleared.")
        cursor.close()
        cnx.close()
    except mysql.connector.Error as err:
        print(f"Error clearing Passenger table: {err}")

def drop_passenger_name_index():
    """删除Passenger.name上的索引 (如果存在)"""
    try:
        cnx = mysql.connector.connect(**DB_CONFIG)
        cursor = cnx.cursor()
        print("Attempting to drop index idx_passenger_name if it exists...")
        # 检查索引是否存在
        cursor.execute("SHOW INDEX FROM Passenger WHERE Key_name = 'idx_passenger_name';")
        if cursor.fetchone():
            cursor.execute("DROP INDEX idx_passenger_name ON Passenger;")
            cnx.commit()
            print("Index idx_passenger_name dropped.")
        else:
            print("Index idx_passenger_name does not exist, no action taken.")
        cursor.close()
        cnx.close()
    except mysql.connector.Error as err:
        print(f"Error dropping index idx_passenger_name: {err}")


def run_test_scenario(scenario_name, use_index, sql_setup_script=None):
    """运行一个完整的测试场景 (插入+查询)"""
    print(f"\n{'='*20} RUNNING SCENARIO: {scenario_name} {'='*20}")

    # 1. 清空表
    clear_passenger_table()

    # 2. 设置索引状态
    if use_index:
        if sql_setup_script: # 例如 DBLab8_Add_Index_A.sql
             execute_sql_script(sql_setup_script)
        else: # 确保索引存在
            execute_sql_script('../DBLab8_Add_Index_A.sql') # 假设路径
    else: # 无索引场景
        drop_passenger_name_index() # 确保索引不存在

    # 3. 启动插入器和查询器
    # 这里用 subprocess.Popen 模拟，但错误处理和日志同步会更复杂
    
    inserter_cmd = ['python', 'inserter.py', '--scenario', 'with_index' if use_index else 'no_index']
    querier_cmd = ['python', 'querier.py', '--scenario', 'with_index' if use_index else 'no_index']

    print(f"Starting inserter for {scenario_name}...")
    # 或者像它们当前那样，通过调用不同的主函数（run_inserters(use_index_scenario=True/False)）
    
    # 示例：手动调用修改后的脚本
    # from inserter import run_inserters
    # from querier import run_querier

    # print(f"Running inserters for {'with_index' if use_index else 'no_index'} scenario...")
    # run_inserters(use_index_scenario=use_index)
    
    # print(f"Running queriers for {'with_index' if use_index else 'no_index'} scenario...")
    # run_querier(use_index_scenario=use_index)


    # 可能需要从 config.py 中读取日志文件名，或者通过命令行参数传递
    
    
    # 使它们在 if __name__ == '__main__': 部分能根据命令行参数选择运行哪个场景
    # 例如:
    # if __name__ == '__main__':
    #     import argparse
    #     parser = argparse.ArgumentParser()
    #     parser.add_argument('--scenario', choices=['no_index', 'with_index'], required=True)
    #     args = parser.parse_args()
    #     is_indexed_scenario = (args.scenario == 'with_index')
    #     if 'inserter.py': run_inserters(is_indexed_scenario)
    #     if 'querier.py': run_querier(is_indexed_scenario)

    print(f"Simulating inserter process for {scenario_name} (run inserter.py manually or adapt)...")
    # inserter_process = subprocess.Popen(inserter_cmd, cwd=os.path.dirname(os.path.abspath(__file__)))
    
    # 等待插入器运行一段时间或开始填充数据
    print("Waiting for a few seconds before starting querier...")
    time.sleep(15) # 给插入器一点启动时间

    print(f"Simulating querier process for {scenario_name} (run querier.py manually or adapt)...")
    # querier_process = subprocess.Popen(querier_cmd, cwd=os.path.dirname(os.path.abspath(__file__)))

    # 等待测试完成 (这里只是简单等待，实际中需要监控进程或日志)
    # inserter_process.wait(timeout=config.INSERTER_CONFIG.get('max_runtime_seconds', 1800)) # 假设有最大运行时间
    # querier_process.wait(timeout=config.QUERIER_CONFIG.get('max_runtime_seconds', 1800))
    
    print(f"Processes for {scenario_name} would be running. Monitor their logs.")
    print(f"Please run inserter.py and querier.py manually for each scenario after setting up the database.")
    print(f"Example for no-index: clear table, drop index, run inserter(False), run querier(False)")
    print(f"Example for with-index: clear table, add index, run inserter(True), run querier(True)")

    print(f"{'='*20} SCENARIO: {scenario_name} conceptually finished {'='*20}\n")


if __name__ == "__main__":
    # 0. 确保基础表结构存在 (如果第一次运行)
    # print("Setting up base schema (if not already done)...")
    # execute_sql_script('../DBLab8_Base_Schema.sql') # 调整路径

    # 场景1: 无索引压力测试
    # run_test_scenario("No Index on Passenger.name", use_index=False)
    
    # 等待用户分析完无索引场景的结果，或者脚本自动等待
    # input("Press Enter to continue to 'With Index' scenario...")

    # 场景2: 有索引压力测试
    run_test_scenario("With Index on Passenger.name", use_index=True, sql_setup_script='../DBLab8_Add_Index_A.sql')

    print("Main test runner conceptual script finished.")
    print("Please adapt and run individual components (SQL scripts, inserter.py, querier.py) as described in the lab report.")
