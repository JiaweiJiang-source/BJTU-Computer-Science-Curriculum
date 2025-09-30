# concurrency_tests/dirty_read_test.py
import mysql.connector
import threading
import time

DB_CONFIG = {
    'user': 'root',
    'password': 'Aa585891', 
    'host': 'localhost',
    'database': 'AirlineDB',
    'raise_on_warnings': True
}

ACCOUNT_ID_TO_TEST = 1

def setup_account_balance(initial_balance):
    """
    重置账户余额。
    Resets the balance of the test account.
    """
    conn = None
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        cursor.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (initial_balance, ACCOUNT_ID_TO_TEST))
        conn.commit()
        print(f"账户 {ACCOUNT_ID_TO_TEST} 的余额已重置为 {initial_balance}")
    except mysql.connector.Error as err:
        print(f"设置账户时出错: {err}")
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

def transaction_a_update_and_rollback(isolation_level_for_a):
    """
    事务 A: 更新余额，等待，然后回滚。
    Transaction A: Updates balance, waits, then rolls back.
    """
    conn_a = None
    try:
        conn_a = mysql.connector.connect(**DB_CONFIG)
        cursor_a = conn_a.cursor()
        if isolation_level_for_a: 
             cursor_a.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_a}")
        
        print(f"事务 A (隔离级别: {isolation_level_for_a or 'DEFAULT'}): 开始。")
        conn_a.start_transaction()
        
        cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        original_balance = cursor_a.fetchone()[0]
        print(f"事务 A: 账户 {ACCOUNT_ID_TO_TEST} 的原始余额是 {original_balance}。")
        
        new_balance_a = original_balance - 100
        cursor_a.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (new_balance_a, ACCOUNT_ID_TO_TEST))
        print(f"事务 A: 余额更新为 {new_balance_a} (未提交)。")
        
        time.sleep(2) # 暂停以允许事务 B 读取未提交的数据

        conn_a.rollback()
        print("事务 A: 回滚更改。")
        
        cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        final_balance_a = cursor_a.fetchone()[0]
        print(f"事务 A: 回滚后的最终余额是 {final_balance_a}。")

    except mysql.connector.Error as err:
        print(f"事务 A 错误: {err}")
        if conn_a and conn_a.in_transaction:
            conn_a.rollback()
    finally:
        if conn_a and conn_a.is_connected():
            cursor_a.close()
            conn_a.close()
        print("事务 A: 结束。")

def transaction_b_read_balance(isolation_level_for_b):
    """
    事务 B: 读取余额。
    Transaction B: Reads balance.
    """
    conn_b = None
    try:
        conn_b = mysql.connector.connect(**DB_CONFIG)
        cursor_b = conn_b.cursor()
        cursor_b.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_b}")
        
        print(f"事务 B (隔离级别: {isolation_level_for_b}): 开始。")
        conn_b.start_transaction() # 事务 B 也开始一个事务
        
        time.sleep(0.5) # 确保事务 A 已更新 (但未提交)
        
        cursor_b.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        balance_b = cursor_b.fetchone()[0]
        print(f"事务 B: 读取到账户 {ACCOUNT_ID_TO_TEST} 的余额为 {balance_b}。")
        
        conn_b.commit() # 提交事务 B (只读)

    except mysql.connector.Error as err:
        print(f"事务 B 错误: {err}")
        if conn_b and conn_b.in_transaction:
            conn_b.rollback()
    finally:
        if conn_b and conn_b.is_connected():
            cursor_b.close()
            conn_b.close()
        print("事务 B: 结束。")

if __name__ == "__main__":
    initial_test_balance = 1000.00
    setup_account_balance(initial_test_balance) # 每次测试运行前重置余额

    print("\n--- 使用 READ UNCOMMITTED 为事务 B 测试脏读 ---")

    # 事务 A 使用 REPEATABLE READ，因为它执行更新并回滚，这通常是一个合理的隔离级别。
    # 事务 B 使用 READ UNCOMMITTED，这将使其能够读取事务 A 未提交的更改。
    thread_a = threading.Thread(target=transaction_a_update_and_rollback, args=("REPEATABLE READ",))
    thread_b = threading.Thread(target=transaction_b_read_balance, args=("READ UNCOMMITTED",))

    thread_a.start()
    time.sleep(0.1) # 稍微延迟以确保事务 A 先进行更新
    thread_b.start()

    thread_a.join()
    thread_b.join()
    print("脏读测试 (READ UNCOMMITTED for B) 完成。")
    print("READ UNCOMMITTED 预期结果: 事务 B 读取到事务 A 未提交的更新。")
    
    print("\n--- 为下一次测试重置余额 ---")
    setup_account_balance(initial_test_balance)

    print("\n--- 使用 READ COMMITTED 为事务 B 测试脏读 ---")
    # 事务 B 使用 READ COMMITTED，它应该只读取已提交的数据。
    thread_a = threading.Thread(target=transaction_a_update_and_rollback, args=("REPEATABLE READ",))
    thread_b = threading.Thread(target=transaction_b_read_balance, args=("READ COMMITTED",))

    thread_a.start()
    time.sleep(0.1)
    thread_b.start()

    thread_a.join()
    thread_b.join()
    print("脏读测试 (READ COMMITTED for B) 完成。")
    print("READ COMMITTED 预期结果: 事务 B 读取到原始的已提交余额，而不是事务 A 未提交的更新。")
    
# # concurrency_tests/dirty_read_test.py
# import mysql.connector
# import threading
# import time

# DB_CONFIG = {
#     'user': 'root',
#     'password': 'Aa585891', 
#     'host': 'localhost',
#     'database': 'AirlineDB',
#     'raise_on_warnings': True
# }

# ACCOUNT_ID_TO_TEST = 1

# def setup_account_balance(initial_balance):
#     conn = None
#     try:
#         conn = mysql.connector.connect(**DB_CONFIG)
#         cursor = conn.cursor()
#         cursor.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (initial_balance, ACCOUNT_ID_TO_TEST))
#         conn.commit()
#         print(f"Account {ACCOUNT_ID_TO_TEST} balance reset to {initial_balance}")
#     except mysql.connector.Error as err:
#         print(f"Error setting up account: {err}")
#     finally:
#         if conn and conn.is_connected():
#             cursor.close()
#             conn.close()

# def transaction_a_update_and_rollback(isolation_level_for_a):
#     """Transaction A: Updates balance, waits, then rolls back."""
#     conn_a = None
#     try:
#         conn_a = mysql.connector.connect(**DB_CONFIG)
#         cursor_a = conn_a.cursor()
#         if isolation_level_for_a: 
#              cursor_a.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_a}")
        
#         print(f"Transaction A (Isolation: {isolation_level_for_a or 'DEFAULT'}): Starting.")
#         conn_a.start_transaction()
        
#         cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         original_balance = cursor_a.fetchone()[0]
#         print(f"Transaction A: Original balance for account {ACCOUNT_ID_TO_TEST} is {original_balance}.")
        
#         new_balance_a = original_balance - 100
#         cursor_a.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (new_balance_a, ACCOUNT_ID_TO_TEST))
#         print(f"Transaction A: Updated balance to {new_balance_a} (UNCOMMITTED).")
        
#         time.sleep(2) 

#         conn_a.rollback()
#         print("Transaction A: Rolled back changes.")
        
#         cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         final_balance_a = cursor_a.fetchone()[0]
#         print(f"Transaction A: Final balance after rollback is {final_balance_a}.")

#     except mysql.connector.Error as err:
#         print(f"Transaction A Error: {err}")
#         if conn_a and conn_a.in_transaction:
#             conn_a.rollback()
#     finally:
#         if conn_a and conn_a.is_connected():
#             cursor_a.close()
#             conn_a.close()
#         print("Transaction A: Finished.")

# def transaction_b_read_balance(isolation_level_for_b):
#     """Transaction B: Reads balance."""
#     conn_b = None
#     try:
#         conn_b = mysql.connector.connect(**DB_CONFIG)
#         cursor_b = conn_b.cursor()
#         cursor_b.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_b}")
        
#         print(f"Transaction B (Isolation: {isolation_level_for_b}): Starting.")
#         conn_b.start_transaction() # Start transaction for B as well
        
#         time.sleep(0.5) # Ensure Transaction A has updated (but not committed)
        
#         cursor_b.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         balance_b = cursor_b.fetchone()[0]
#         print(f"Transaction B: Read balance for account {ACCOUNT_ID_TO_TEST} as {balance_b}.")
        
#         conn_b.commit() # Commit B's transaction (read only)

#     except mysql.connector.Error as err:
#         print(f"Transaction B Error: {err}")
#         if conn_b and conn_b.in_transaction:
#             conn_b.rollback()
#     finally:
#         if conn_b and conn_b.is_connected():
#             cursor_b.close()
#             conn_b.close()
#         print("Transaction B: Finished.")

# if __name__ == "__main__":
#     initial_test_balance = 1000.00
#     setup_account_balance(initial_test_balance) # Reset balance before each test run

#     print("\n--- Testing Dirty Read with READ UNCOMMITTED for Transaction B ---")


#     thread_a = threading.Thread(target=transaction_a_update_and_rollback, args=("REPEATABLE READ",))
#     thread_b = threading.Thread(target=transaction_b_read_balance, args=("READ UNCOMMITTED",))

#     thread_a.start()
#     time.sleep(0.1) 
#     thread_b.start()

#     thread_a.join()
#     thread_b.join()
#     print("Dirty Read Test (READ UNCOMMITTED for B) finished.")
#     print("Expected for READ UNCOMMITTED: Transaction B reads the uncommitted update from A.")
    
#     print("\n--- Resetting balance for next test ---")
#     setup_account_balance(initial_test_balance)

#     print("\n--- Testing Dirty Read with READ COMMITTED for Transaction B ---")
#     thread_a = threading.Thread(target=transaction_a_update_and_rollback, args=("REPEATABLE READ",))
#     thread_b = threading.Thread(target=transaction_b_read_balance, args=("READ COMMITTED",))

#     thread_a.start()
#     time.sleep(0.1)
#     thread_b.start()

#     thread_a.join()
#     thread_b.join()
#     print("Dirty Read Test (READ COMMITTED for B) finished.")
#     print("Expected for READ COMMITTED: Transaction B reads the original committed balance, not A's uncommitted update.")
