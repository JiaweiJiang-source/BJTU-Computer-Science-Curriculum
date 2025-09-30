import mysql.connector
from mysql.connector import errorcode
from flask import Flask, request, jsonify, render_template
import re

app = Flask(__name__)

# --- 数据库配置 ---
DB_CONFIG = {
    'user': 'root',
    'password': 'Aa585891',  # 请确保这是您的MySQL密码
    'host': 'localhost',
    'database': 'AirlineDB',
    'raise_on_warnings': True
}

# --- 数据库连接辅助函数 ---
def get_db_connection():
    """建立到数据库的连接。"""
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        return conn
    except mysql.connector.Error as err:
        print(f"数据库连接错误: {err}")
        return None

# --- 解析重复条目错误的辅助函数 ---
def parse_duplicate_entry_error(error_msg):
    # 尝试提取违反约束的键名和值
    match = re.search(r"Duplicate entry '(.+?)' for key 'Passenger.(.+?)'", error_msg) # Passenger.后面是索引名
    if match:
        value = match.group(1) # 重复的值
        key_internal_name = match.group(2)   # 违反的键名 (例如 id_card_number, email, PRIMARY)
        
        # 将内部键名/索引名映射为用户友好的字段名
        key_map = {
            'id_card_number': '身份证号', # 假设唯一索引名为 id_card_number
            'email': '电子邮件',           # 假设唯一索引名为 email
            'frequent_flyer_number': '常旅客编号', # 假设唯一索引名为 frequent_flyer_number
            'PRIMARY': '乘客ID' # 主键冲突通常不会在添加时发生（如果ID自增）
        }
        friendly_key_name = key_map.get(key_internal_name, key_internal_name) # 如果映射中没有，则使用原始内部键名
        return f"{friendly_key_name} '{value}' 已存在。"
    return "信息重复，无法添加或更新。"

# --- 乘客相关的 API 端点 (直接SQL操作) ---
@app.route('/api/passengers', methods=['GET'])
def get_passengers():
    """获取所有乘客列表"""
    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor(dictionary=True)
    try:
        cursor.execute("SELECT passenger_id, name, id_card_number, phone_number, email, frequent_flyer_number FROM Passenger ORDER BY passenger_id")
        passengers = cursor.fetchall()
        return jsonify(passengers)
    except mysql.connector.Error as err:
        print(f"获取乘客列表时出错: {err}")
        return jsonify({"error": "获取乘客列表失败"}), 500
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/passengers/<int:passenger_id>', methods=['GET'])
def get_passenger(passenger_id):
    """根据 ID 获取单个乘客信息"""
    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor(dictionary=True)
    try:
        cursor.execute("SELECT * FROM Passenger WHERE passenger_id = %s", (passenger_id,))
        passenger = cursor.fetchone()
        if passenger:
            return jsonify(passenger)
        else:
            return jsonify({"error": "乘客未找到"}), 404
    except mysql.connector.Error as err:
         print(f"获取乘客 {passenger_id} 信息时出错: {err}")
         return jsonify({"error": "获取乘客信息失败"}), 500
    finally:
         if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/passengers', methods=['POST'])
def add_passenger():
    """添加新乘客 (现在passenger_id应为AUTO_INCREMENT)"""
    if not request.is_json:
        return jsonify({"error": "请求必须是 JSON 格式"}), 400
    data = request.get_json()
    required_fields = ['name', 'id_card_number', 'phone_number']
    if not all(field in data and data[field] for field in required_fields): # 确保必填字段存在且非空
        return jsonify({"error": "缺少必填项 (姓名, 身份证号, 联系电话)"}), 400

    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    # passenger_id 不再由应用插入，它将由数据库自动生成
    sql = """
        INSERT INTO Passenger (name, id_card_number, phone_number, email, frequent_flyer_number)
        VALUES (%s, %s, %s, %s, %s)
    """
    email_val = data.get('email') or None # 处理空字符串或None
    frequent_flyer_val = data.get('frequent_flyer_number') or None # 处理空字符串或None

    try:
        cursor.execute(sql, (
            data['name'],
            data['id_card_number'],
            data['phone_number'],
            email_val,
            frequent_flyer_val
        ))
        conn.commit()
        new_passenger_id = cursor.lastrowid # 获取自增ID
        return jsonify({"message": "乘客添加成功", "passenger_id": new_passenger_id}), 201
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"添加乘客时出错: {err} (错误码: {err.errno})")
        error_message = "添加乘客失败，发生数据库错误。"
        if err.errno == errorcode.ER_DUP_ENTRY:
            error_message = parse_duplicate_entry_error(err.msg)
            return jsonify({"error": error_message}), 409
        return jsonify({"error": error_message}), 500
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/passengers/<int:passenger_id>', methods=['PUT'])
def update_passenger(passenger_id):
    """更新指定 ID 的乘客信息"""
    if not request.is_json:
        return jsonify({"error": "请求必须是 JSON 格式"}), 400
    data = request.get_json()
    if not data: # 确保请求体不为空
         return jsonify({"error": "请求体不能为空"}), 400

    # 假设前端会发送所有相关字段，即使某些字段值未改变
    # 如果只发送部分字段，需要动态构建SQL语句
    name_val = data.get('name')
    id_card_val = data.get('id_card_number')
    phone_val = data.get('phone_number')
    email_val = data.get('email') # 允许为null或空字符串，数据库层面处理
    frequent_flyer_val = data.get('frequent_flyer_number')

    # 确保核心字段存在，因为前端表单是required的
    if name_val is None or id_card_val is None or phone_val is None:
        return jsonify({"error": "更新时，姓名、身份证号和电话号码为必填项。"}), 400


    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    sql = """
        UPDATE Passenger
        SET name = %s, id_card_number = %s, phone_number = %s, email = %s, frequent_flyer_number = %s
        WHERE passenger_id = %s
    """
    try:
        cursor.execute(sql, (
            name_val, id_card_val, phone_val, email_val, frequent_flyer_val, passenger_id
        ))
        if cursor.rowcount == 0: # 没有行被更新
             conn.rollback() # 可能是ID不存在或数据未改变
             return jsonify({"error": "乘客未找到或信息无变化"}), 404
        conn.commit()
        return jsonify({"message": "乘客更新成功"})
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"更新乘客 {passenger_id} 时出错: {err} (错误码: {err.errno})")
        error_message = "更新乘客失败，发生数据库错误。"
        if err.errno == errorcode.ER_DUP_ENTRY:
            error_message = parse_duplicate_entry_error(err.msg)
            return jsonify({"error": error_message}), 409
        return jsonify({"error": error_message}), 500
    finally:
         if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/passengers/<int:passenger_id>', methods=['DELETE'])
def delete_passenger(passenger_id):
    """删除指定 ID 的乘客"""
    conn = get_db_connection()
    if not conn:
       return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    try:
        # 先检查乘客是否存在，以便返回更明确的404
        cursor.execute("SELECT 1 FROM Passenger WHERE passenger_id = %s", (passenger_id,))
        if not cursor.fetchone():
            return jsonify({"error": "乘客未找到"}), 404

        cursor.execute("DELETE FROM Passenger WHERE passenger_id = %s", (passenger_id,))
        conn.commit()
        # rowcount 在DELETE后可能不是预期的，因为之前的SELECT已消耗结果
        # 通常DELETE成功不报错即可，或依赖外键约束错误
        return jsonify({"message": "乘客删除成功"})
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"删除乘客 {passenger_id} 时出错: {err} (错误码: {err.errno})")
        if err.errno == errorcode.ER_ROW_IS_REFERENCED_2 or err.errno == errorcode.ER_ROW_IS_REFERENCED:
             return jsonify({"error": "无法删除乘客，因为该乘客有关联的预订或机票。"}), 409
        return jsonify({"error": "删除乘客失败，发生数据库错误。"}), 500
    finally:
         if conn and conn.is_connected():
            cursor.close()
            conn.close()

# --- Lab 7: API Endpoints for Stored Procedures ---

@app.route('/api/sp/passengers/<int:passenger_id>/details', methods=['GET'])
def get_passenger_details_sp(passenger_id):
    """通过存储过程获取乘客及其预订详情"""
    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor(dictionary=True)
    try:
        cursor.callproc('GetPassengerAndBookings', (passenger_id,))
        results = []
        for result in cursor.stored_results(): # 存储过程可能返回多个结果集
            results.extend(result.fetchall())
        
        if not results:
            cursor.execute("SELECT 1 FROM Passenger WHERE passenger_id = %s", (passenger_id,)) # 重新获取游标
            if not cursor.fetchone():
                 return jsonify({"error": "乘客未找到 (SP)"}), 404
            return jsonify({"message": "乘客存在，但没有预订信息 (SP)。", "details": []})
        return jsonify(results)
    except mysql.connector.Error as err:
        print(f"调用 GetPassengerAndBookings 存储过程时出错: {err}")
        return jsonify({"error": "获取乘客详情失败 (SP)"}), 500
    finally:
        if conn and conn.is_connected():
            # 确保在读取完所有存储过程结果后关闭游标
            # 对于callproc，通常在连接关闭时游标也会被清理
            # 但显式关闭是个好习惯，如果后续还有操作的话
            while conn.next_result(): # 确保所有结果集都被处理
                pass
            cursor.close()
            conn.close()

@app.route('/api/sp/passengers', methods=['POST'])
def add_passenger_sp():
    """通过存储过程添加新乘客 (SP应处理passenger_id自增)"""
    if not request.is_json:
        return jsonify({"error": "请求必须是 JSON 格式"}), 400
    data = request.get_json()
    required_fields = ['name', 'id_card_number', 'phone_number']
    if not all(field in data and data[field] for field in required_fields):
        return jsonify({"error": "缺少必填项 (姓名, 身份证号, 联系电话)"}), 400

    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    try:
        args = (
            data['name'],
            data['id_card_number'],
            data['phone_number'],
            data.get('email') or None,
            data.get('frequent_flyer_number') or None,
            0  # Placeholder for OUT parameter p_new_passenger_id
        )
        result_args = cursor.callproc('AddPassengerViaSP', args)
        conn.commit()
        new_passenger_id = result_args[5] # 获取OUT参数
        return jsonify({"message": "乘客通过存储过程添加成功", "passenger_id": new_passenger_id}), 201
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"调用 AddPassengerViaSP 存储过程时出错: {err} (错误码: {err.errno})")
        error_message = "通过存储过程添加乘客失败。"
        if err.errno == errorcode.ER_DUP_ENTRY:
            error_message = parse_duplicate_entry_error(err.msg) if hasattr(err, 'msg') else "信息重复 (SP)。"
            return jsonify({"error": error_message}), 409
        return jsonify({"error": error_message + f" 数据库错误: {err.msg if hasattr(err, 'msg') else str(err)}" }), 500
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/sp/passengers/<int:passenger_id>', methods=['DELETE'])
def delete_passenger_sp(passenger_id):
    """通过存储过程删除乘客"""
    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    try:
        cursor.execute("SELECT 1 FROM Passenger WHERE passenger_id = %s", (passenger_id,))
        if not cursor.fetchone():
            return jsonify({"error": "乘客未找到 (SP)"}), 404

        cursor.callproc('DeletePassengerViaSP', (passenger_id,))
        conn.commit()
        return jsonify({"message": "乘客通过存储过程删除成功"})
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"调用 DeletePassengerViaSP 存储过程时出错: {err}")
        if err.errno == errorcode.ER_ROW_IS_REFERENCED_2 or err.errno == errorcode.ER_ROW_IS_REFERENCED:
             return jsonify({"error": "无法通过存储过程删除乘客，因为该乘客有关联数据。"}), 409
        return jsonify({"error": "通过存储过程删除乘客失败"}), 500
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

@app.route('/api/sp/passengers/<int:passenger_id>/email', methods=['PUT'])
def update_passenger_email_sp(passenger_id):
    """通过存储过程更新乘客邮箱"""
    if not request.is_json:
        return jsonify({"error": "请求必须是 JSON 格式"}), 400
    data = request.get_json()
    new_email = data.get('email')

    if new_email is None : # 如果 'email' 键不存在于JSON中
        return jsonify({"error": "新邮箱未在请求中提供"}), 400
    # 如果 'email' 键存在但值为空字符串，这是允许的，表示清空邮箱

    conn = get_db_connection()
    if not conn:
        return jsonify({"error": "数据库连接失败"}), 500
    cursor = conn.cursor()
    try:
        cursor.execute("SELECT 1 FROM Passenger WHERE passenger_id = %s", (passenger_id,))
        if not cursor.fetchone():
            return jsonify({"error": "乘客未找到 (SP)"}), 404
            
        cursor.callproc('UpdatePassengerEmailViaSP', (passenger_id, new_email if new_email != "" else None)) # 如果为空字符串则传NULL
        conn.commit()
        return jsonify({"message": "乘客邮箱通过存储过程更新成功"})
    except mysql.connector.Error as err:
        conn.rollback()
        print(f"调用 UpdatePassengerEmailViaSP 存储过程时出错: {err} (错误码: {err.errno})")
        error_message = "通过存储过程更新乘客邮箱失败。"
        if err.errno == errorcode.ER_DUP_ENTRY:
            error_message = f"邮箱 '{new_email}' 已被其他乘客使用 (SP)。"
            return jsonify({"error": error_message}), 409
        return jsonify({"error": error_message + f" 数据库错误: {err.msg if hasattr(err, 'msg') else str(err)}"}), 500
    finally:
        if conn and conn.is_connected():
            cursor.close()
            conn.close()

# --- 用于提供 HTML 页面的路由 ---
@app.route('/')
def index():
    """提供主要的乘客管理 HTML 页面。"""
    return render_template('passengers.html')

# --- 运行 Flask 应用 ---
if __name__ == '__main__':
    app.run(debug=True, port=5000)
