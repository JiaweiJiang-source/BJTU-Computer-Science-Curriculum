import requests

# 设置代理
proxies = {
    "http": "http://192.168.0.105:8080",
    "https": "http://192.168.0.105:8080",
}

# 发送请求
response = requests.get("http://example.com", proxies=proxies)

print(response.text)
