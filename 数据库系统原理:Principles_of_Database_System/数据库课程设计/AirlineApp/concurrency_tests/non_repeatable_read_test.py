# concurrency_tests/non_repeatable_read_test.py
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

def transaction_a_read_twice(isolation_level_for_a):
    """
    事务 A: 读取余额，等待，再次读取余额。
    Transaction A: Reads balance, waits, reads balance again.
    """
    conn_a = None
    try:
        conn_a = mysql.connector.connect(**DB_CONFIG)
        cursor_a = conn_a.cursor()
        cursor_a.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_a}")
        
        print(f"事务 A (隔离级别: {isolation_level_for_a}): 开始。")
        conn_a.start_transaction()
        
        cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        balance1_a = cursor_a.fetchone()[0]
        print(f"事务 A: 第一次读取账户 {ACCOUNT_ID_TO_TEST} 的余额是 {balance1_a}。")
        
        time.sleep(1.5) # 等待事务 B 更新并提交

        cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        balance2_a = cursor_a.fetchone()[0]
        print(f"事务 A: 第二次读取账户 {ACCOUNT_ID_TO_TEST} 的余额是 {balance2_a}。")
        
        if balance1_a == balance2_a:
            print("事务 A: 余额是可重复的。")
        else:
            print("事务 A: 余额不可重复 (发生了不可重复读)。")
            
        conn_a.commit()

    except mysql.connector.Error as err:
        print(f"事务 A 错误: {err}")
        if conn_a and conn_a.in_transaction:
            conn_a.rollback()
    finally:
        if conn_a and conn_a.is_connected():
            cursor_a.close()
            conn_a.close()
        print("事务 A: 结束。")

def transaction_b_update_and_commit(isolation_level_for_b):
    """
    事务 B: 更新余额并提交。
    Transaction B: Updates balance and commits.
    """
    conn_b = None
    try:
        conn_b = mysql.connector.connect(**DB_CONFIG)
        cursor_b = conn_b.cursor()
        if isolation_level_for_b: # 对于写入方来说不是严格必要的，但却是好习惯
            cursor_b.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_b}")

        print(f"事务 B (隔离级别: {isolation_level_for_b or 'DEFAULT'}): 开始。")
        conn_b.start_transaction()
        
        time.sleep(0.5) # 确保事务 A 已经完成了第一次读取
        
        cursor_b.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
        original_balance_b = cursor_b.fetchone()[0]
        
        new_balance_b = original_balance_b + 200
        cursor_b.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (new_balance_b, ACCOUNT_ID_TO_TEST))
        print(f"事务 B: 余额从 {original_balance_b} 更新到 {new_balance_b}。")
        
        conn_b.commit()
        print("事务 B: 提交更改。")

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
    setup_account_balance(initial_test_balance)

    print("\n--- 使用 READ COMMITTED 为事务 A 测试不可重复读 ---")
    # 事务 B 可以使用任何允许其提交更改的隔离级别运行
    thread_a_rc = threading.Thread(target=transaction_a_read_twice, args=("READ COMMITTED",))
    thread_b_rc = threading.Thread(target=transaction_b_update_and_commit, args=("READ COMMITTED",))

    thread_a_rc.start()
    time.sleep(0.1) # 让 A 稍微领先一点，以便它进行第一次读取
    thread_b_rc.start()

    thread_a_rc.join()
    thread_b_rc.join()
    print("不可重复读测试 (READ COMMITTED for A) 完成。")
    print("READ COMMITTED 预期结果: 事务 A 看到不同的余额 (不可重复读)。")

    print("\n--- 为下一次测试重置余额 ---")
    setup_account_balance(initial_test_balance)
    
    print("\n--- 使用 REPEATABLE READ 为事务 A 测试不可重复读 ---")
    thread_a_rr = threading.Thread(target=transaction_a_read_twice, args=("REPEATABLE READ",))
    thread_b_rr = threading.Thread(target=transaction_b_update_and_commit, args=("READ COMMITTED",)) # B 可以是 RC

    thread_a_rr.start()
    time.sleep(0.1)
    thread_b_rr.start()

    thread_a_rr.join()
    thread_b_rr.join()
    print("不可重复读测试 (REPEATABLE READ for A) 完成。")
    print("REPEATABLE READ 预期结果: 事务 A 看到相同的余额 (可重复读)。")

# # concurrency_tests/non_repeatable_read_test.py
# import mysql.connector
# import threading
# import time

# DB_CONFIG = {
#     'user': 'root',
#     'password': 'Aa585891', # Your MySQL password
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

# def transaction_a_read_twice(isolation_level_for_a):
#     """Transaction A: Reads balance, waits, reads balance again."""
#     conn_a = None
#     try:
#         conn_a = mysql.connector.connect(**DB_CONFIG)
#         cursor_a = conn_a.cursor()
#         cursor_a.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_a}")
        
#         print(f"Transaction A (Isolation: {isolation_level_for_a}): Starting.")
#         conn_a.start_transaction()
        
#         cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         balance1_a = cursor_a.fetchone()[0]
#         print(f"Transaction A: First read of balance for account {ACCOUNT_ID_TO_TEST} is {balance1_a}.")
        
#         time.sleep(1.5) # Wait for Transaction B to update and commit

#         cursor_a.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         balance2_a = cursor_a.fetchone()[0]
#         print(f"Transaction A: Second read of balance for account {ACCOUNT_ID_TO_TEST} is {balance2_a}.")
        
#         if balance1_a == balance2_a:
#             print("Transaction A: Balances are repeatable.")
#         else:
#             print("Transaction A: Balances are NOT repeatable (Non-Repeatable Read occurred).")
            
#         conn_a.commit()

#     except mysql.connector.Error as err:
#         print(f"Transaction A Error: {err}")
#         if conn_a and conn_a.in_transaction:
#             conn_a.rollback()
#     finally:
#         if conn_a and conn_a.is_connected():
#             cursor_a.close()
#             conn_a.close()
#         print("Transaction A: Finished.")

# def transaction_b_update_and_commit(isolation_level_for_b):
#     """Transaction B: Updates balance and commits."""
#     conn_b = None
#     try:
#         conn_b = mysql.connector.connect(**DB_CONFIG)
#         cursor_b = conn_b.cursor()
#         if isolation_level_for_b: # Not strictly necessary for writer, but good practice
#             cursor_b.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level_for_b}")

#         print(f"Transaction B (Isolation: {isolation_level_for_b or 'DEFAULT'}): Starting.")
#         conn_b.start_transaction()
        
#         time.sleep(0.5) # Ensure Transaction A has done its first read
        
#         cursor_b.execute("SELECT balance FROM Accounts WHERE account_id = %s", (ACCOUNT_ID_TO_TEST,))
#         original_balance_b = cursor_b.fetchone()[0]
        
#         new_balance_b = original_balance_b + 200
#         cursor_b.execute("UPDATE Accounts SET balance = %s WHERE account_id = %s", (new_balance_b, ACCOUNT_ID_TO_TEST))
#         print(f"Transaction B: Updated balance from {original_balance_b} to {new_balance_b}.")
        
#         conn_b.commit()
#         print("Transaction B: Committed changes.")

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
#     setup_account_balance(initial_test_balance)

#     print("\n--- Testing Non-Repeatable Read with READ COMMITTED for Transaction A ---")
#     # Transaction B can run with any isolation that allows it to commit its change
#     thread_a_rc = threading.Thread(target=transaction_a_read_twice, args=("READ COMMITTED",))
#     thread_b_rc = threading.Thread(target=transaction_b_update_and_commit, args=("READ COMMITTED",))

#     thread_a_rc.start()
#     time.sleep(0.1) # Give A a slight head start for its first read
#     thread_b_rc.start()

#     thread_a_rc.join()
#     thread_b_rc.join()
#     print("Non-Repeatable Read Test (READ COMMITTED for A) finished.")
#     print("Expected for READ COMMITTED: Transaction A sees different balances (Non-Repeatable Read).")

#     print("\n--- Resetting balance for next test ---")
#     setup_account_balance(initial_test_balance)
    
#     print("\n--- Testing Non-Repeatable Read with REPEATABLE READ for Transaction A ---")
#     thread_a_rr = threading.Thread(target=transaction_a_read_twice, args=("REPEATABLE READ",))
#     thread_b_rr = threading.Thread(target=transaction_b_update_and_commit, args=("READ COMMITTED",)) # B can be RC

#     thread_a_rr.start()
#     time.sleep(0.1)
#     thread_b_rr.start()

#     thread_a_rr.join()
#     thread_b_rr.join()
#     print("Non-Repeatable Read Test (REPEATABLE READ for A) finished.")
#     print("Expected for REPEATABLE READ: Transaction A sees the same balance (Repeatable Read).")
