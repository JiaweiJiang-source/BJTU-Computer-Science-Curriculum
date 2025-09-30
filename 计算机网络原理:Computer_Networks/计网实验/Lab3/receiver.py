import socket
# 导入 packet 模块中的 Packet 类和类型常量
from packet import Packet, TYPE_DATA, TYPE_ACK, TYPE_FIN 

RECEIVER_IP = '0.0.0.0' 
RECEIVER_PORT = 8001 
OUTPUT_FILE = "output.txt" 

# --- 初始化 ---
expected_seq_num = 0 
last_ack_packet_bytes = None # 需要时重发

# --- UDP 套接字 ---
receiver_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # 创建 UDP 套接字
receiver_socket.bind((RECEIVER_IP, RECEIVER_PORT)) # 绑定 IP 地址和端口
print(f"Receiver listening on port {RECEIVER_PORT}...") 

# --- 主接收逻辑 ---
try:
    with open(OUTPUT_FILE, 'wb') as f:
        while True: 
            try:
                # 从套接字接收数据，最大 2048 字节，同时获取发送方地址
                packet_bytes, sender_addr = receiver_socket.recvfrom(2048) 
                packet = Packet.deserialize(packet_bytes)

                # 检查反序列化是否成功
                if not packet:
                    print("Received potentially corrupted/incomplete data.") 
                    # 如果之前发送过 ACK，则选择性地重发最后的 ACK
                    if last_ack_packet_bytes:
                         receiver_socket.sendto(last_ack_packet_bytes, sender_addr)
                    continue 

                # 首先检查是否是 FIN 数据包 (结束传输信号)
                if packet.data_type == TYPE_FIN:
                    print(f"Received FIN packet {packet.seq_num} from {sender_addr}. Sending FIN-ACK and exiting.") # 打印收到 FIN 的信息
                    # 创建一个 FIN-ACK 数据包来确认收到 FIN
                    fin_ack = Packet(seq_num=packet.seq_num, data_type=TYPE_FIN) 
                    # 发送 FIN-ACK 给发送方
                    receiver_socket.sendto(fin_ack.serialize(), sender_addr)
                    break 

                # 处理数据 (DATA) 数据包
                if packet.data_type == TYPE_DATA:
                     print(f"Received packet {packet.seq_num}. Expecting {expected_seq_num}.") # 打印收到数据包的序列号和期望的序列号

                     # 验证数据包的校验和是否正确
                     if packet.verify_checksum():
                         # 检查收到的数据包序列号是否是期望的序列号
                         if packet.seq_num == expected_seq_num:
                             print(f"Packet {packet.seq_num} accepted. Delivering data.") # 打印接受数据包并准备交付的信息
                             # 将数据包中的数据写入输出文件
                             f.write(packet.data)
                             # 准备并发送当前数据包的 ACK
                             ack_packet = Packet(seq_num=expected_seq_num, data_type=TYPE_ACK)
                             # 保存这个 ACK 的字节流，以便后面可能需要重发
                             last_ack_packet_bytes = ack_packet.serialize() # Store for potential resend
                             # 发送 ACK 给发送方
                             receiver_socket.sendto(last_ack_packet_bytes, sender_addr)
                             print(f"Sent ACK {expected_seq_num}.") # 打印已发送 ACK 的信息
                             # 期望的序列号加 1
                             expected_seq_num += 1
                         else:
                             # 如果收到的包不是期望序号 (失序或重复)
                             print(f"Packet {packet.seq_num} discarded (out of order/duplicate). Resending ACK {expected_seq_num - 1}.") # 打印丢弃数据包并重发 ACK 的信息
                             # 重新发送上一个成功接收并确认的 ACK
                             # 只有在至少收到过一个包 (expected_seq_num > 0) 时才重发 ACK
                             if expected_seq_num > 0:
                                 ack_packet = Packet(seq_num=expected_seq_num - 1, data_type=TYPE_ACK)
                                 receiver_socket.sendto(ack_packet.serialize(), sender_addr)
                             # 如果 expected_seq_num 为 0，可以考虑发送 ACK -1 或不发送，取决于具体协议设计
                     else:
                         # 如果校验和错误
                         print(f"Packet {packet.seq_num} discarded (checksum error).") # 打印因校验和错误丢弃数据包的信息
                         # (可选) 重新发送上一个成功接收的 ACK，以提示发送方
                         if expected_seq_num > 0 and last_ack_packet_bytes:
                             print(f"Resending ACK {expected_seq_num - 1} due to corrupted packet.") # 打印因校验和错误重发 ACK 的信息
                             receiver_socket.sendto(last_ack_packet_bytes, sender_addr)

            except Exception as e:
                # 捕获处理单个数据包时可能出现的异常
                print(f"Error processing packet: {e}")

except Exception as e:
    # 捕获接收过程中的其他异常 (例如文件打开失败)
    print(f"An error occurred in the receiver: {e}")
finally:
    # 无论是否发生异常，最后都关闭套接字
    receiver_socket.close()
    print("Receiver finished.") # 打印接收方结束运行的信息