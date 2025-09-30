-- ============================================================
-- DBLab3_22281188.sql
-- 航空公司数据库模式创建脚本
-- 目标平台: 航空公司数据库示例数据插入脚本
-- 作者: [江家玮/北京交通大学]  -- 请确认作者信息是否需要保留或修改
-- 日期: 2025-04-01
-- ============================================================

-- 确保正在使用正确的数据库
SET NAMES 'utf8mb4';
USE AirlineDB;

-- 清除现有数据
DELETE FROM FlightStatus;
DELETE FROM Ticket;
DELETE FROM Payment;
DELETE FROM Booking;
DELETE FROM Flight;
DELETE FROM Route;
DELETE FROM Passenger;
DELETE FROM Airport;
DELETE FROM Airline;


-- 插入航空公司 (父表)
INSERT INTO Airline (airline_id, name, hq_location, contact_info, route_count) VALUES
(1, 'China Eastern Airlines', 'Shanghai', '95530', 0), -- 替换了 '东方航空', '上海'
(2, 'China Southern Airlines', 'Guangzhou', '95539', 0); -- 替换了 '南方航空', '广州'

-- 插入机场
INSERT INTO Airport (airport_id, airport_name, location, flight_count) VALUES
(101, 'Shanghai Hongqiao International Airport', 'Shanghai, China', 0), -- 替换了中文名称/位置
(102, 'Guangzhou Baiyun International Airport', 'Guangzhou, China', 0),
(103, 'Beijing Capital International Airport', 'Beijing, China', 0);

-- 插入乘客
INSERT INTO Passenger (passenger_id, name, id_card_number, phone_number, email, frequent_flyer_number) VALUES
(1001, 'Jiang Jiawei', '310101199001011234', '13800138000', 'byjiaweijiang@gmail.com', 'MU123456'), -- 使用了拼音，修正了逗号拼写错误
(1002, 'Xiao Jiang', '440101199202022345', '13900139000', '22281188@bjtu.edu.cn', 'CZ654321'), -- 使用了拼音别名
(1003, 'Da Jiang', '110101198803033456', '13700137000', 'jiangpig0130@gmail.com', NULL); -- 使用了拼音别名

-- 插入航线 (航空公司的子表)
INSERT INTO Route (route_id, airline_id, origin, destination, duration) VALUES
(201, 1, 'Shanghai Hongqiao International Airport', 'Beijing Capital International Airport', '2h 10m'), -- 东航 上海->北京
(202, 2, 'Guangzhou Baiyun International Airport', 'Shanghai Hongqiao International Airport', '2h 00m'); -- 南航 广州->上海

-- 插入航班 (航空公司和航线的子表)
-- 确保起飞/到达地点与航线匹配，且ID存在
INSERT INTO Flight (flight_id, airline_id, route_id, departure_time, arrival_time, departure_location, destination_location, total_seats, aircraft_model) VALUES
(3001, 1, 201, '2025-07-15 08:00:00', '2025-07-15 10:10:00', 'Shanghai Hongqiao International Airport', 'Beijing Capital International Airport', 180, 'A320'),
(3002, 2, 202, '2025-07-15 09:30:00', '2025-07-15 11:30:00', 'Guangzhou Baiyun International Airport', 'Shanghai Hongqiao International Airport', 220, 'B737');

-- 插入预订 (乘客和航班的子表)
INSERT INTO Booking (booking_id, passenger_id, flight_id, seat_type, booking_date, price, payment_status) VALUES
(4001, 1001, 3001, 'Economy', '2025-06-10 14:30:00', 750.00, 'Paid'), -- 江家玮预订了航班3001
(4002, 1002, 3002, 'Business', '2025-06-11 10:00:00', 1800.00, 'Paid'); -- 小江江预订了航班3002
-- (添加一个状态为'Pending'的预订以测试默认值/后续更新)
INSERT INTO Booking (booking_id, passenger_id, flight_id, seat_type, booking_date, price) VALUES
(4003, 1003, 3001, 'Economy', '2025-06-12 11:00:00', 780.00); -- 大江江预订了航班3001, 状态默认为'Pending'

-- 插入支付 (预订的子表)
-- 确保 booking_id 存在且与预订详情匹配
INSERT INTO Payment (payment_id, booking_id, payment_method, payment_amount, payment_time, payment_status) VALUES
(5001, 4001, 'Alipay', 750.00, '2025-06-10 14:35:00', 'Success'), -- 对应江家玮的预订
(5002, 4002, 'Credit Card', 1800.00, '2025-06-11 10:05:00', 'Success'); -- 对应小江江的预订

-- 插入机票 (航班和乘客的子表)
-- 确保 flight_id, passenger_id 存在且与预订/支付详情匹配
INSERT INTO Ticket (ticket_id, flight_id, ticket_number, seat_number, passenger_id, price) VALUES
(6001, 3001, 'TKT-MU-12345', '15A', 1001, 750.00), -- 江家玮的机票
(6002, 3002, 'TKT-CZ-67890', '03B', 1002, 1800.00); -- 小江江的机票

-- 插入航班状态 (航班的子表)
-- 确保 flight_id 存在
INSERT INTO FlightStatus (flight_id, status, update_time, delay_reason, cancellation_reason) VALUES
(3001, 'On Time', '2025-07-15 07:00:00', NULL, NULL),
(3002, 'On Time', '2025-07-15 08:30:00', NULL, NULL);

-- ============================================================
-- 测试数据完整性约束

-- 参照完整性测试
-- 尝试插入一个不存在的乘客 (passenger_id=9999) 的预订
-- INSERT INTO Booking (booking_id, passenger_id, flight_id, seat_type, booking_date, price) VALUES
-- (4999, 9999, 3001, 'Economy', '2025-06-13 10:00:00', 700.00);

-- 唯一约束测试
-- 尝试插入一个具有现有身份证号的乘客
-- INSERT INTO Passenger (passenger_id, name, id_card_number, phone_number, email) VALUES
-- (1004, 'Zhao Liu', '310101199001011234', '13600136000', 'zhaoliu@example.com'); -- 使用了拼音

-- 失败 (NOT NULL 约束测试)
-- 尝试插入一个姓名为NULL的乘客
-- INSERT INTO Passenger (passenger_id, name, id_card_number, phone_number, email) VALUES
-- (1005, NULL, '123456789012345678', '13500135000', 'nullname@example.com');

-- 失败 (域/数据类型测试)
-- 尝试向价格插入非数值
-- INSERT INTO Booking (booking_id, passenger_id, flight_id, seat_type, booking_date, price) VALUES
-- (4998, 1003, 3002, 'Economy', '2025-06-14 09:00:00', 'Expensive');