import pickle # 导入用于序列化和反序列化 Python 对象的库
import hashlib # 导入用于计算哈希值（这里用作校验和）的库

# 定义数据包类型常量
TYPE_DATA = 0 # 数据包
TYPE_ACK = 1  # 确认 (ACK) 包
TYPE_FIN = 2  # 结束传输 (FIN) 包

# 定义 Packet 类，用于封装网络传输的数据单元
class Packet:
    # 初始化方法 (构造函数)
    # seq_num: 数据包的序列号
    # data_type: 数据包的类型 (使用上面定义的常量)
    # data: 数据包实际承载的数据 (默认为空字节串)
    def __init__(self, seq_num, data_type, data=b''):
        self.seq_num = seq_num       # 序列号
        self.data_type = data_type   # 包类型 (DATA, ACK, FIN)
        self.data = data             # 数据负载
        self.checksum = self._calculate_checksum() # 计算并存储校验和

    # 内部方法，用于计算数据包的校验和
    def _calculate_checksum(self):
        # 使用 MD5 算法作为校验和示例 (注意：MD5 主要用于验证完整性，不是严格意义上的错误检测码)
        # 实际应用中可能需要更适合网络传输的校验和算法，例如 Fletcher's checksum 或 CRC
        chksum = hashlib.md5()
        # 将序列号、类型和数据整合，并更新到 MD5 对象中
        # 注意：需要将数字转换为字符串再编码为字节串
        chksum.update(str(self.seq_num).encode())
        chksum.update(str(self.data_type).encode())
        chksum.update(self.data)
        # 返回 MD5 的十六进制摘要作为校验和
        return chksum.hexdigest() # 也可以使用整数形式的校验和

    # 验证接收到的数据包的校验和是否正确
    def verify_checksum(self):
        # 重新计算当前数据包内容的校验和，与存储的 checksum 字段比较
        return self.checksum == self._calculate_checksum()

    # 将 Packet 对象序列化为字节流，以便通过网络发送
    def serialize(self):
        # 使用 pickle 库的 dumps 方法将对象转换为字节串
        return pickle.dumps(self)

    # 静态方法，用于将接收到的字节流反序列化为 Packet 对象
    @staticmethod
    def deserialize(raw_bytes):
        try:
            # 使用 pickle 库的 loads 方法将字节串转换回对象
            return pickle.loads(raw_bytes)
        except (pickle.UnpicklingError, EOFError):
            # 如果反序列化过程中出错 (例如数据损坏或不完整)，则返回 None
            return None