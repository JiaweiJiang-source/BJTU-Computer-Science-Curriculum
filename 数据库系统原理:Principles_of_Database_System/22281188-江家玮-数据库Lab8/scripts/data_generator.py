# scripts/data_generator.py
# 数据生成辅助脚本

import random
import string
from faker import Faker # pip install Faker

fake = Faker('zh_CN') 

def generate_passenger_data(passenger_id):
    """生成单条乘客数据"""
    name = fake.name()
    # 生成一个相对唯一的身份证号 (Faker的ssn可能不够唯一，这里简单模拟)
    id_card_start = random.choice(['110', '310', '440', '510'])
    id_card_number = id_card_start + "".join(random.choices(string.digits, k=15))
    
    phone_number = fake.phone_number()
    email = fake.email()
    frequent_flyer_number = None
    if random.random() < 0.3: # 30%的乘客有常旅客编号
        frequent_flyer_number = random.choice(['CA', 'MU', 'CZ', 'HU']) + "".join(random.choices(string.digits, k=6))
        
    return (passenger_id, name, id_card_number, phone_number, email, frequent_flyer_number)

def generate_batch_passenger_data(start_id, batch_size):
    """生成一批乘客数据"""
    batch_data = []
    for i in range(batch_size):
        batch_data.append(generate_passenger_data(start_id + i))
    return batch_data

if __name__ == '__main__':
    # 测试生成数据
    print("--- 单条乘客数据示例 ---")
    print(generate_passenger_data(1))
    
    print("\n--- 批量乘客数据示例 (5条) ---")
    batch = generate_batch_passenger_data(100, 5)
    for record in batch:
        print(record)
