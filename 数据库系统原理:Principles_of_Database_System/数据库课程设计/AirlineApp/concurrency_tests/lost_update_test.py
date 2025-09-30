# concurrency_tests/lost_update_test.py
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
FLIGHT_ID_TO_TEST = 3001 

def setup_flight_seats(initial_booked_seats):
    """
    重置航班的已预订座位数。
    Reset the booked seats count for a flight.
    """
    conn = None
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        
        cursor.execute("UPDATE Flight SET booked_seats = %s WHERE flight_id = %s", 
                       (initial_booked_seats, FLIGHT_ID_TO_TEST))
        conn.commit()
        print(f"航班 {FLIGHT_ID_TO_TEST} 的已预订座位数已重置为 {initial_booked_seats}")
    except mysql.connector.Error as err:
        print(f"设置航班座位时出错: {err}")
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

def simulate_booking_lost_update(thread_name, isolation_level):
    """
    模拟可能导致更新丢失的读-修改-写循环。
    Simulates a read-modify-write cycle that can lead to lost updates.
    """
    conn = None
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        cursor.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level}")
        print(f"{thread_name} (隔离级别: {isolation_level}): 开始。")
        
        conn.start_transaction()

        # 1. 读取当前的已预订座位数
        # 1. Read current booked_seats
        cursor.execute("SELECT booked_seats FROM Flight WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
        current_seats_read = cursor.fetchone()[0]
        print(f"{thread_name}: 读取到的已预订座位数为 {current_seats_read}。")

        # 模拟一些处理时间，然后写回
        # Simulate some processing time before writing back
        time.sleep(0.5) 

        # 2. 在应用程序内存中修改
        # 2. Modify in application memory
        new_seats_calculated = current_seats_read + 1
        print(f"{thread_name}: 计算出的新座位数为 {new_seats_calculated}。")

        # 3. 写回
        # 3. Write back
        # 如果不是原子操作或没有正确锁定，这将是问题所在
        # This is the problematic step if not atomic or properly locked
        cursor.execute("UPDATE Flight SET booked_seats = %s WHERE flight_id = %s", 
                       (new_seats_calculated, FLIGHT_ID_TO_TEST))
        
        # 对于 REPEATABLE READ 和 SERIALIZABLE，如果另一个事务持有锁，此更新可能会等待或失败。
        # 对于 READ COMMITTED，它可能会被覆盖。
        # For REPEATABLE READ and SERIALIZABLE, this update might wait or fail if another tx holds locks.
        # For READ COMMITTED, it might overwrite.
        
        conn.commit()
        print(f"{thread_name}: 提交更新。尝试将已预订座位数设置为 {new_seats_calculated}。")

    except mysql.connector.Error as err:
        print(f"{thread_name} 错误: {err}")
        if conn and conn.in_transaction:
            conn.rollback()
            print(f"{thread_name}: 因错误回滚。")
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()
        print(f"{thread_name}: 结束。")

def check_final_seats(expected_increment):
    """
    检查最终的已预订座位数。
    Check the final booked seats count.
    """
    conn = None
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        cursor.execute("SELECT booked_seats FROM Flight WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
        final_seats = cursor.fetchone()[0]
        print(f"\n最终检查: 航班 {FLIGHT_ID_TO_TEST} 的已预订座位数为 {final_seats}。")
        print(f"预期从初始值增加 {expected_increment}。")
        if final_seats < initial_booked_seats_for_test + expected_increment:
            print("很可能发生了更新丢失！")
        elif final_seats == initial_booked_seats_for_test + expected_increment:
            print("更新似乎已正确应用 (此处未观察到更新丢失)。")
        else:
            print("座位数高于预期，请检查逻辑。")
            
    except mysql.connector.Error as err:
        print(f"检查最终座位时出错: {err}")
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

if __name__ == "__main__":
    # 从 DBLab3_Data_Insert.sql 获取航班 3001 的初始已预订座位数
    # 根据您的插入脚本：航班 3001 最初有 1 个预订 (4001)，
    # 还有另一个 (4003)。因此，如果触发器在插入时正确运行，已预订座位数应为 2。
    # 为了测试，我们将其设置为已知值。
    # Get initial booked_seats for flight 3001 from DBLab3_Data_Insert.sql
    # From your insert script: flight 3001 has 1 booking initially (4001),
    # and another (4003). So booked_seats should be 2 if triggers ran correctly on insert.
    # For the test, let's set it to a known value.
    initial_booked_seats_for_test = 5 
    setup_flight_seats(initial_booked_seats_for_test)

    # 使用 READ COMMITTED 进行测试 (在这种情况下更容易发生更新丢失)
    # Test with READ COMMITTED (more prone to lost updates in this scenario)
    print("\n--- 使用 READ COMMITTED 测试更新丢失 ---")
    print("模拟两次并发预订尝试...")
    
    # 两个线程都将尝试将座位数增加 1。
    # 如果没有更新丢失，最终计数应为 initial_booked_seats_for_test + 2。
    # 如果发生更新丢失，最终计数可能为 initial_booked_seats_for_test + 1。
    # Both threads will try to increment the seat count by 1.
    # If no lost update, final count should be initial_booked_seats_for_test + 2.
    # If lost update, final count might be initial_booked_seats_for_test + 1.

    thread1_rc = threading.Thread(target=simulate_booking_lost_update, args=("线程-1 (RC)", "READ COMMITTED"))
    thread2_rc = threading.Thread(target=simulate_booking_lost_update, args=("线程-2 (RC)", "READ COMMITTED"))

    thread1_rc.start()
    # 引入微小延迟以增加演示时交错的可能性
    # 但真正的并发取决于操作系统调度和数据库行为。
    # Introduce slight delay to increase chance of interleaving for demonstration
    # but true concurrency depends on OS scheduling and DB behavior.
    time.sleep(0.05) 
    thread2_rc.start()

    thread1_rc.join()
    thread2_rc.join()
    
    check_final_seats(expected_increment=2)
    print("更新丢失测试 (READ COMMITTED) 完成。")
    print("READ COMMITTED 预期结果 (使用此读-修改-写逻辑): 有可能发生更新丢失。")

    print("\n--- 为下一次测试重置座位数 ---")
    setup_flight_seats(initial_booked_seats_for_test)

    # 使用 REPEATABLE READ 进行测试
    # MySQL 的 REPEATABLE READ 与 InnoDB 通常会防止这种特定类型的更新丢失
    # 对于简单的 UPDATE 语句，因为它的锁定机制，但读-修改-写循环仍然是一个问题。
    # SELECT ... FOR UPDATE 是锁定行的更好方法。
    # Test with REPEATABLE READ
    # MySQL's REPEATABLE READ with InnoDB often prevents this specific type of lost update
    # for simple UPDATE statements due to its locking, but the R-M-W cycle is still a concern.
    # SELECT ... FOR UPDATE would be a better way to lock the row.
    print("\n--- 使用 REPEATABLE READ 测试更新丢失 ---")
    thread1_rr = threading.Thread(target=simulate_booking_lost_update, args=("线程-1 (RR)", "REPEATABLE READ"))
    thread2_rr = threading.Thread(target=simulate_booking_lost_update, args=("线程-2 (RR)", "REPEATABLE READ"))

    thread1_rr.start()
    time.sleep(0.05)
    thread2_rr.start()

    thread1_rr.join()
    thread2_rr.join()

    check_final_seats(expected_increment=2)
    print("更新丢失测试 (REPEATABLE READ) 完成。")
    print("REPEATABLE READ 预期结果: 可能会因锁定而防止更新丢失，或者一个事务可能等待/出错。")
    
    print("\n--- 使用原子更新 (正确方法) ---")
    # 这与隔离级别无关，而是正确的应用程序逻辑
    # This is not about isolation levels but correct application logic
    setup_flight_seats(initial_booked_seats_for_test)
    def atomic_increment(thread_name):
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        # 这对于计数器来说是原子且安全的，可防止更新丢失
        # This is atomic and safe against lost updates for counters
        cursor.execute("UPDATE Flight SET booked_seats = booked_seats + 1 WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
        conn.commit()
        print(f"{thread_name}: 原子地增加了座位。")
        cursor.close()
        conn.close()

    thread_atomic1 = threading.Thread(target=atomic_increment, args=("原子线程-1",))
    thread_atomic2 = threading.Thread(target=atomic_increment, args=("原子线程-2",))
    thread_atomic1.start()
    thread_atomic2.start()
    thread_atomic1.join()
    thread_atomic2.join()
    check_final_seats(expected_increment=2)
    print("原子更新测试完成。这对于计数器来说应该始终是正确的。")

# # concurrency_tests/lost_update_test.py
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
# FLIGHT_ID_TO_TEST = 3001 # 假设存在 DBLab3_Data_Insert.sql

# def setup_flight_seats(initial_booked_seats):
#     conn = None
#     try:
#         conn = mysql.connector.connect(**DB_CONFIG)
#         cursor = conn.cursor()
        
#         cursor.execute("UPDATE Flight SET booked_seats = %s WHERE flight_id = %s", 
#                        (initial_booked_seats, FLIGHT_ID_TO_TEST))
#         conn.commit()
#         print(f"Flight {FLIGHT_ID_TO_TEST} booked_seats reset to {initial_booked_seats}")
#     except mysql.connector.Error as err:
#         print(f"Error setting up flight seats: {err}")
#     finally:
#         if conn and conn.is_connected():
#             cursor.close()
#             conn.close()

# def simulate_booking_lost_update(thread_name, isolation_level):
#     """Simulates a read-modify-write cycle that can lead to lost updates."""
#     conn = None
#     try:
#         conn = mysql.connector.connect(**DB_CONFIG)
#         cursor = conn.cursor()
#         cursor.execute(f"SET SESSION TRANSACTION ISOLATION LEVEL {isolation_level}")
#         print(f"{thread_name} (Isolation: {isolation_level}): Starting.")
        
#         conn.start_transaction()

#         # 1. Read current booked_seats
#         cursor.execute("SELECT booked_seats FROM Flight WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
#         current_seats_read = cursor.fetchone()[0]
#         print(f"{thread_name}: Read booked_seats as {current_seats_read}.")

#         # Simulate some processing time before writing back
#         time.sleep(0.5) 

#         # 2. Modify in application memory
#         new_seats_calculated = current_seats_read + 1
#         print(f"{thread_name}: Calculated new_seats as {new_seats_calculated}.")

#         # 3. Write back
#         # This is the problematic step if not atomic or properly locked
#         cursor.execute("UPDATE Flight SET booked_seats = %s WHERE flight_id = %s", 
#                        (new_seats_calculated, FLIGHT_ID_TO_TEST))
        
#         # For REPEATABLE READ and SERIALIZABLE, this update might wait or fail if another tx holds locks.
#         # For READ COMMITTED, it might overwrite.
        
#         conn.commit()
#         print(f"{thread_name}: Committed update. Attempted to set booked_seats to {new_seats_calculated}.")

#     except mysql.connector.Error as err:
#         print(f"{thread_name} Error: {err}")
#         if conn and conn.in_transaction:
#             conn.rollback()
#             print(f"{thread_name}: Rolled back due to error.")
#     finally:
#         if conn and conn.is_connected():
#             cursor.close()
#             conn.close()
#         print(f"{thread_name}: Finished.")

# def check_final_seats(expected_increment):
#     conn = None
#     try:
#         conn = mysql.connector.connect(**DB_CONFIG)
#         cursor = conn.cursor()
#         cursor.execute("SELECT booked_seats FROM Flight WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
#         final_seats = cursor.fetchone()[0]
#         print(f"\nFinal check: Flight {FLIGHT_ID_TO_TEST} has {final_seats} booked_seats.")
#         print(f"Expected increment from initial was {expected_increment}.")
#         if final_seats < initial_booked_seats_for_test + expected_increment:
#             print("LOST UPDATE LIKELY OCCURRED!")
#         elif final_seats == initial_booked_seats_for_test + expected_increment:
#             print("Updates seem to be correctly applied (no lost update observed here).")
#         else:
#             print("Seats are higher than expected, check logic.")
            
#     except mysql.connector.Error as err:
#         print(f"Error checking final seats: {err}")
#     finally:
#         if conn and conn.is_connected():
#             cursor.close()
#             conn.close()

# if __name__ == "__main__":
#     # Get initial booked_seats for flight 3001 from DBLab3_Data_Insert.sql
#     # From your insert script: flight 3001 has 1 booking initially (4001),
#     # and another (4003). So booked_seats should be 2 if triggers ran correctly on insert.
#     # For the test, let's set it to a known value.
#     initial_booked_seats_for_test = 5 
#     setup_flight_seats(initial_booked_seats_for_test)

#     # Test with READ COMMITTED (more prone to lost updates in this scenario)
#     print("\n--- Testing Lost Update with READ COMMITTED ---")
#     print("Simulating two concurrent booking attempts...")
    
#     # Both threads will try to increment the seat count by 1.
#     # If no lost update, final count should be initial_booked_seats_for_test + 2.
#     # If lost update, final count might be initial_booked_seats_for_test + 1.

#     thread1_rc = threading.Thread(target=simulate_booking_lost_update, args=("Thread-1 (RC)", "READ COMMITTED"))
#     thread2_rc = threading.Thread(target=simulate_booking_lost_update, args=("Thread-2 (RC)", "READ COMMITTED"))

#     thread1_rc.start()
#     # Introduce slight delay to increase chance of interleaving for demonstration
#     # but true concurrency depends on OS scheduling and DB behavior.
#     time.sleep(0.05) 
#     thread2_rc.start()

#     thread1_rc.join()
#     thread2_rc.join()
    
#     check_final_seats(expected_increment=2)
#     print("Lost Update Test (READ COMMITTED) finished.")
#     print("Expected for READ COMMITTED (with this R-M-W logic): Lost update is possible.")

#     print("\n--- Resetting seats for next test ---")
#     setup_flight_seats(initial_booked_seats_for_test)

#     # Test with REPEATABLE READ
#     # MySQL's REPEATABLE READ with InnoDB often prevents this specific type of lost update
#     # for simple UPDATE statements due to its locking, but the R-M-W cycle is still a concern.
#     # SELECT ... FOR UPDATE would be a better way to lock the row.
#     print("\n--- Testing Lost Update with REPEATABLE READ ---")
#     thread1_rr = threading.Thread(target=simulate_booking_lost_update, args=("Thread-1 (RR)", "REPEATABLE READ"))
#     thread2_rr = threading.Thread(target=simulate_booking_lost_update, args=("Thread-2 (RR)", "REPEATABLE READ"))

#     thread1_rr.start()
#     time.sleep(0.05)
#     thread2_rr.start()

#     thread1_rr.join()
#     thread2_rr.join()

#     check_final_seats(expected_increment=2)
#     print("Lost Update Test (REPEATABLE READ) finished.")
#     print("Expected for REPEATABLE READ: May prevent lost update due to locking, or one transaction might wait/error.")
    
#     print("\n--- Using Atomic Update (Correct Way) ---")
#     # This is not about isolation levels but correct application logic
#     setup_flight_seats(initial_booked_seats_for_test)
#     def atomic_increment(thread_name):
#         conn = mysql.connector.connect(**DB_CONFIG)
#         cursor = conn.cursor()
#         # This is atomic and safe against lost updates for counters
#         cursor.execute("UPDATE Flight SET booked_seats = booked_seats + 1 WHERE flight_id = %s", (FLIGHT_ID_TO_TEST,))
#         conn.commit()
#         print(f"{thread_name}: Atomically incremented seats.")
#         cursor.close()
#         conn.close()

#     thread_atomic1 = threading.Thread(target=atomic_increment, args=("AtomicThread-1",))
#     thread_atomic2 = threading.Thread(target=atomic_increment, args=("AtomicThread-2",))
#     thread_atomic1.start()
#     thread_atomic2.start()
#     thread_atomic1.join()
#     thread_atomic2.join()
#     check_final_seats(expected_increment=2)
#     print("Atomic update test finished. This should always be correct for counters.")

