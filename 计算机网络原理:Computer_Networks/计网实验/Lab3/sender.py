import socket
import threading
import time
import random
from packet import Packet, TYPE_DATA, TYPE_ACK, TYPE_FIN 

# --- 配置信息 ---
RECEIVER_IP = '0.0.0.0' 
RECEIVER_PORT = 8001 
SENDER_PORT = 8000 
WINDOW_SIZE = 32 # GBN 的窗口大小 N
TIMEOUT_INTERVAL = 0.5 # 超时重传的时间间隔 (秒)
PACKET_SIZE = 1
# 模拟参数
LOSS_RATE = 0.1 # 模拟丢包率 (10%)
# CORRUPTION_RATE = 0.05 # 模拟数据包损坏率 (5%) 

# --- 状态变量 ---
base = 0 
next_seq_num = 0 
send_buffer = {} # 存储已发送但未确认的数据包
buffer_lock = threading.Lock() 
timer = None # 定时器对象
timer_lock = threading.Lock() 
ack_received = threading.Event() 

# --- UDP 套接字 ---
sender_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sender_socket.bind(('0.0.0.0', SENDER_PORT)) 


# 模拟不可靠的 UDP 发送
def udt_send(packet_bytes, addr):
    # 模拟丢包
    if random.random() < LOSS_RATE:
        print(f"**** SIMULATED LOSS: Packet {Packet.deserialize(packet_bytes).seq_num} ****")
        return # 直接返回，不发送
    # 模拟数据包损坏 
    # ... (implementation omitted for brevity)
    # 实际发送 UDP 数据包
    sender_socket.sendto(packet_bytes, addr)

# 启动（或重启）定时器
def start_timer():
    global timer
    stop_timer() # 先停止任何可能正在运行的旧定时器
    with timer_lock: # 锁定定时器对象保证线程安全
        # 创建一个新的定时器，当时间到达 TIMEOUT_INTERVAL 秒后执行 handle_timeout 函数
        timer = threading.Timer(TIMEOUT_INTERVAL, handle_timeout)
        timer.start() # 启动定时器
        print(f"TIMER STARTED for base {base}") # 打印定时器启动信息

# 停止当前定时器
def stop_timer():
    global timer
    with timer_lock: # 锁定定时器对象
        # 正在运行
        if timer and timer.is_alive():
            timer.cancel()
            print("TIMER STOPPED")
        timer = None # 重制

# 超时
def handle_timeout():
    global timer
    print(f"TIMEOUT! Resending packets from {base} to {next_seq_num - 1}") 
    with timer_lock: 
        timer = None 
    with buffer_lock: 
        # GBN：重传从 [base，next_seq_num-1] -----> 已发送，但未确认的包
        for i in range(base, next_seq_num):
            if i in send_buffer: # 数据包仍在缓冲区，发送但是无确认
                print(f"Resending packet {i}") 
                udt_send(send_buffer[i], (RECEIVER_IP, RECEIVER_PORT))
    # 如果窗口中仍有未确认的数据包，则重新启动定时器
    if base < next_seq_num:
        start_timer()

# --- ACK 接收线程 ---
# 该函数在一个单独的线程中运行，专门负责接收来自接收方的 ACK
def receive_acks():
    global base
    while True: # 持续监听
        try:
            # 尝试从套接字接收数据 (最大 1024 字节)，忽略来源地址
            ack_bytes, _ = sender_socket.recvfrom(1024) # Buffer size for ACK
            # 反序列化收到的字节流为 Packet 对象
            ack_packet = Packet.deserialize(ack_bytes)

            # 检查 ACK 包是否有效：对象存在、类型为 ACK、校验和正确
            if ack_packet and ack_packet.data_type == TYPE_ACK and ack_packet.verify_checksum():
                ack_num = ack_packet.seq_num # 获取确认序号
                print(f"Received ACK {ack_num}") # 打印收到 ACK 的信息

                # 若果 ack_num >= base，说明这是一个有效的 ACK
                # 需要更新窗口的 base
                if ack_num >= base:
                    with buffer_lock: # 锁定发送缓冲区
                        # 从缓冲区中移除已被确认的数据包 (从旧 base 到 ack_num)
                        for i in range(base, ack_num + 1):
                            send_buffer.pop(i, None) # pop 如果键不存在，返回 None
                    # 更新 base 为收到的确认序号 + 1
                    base = ack_num + 1
                    print(f"Window base updated to {base}") # 打印窗口 base 更新信息
                    # 如果 base 追上了 next_seq_num，说明所有已发送的包都确认了
                    if base == next_seq_num:
                        stop_timer() 
                    else:
                        # 否则，窗口中还有未确认的包，需要为新的 base 重启定时器
                        start_timer() 
                    # 设置事件，通知主发送线程可能有新的窗口空间可用
                    ack_received.set() # Signal main thread
                else:
                     # 如果 ack_num < base，说明是过时或重复的 ACK
                     print(f"Received duplicate/old ACK {ack_num}, ignored.") # 打印忽略信息

            # 处理接收到的 FIN-ACK，表示接收方确认收到 FIN
            elif ack_packet and ack_packet.data_type == TYPE_FIN:
                 print("Received FIN ACK. Transmission likely complete.") # 打印收到 FIN-ACK
                 break # 退出 ACK 接收循环 (或者设置一个标志位通知主线程)

            else:
                # 如果收到的包无法解析、类型错误或校验和错误
                print("Received corrupted or invalid ACK packet.") # 打印收到无效 ACK 的信息

        except socket.timeout:
            # 如果 recvfrom 超时，继续循环等待
            continue 
        except Exception as e:
            print(f"Error receiving ACK: {e}")
            break 

# --- 发送 ---
def send_data(filename):
    global next_seq_num # 全局
    try:
        with open(filename, 'rb') as f:
            # 创建并启动 ACK 接收线程，设置为守护线程 (主线程退出时它也退出)
            ack_listener = threading.Thread(target=receive_acks, daemon=True)
            ack_listener.start()

            while True: 
                # 检查窗口是否已满，如果已满则等待 ACK
                while next_seq_num >= base + WINDOW_SIZE:
                    print(f"Window full (base={base}, next_seq={next_seq_num}). Waiting...") 
                    ack_received.clear() # 清除 ACK 事件标志
                    # 等待 ACK 到达事件，或者短暂超时，防止死锁
                    ack_received.wait(TIMEOUT_INTERVAL / 2) 

                # 读
                data_chunk = f.read(PACKET_SIZE)
                # 文件读完了｜｜或者传送完了
                if not data_chunk and next_seq_num == base: 
                     break 

                if data_chunk: 
                    packet = Packet(seq_num=next_seq_num, data_type=TYPE_DATA, data=data_chunk)
                    # 序列化数据包为字节流
                    packet_bytes = packet.serialize()

                    with buffer_lock: # 锁定发送缓冲区
                        send_buffer[next_seq_num] = packet_bytes # 将数据包存入缓冲区

                    print(f"Sending packet {next_seq_num} (size: {len(data_chunk)} bytes)")
                    udt_send(packet_bytes, (RECEIVER_IP, RECEIVER_PORT))    # 模拟发送数据包

                    # 第一个包
                    if base == next_seq_num:
                        start_timer()

                    next_seq_num += 1
                # 如果文件已读完 (data_chunk 为空)，但窗口中还有未确认的包
                elif next_seq_num > base:
                    print("EOF reached, waiting for final ACKs...") # 打印文件结束，等待确认的信息
                    # 此时定时器应该已经在运行 (因为 base < next_seq_num)
                    # 可以选择等待一段时间，给 ACK 更多到达的时间
                    time.sleep(TIMEOUT_INTERVAL)


            # 文件发送完毕后，发送一个 FIN 包通知接收方结束
            print("Sending FIN packet") # 打印发送 FIN 的信息
            fin_packet = Packet(seq_num=next_seq_num, data_type=TYPE_FIN)
            udt_send(fin_packet.serialize(), (RECEIVER_IP, RECEIVER_PORT))

            # 等待 ACK 接收线程结束 (或超时)，以确保能收到可能的 FIN-ACK
            ack_listener.join(timeout=TIMEOUT_INTERVAL * 2)
            print("Sender finished.") # 打印发送方结束信息

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except Exception as e:
        print(f"An error occurred during sending: {e}")
    finally:
        stop_timer()
        sender_socket.close()

if __name__ == "__main__":
    file_to_send = "/Users/bananapig/Desktop/Lab3/input.txt"
    send_data(file_to_send)