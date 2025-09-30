from socket import *
import base64
import ssl

msg = "\r\nHELLO WORLD!\r\nGood Luck To You!\r\n\r\n"
endMsg = "From: 22281188@Jiawei Jiang, Beijing Jiaotong University\r\n.\r\n"
mailServer = "smtp.qq.com"
fromAddress = "376234982@qq.com"  # 发送方邮箱
toAddress = "22281188@bjtu.edu.cn"  # 接收方邮箱

username = "376234982@qq.com"  # 发送方邮箱
password = "rrrzyxfadifzbjji"  # 应用专用密码
encoded_username = base64.b64encode(username.encode()).decode()
encoded_password = base64.b64encode(password.encode()).decode()

# 创建客户端套接字并建立连接
serverPort = 587  # SMTP使用587端口（启用TLS）
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((mailServer, serverPort))

# 从客户套接字中接收信息
recv = clientSocket.recv(1024).decode()
print("Server Response:", recv)
if '220' != recv[:3]:
    print('220 reply not received from server.')

# 发送 EHLO 命令并且打印服务端回复
heloCommand = 'EHLO Alice\r\n'
clientSocket.send(heloCommand.encode())  
recv1 = clientSocket.recv(1024).decode()
print("EHLO Response:", recv1)
if '250' != recv1[:3]:
    print('250 reply not received from server.')

# 发送"STARTTLS"命令以启用加密
clientSocket.sendall('STARTTLS\r\n'.encode())
recv2 = clientSocket.recv(1024).decode()
print("STARTTLS Response:", recv2)
if '220' != recv2[:3]:
    print('220 reply not received from server.')

# 启用TLS加密
context = ssl.create_default_context()
clientSocket = context.wrap_socket(clientSocket, server_hostname=mailServer)

# 重新发送 EHLO 命令
clientSocket.send(heloCommand.encode())
recv1 = clientSocket.recv(1024).decode()
print("EHLO Response after TLS:", recv1)
if '250' != recv1[:3]:
    print('250 reply not received from server.')

# 发送 AUTH LOGIN 命令开始身份验证
clientSocket.sendall('AUTH LOGIN\r\n'.encode())
recv3 = clientSocket.recv(1024).decode()
print("AUTH LOGIN Response:", recv3)
if '334' != recv3[:3]:
    print('334 reply not received from server.')

# 发送 Base64 编码的用户名
clientSocket.sendall((encoded_username + '\r\n').encode())
recvName = clientSocket.recv(1024).decode()
print("Username Response:", recvName)
if '334' != recvName[:3]:
    print('334 reply not received from server')

# 发送 Base64 编码的密码
clientSocket.sendall((encoded_password + '\r\n').encode())
recvPass = clientSocket.recv(1024).decode()
print("Password Response:", recvPass)
# 如果用户验证成功，服务器将返回状态码235
if '235' != recvPass[:3]:
    print('235 reply not received from server')

# 发送 MAIL FROM 命令，并包含发件人邮箱地址
clientSocket.sendall(('MAIL FROM: <' + fromAddress + '>\r\n').encode())
recvFrom = clientSocket.recv(1024).decode()
print("MAIL FROM Response:", recvFrom)
if '250' != recvFrom[:3]:
    print('250 reply not received from server')

# 发送 RCPT TO 命令，并包含收件人邮箱地址
clientSocket.sendall(('RCPT TO: <' + toAddress + '>\r\n').encode())
recvTo = clientSocket.recv(1024).decode()  
print("RCPT TO Response:", recvTo)
if '250' != recvTo[:3]:
    print('250 reply not received from server')

# 发送 DATA 命令，表示即将发送邮件内容。服务器将返回状态码354（开始邮件输入，以"."结束）
clientSocket.send('DATA\r\n'.encode())
recvData = clientSocket.recv(1024).decode()
print("DATA Response:", recvData)
if '354' != recvData[:3]:
    print('354 reply not received from server')

# 编辑邮件信息，发送数据
subject = "Test Email"
contentType = "text/plain"

message = 'From:' + fromAddress + '\r\n'
message += 'To:' + toAddress + '\r\n'
message += 'Subject:' + subject + '\r\n'
message += 'Content-Type:' + contentType + '\r\n'
message += '\r\n' + msg
clientSocket.sendall(message.encode())

# 以"."结束。请求成功返回 250
clientSocket.sendall(endMsg.encode())
recvEnd = clientSocket.recv(1024).decode()
print("End Message Response:", recvEnd)
if '250' != recvEnd[:3]:
    print('250 reply not received from server')

# 发送"QUIT"命令，断开和邮件服务器的连接
clientSocket.sendall('QUIT\r\n'.encode())
clientSocket.close()