import socket
import os

def handle_request(client_connection):
    request = client_connection.recv(1024).decode()
    print("Received request:\n", request)
    
    if not request:
        client_connection.close()
        return
    
    request_line = request.split('\n')[0]
    request_file = request_line.split(' ')[1]
    
    if request_file == "/":
        request_file = "/index.html"
    
    file_path = "." + request_file
    
    if os.path.exists(file_path) and os.path.isfile(file_path):
        with open(file_path, "rb") as f:
            response_body = f.read()
        response_header = "HTTP/1.1 200 OK\n" + "Content-Length: " + str(len(response_body)) + "\n\n"
    else:
        response_body = b"<h1>404 Not Found</h1>"
        response_header = "HTTP/1.1 404 Not Found\n" + "Content-Length: " + str(len(response_body)) + "\n\n"
    
    client_connection.sendall(response_header.encode() + response_body)
    client_connection.close()

def start_server(host='0.0.0.0', port=8080):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(5)
    print(f"Server running on {host}:{port}")
    
    while True:
        client_connection, client_address = server_socket.accept()
        handle_request(client_connection)

if __name__ == "__main__":
    start_server("192.168.226.1", 8080)