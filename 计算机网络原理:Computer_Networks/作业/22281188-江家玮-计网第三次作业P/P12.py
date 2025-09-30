import socket

# 设置服务器地址和端口
HOST = '192.168.0.105'  # 服务器地址为本地地址
PORT = 8080  # 可以自定义端口，确保浏览器配置时使用相同端口

# 创建一个TCP套接字
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定套接字到地址和端口
server_socket.bind((HOST, PORT))

# 开始监听连接
server_socket.listen(1)
print(f"服务器启动，正在监听 {HOST}:{PORT}...")

while True:
    # 接受客户端的连接请求
    client_socket, client_address = server_socket.accept()
    print(f"客户端 {client_address} 已连接！")

    try:
        # 接收客户端发送的数据
        request = client_socket.recv(1024).decode('utf-8')
        print(f"接收到的请求：\n{request}")

        # 发送HTTP响应
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!"
        client_socket.sendall(response.encode('utf-8'))

    except Exception as e:
        print(f"发生错误：{e}")
    finally:
        # 关闭与客户端的连接
        client_socket.close()
