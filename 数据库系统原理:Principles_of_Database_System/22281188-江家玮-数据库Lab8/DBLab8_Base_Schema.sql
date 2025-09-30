-- DBLab8_Base_Schema.sql
-- 基础表结构脚本 (源自Lab3的 DBLab3_22281188.sql)
-- 确保在此基础上进行实验

-- CREATE DATABASE IF NOT EXISTS AirlineDB CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
-- USE AirlineDB;

-- 按依赖反序删除表 (如果存在，用于重置)
DROP TABLE IF EXISTS FlightStatus;
DROP TABLE IF EXISTS Ticket;
DROP TABLE IF EXISTS Payment;
DROP TABLE IF EXISTS Booking;
DROP TABLE IF EXISTS Flight;
DROP TABLE IF EXISTS Route;
DROP TABLE IF EXISTS Passenger;
DROP TABLE IF EXISTS Airport;
DROP TABLE IF EXISTS Airline;

-- ============================================================
-- Table: Airline (航空公司)
-- ============================================================
CREATE TABLE Airline (
    airline_id INT PRIMARY KEY NOT NULL COMMENT '航空公司ID',
    name VARCHAR(100) NOT NULL COMMENT '名称',
    hq_location VARCHAR(255) COMMENT '总部位置',
    contact_info VARCHAR(50) COMMENT '联系方式',
    route_count INT DEFAULT 0 COMMENT '航线数量'
) COMMENT='航空公司信息表';

-- ============================================================
-- Table: Airport (机场)
-- ============================================================
CREATE TABLE Airport (
    airport_id INT PRIMARY KEY NOT NULL COMMENT '机场ID',
    airport_name VARCHAR(100) NOT NULL COMMENT '机场名称',
    location VARCHAR(255) NOT NULL COMMENT '机场位置',
    flight_count INT DEFAULT 0 COMMENT '航班数量 (可能为动态数据)'
) COMMENT='机场信息表';

-- ============================================================
-- Table: Passenger (乘客) - 这是我们主要测试的表
-- ============================================================
-- DBLab8_Base_Schema.sql (部分修改)

CREATE TABLE Passenger (
    passenger_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '乘客ID (设为自增，方便Python脚本插入)',
    name VARCHAR(100) NOT NULL COMMENT '姓名 (属性A)',
    id_card_number VARCHAR(18) NOT NULL COMMENT '身份证号', -- 移除了 UNIQUE
    phone_number VARCHAR(20) NOT NULL COMMENT '联系电话',
    email VARCHAR(100) COMMENT '电子邮件', -- 移除了 UNIQUE
    frequent_flyer_number VARCHAR(50) COMMENT '常旅客编号' -- 移除了 UNIQUE
) COMMENT='乘客信息表';

-- ... (其他表定义和索引不变) ...
-- 注意: passenger_id 改为 AUTO_INCREMENT 以简化Python插入逻辑。
-- 如果Lab3中 passenger_id 不是自增，Python脚本中需要手动管理ID的唯一性。

-- ============================================================
-- Table: Route (航线)
-- ============================================================
CREATE TABLE Route (
    route_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '航线ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    origin VARCHAR(100) NOT NULL COMMENT '起点',
    destination VARCHAR(100) NOT NULL COMMENT '终点',
    duration VARCHAR(20) COMMENT '飞行时长 (e.g., 2h 30m)',
    FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='航线信息表';

-- ============================================================
-- Table: Flight (航班)
-- ============================================================
CREATE TABLE Flight (
    flight_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '航班ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    route_id INT NOT NULL COMMENT '航线ID (外键)',
    departure_time DATETIME NOT NULL COMMENT '起飞时间',
    arrival_time DATETIME NOT NULL COMMENT '抵达时间',
    departure_location VARCHAR(100) NOT NULL COMMENT '起飞地点',
    destination_location VARCHAR(100) NOT NULL COMMENT '目的地',
    total_seats INT NOT NULL COMMENT '座位总数',
    booked_seats INT DEFAULT 0 COMMENT '已预订座位数',
    aircraft_model VARCHAR(50) COMMENT '机型',
    FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (route_id) REFERENCES Route(route_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='航班信息表';

-- ============================================================
-- Table: Booking (预订)
-- ============================================================
CREATE TABLE Booking (
    booking_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '预订ID',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    seat_type VARCHAR(20) COMMENT '座位类型 (e.g., Economy, Business)',
    booking_date DATETIME NOT NULL COMMENT '预订日期',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Pending' COMMENT '支付状态 (e.g., Pending, Paid, Cancelled)',
    FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='预订信息表';

-- ============================================================
-- Table: Payment (支付)
-- ============================================================
CREATE TABLE Payment (
    payment_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '支付ID',
    booking_id INT NOT NULL COMMENT '预订ID (外键)',
    payment_method VARCHAR(50) NOT NULL COMMENT '支付方式 (e.g., Credit Card, Alipay)',
    payment_amount DECIMAL(10, 2) NOT NULL COMMENT '支付金额',
    payment_time DATETIME NOT NULL COMMENT '支付时间',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Success' COMMENT '支付状态 (e.g., Success, Failed)',
    FOREIGN KEY (booking_id) REFERENCES Booking(booking_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='支付信息表';

-- ============================================================
-- Table: Ticket (机票)
-- ============================================================
CREATE TABLE Ticket (
    ticket_id INT PRIMARY KEY NOT NULL AUTO_INCREMENT COMMENT '机票ID',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    ticket_number VARCHAR(50) NOT NULL UNIQUE COMMENT '票号',
    seat_number VARCHAR(10) NOT NULL COMMENT '座位号',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价 (可能冗余, 可从Booking获取)',
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='机票信息表';

-- ============================================================
-- Table: FlightStatus (航班状态)
-- ============================================================
CREATE TABLE FlightStatus (
    flight_id INT PRIMARY KEY NOT NULL COMMENT '航班ID (主键/外键, 指向Flight表)',
    status VARCHAR(50) NOT NULL COMMENT '状态 (e.g., On Time, Delayed, Cancelled)',
    update_time DATETIME NOT NULL COMMENT '更新时间',
    delay_reason TEXT COMMENT '延误原因',
    cancellation_reason TEXT COMMENT '取消原因',
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE CASCADE ON UPDATE CASCADE
) COMMENT='航班状态表 (通常存储当前状态)';

-- 添加 Lab3 中已有的其他索引 (如果 Passenger.name 不是被这些索引覆盖的话)
-- 根据 DBLab3_22281188.sql，Passenger 表除了主键和UNIQUE约束外，还对 phone_number 加了索引。
-- name 列在初始状态下是没有单独索引的，适合作为属性A。
ALTER TABLE Route ADD INDEX idx_route_airline (airline_id);
ALTER TABLE Flight ADD INDEX idx_flight_airline (airline_id);
ALTER TABLE Flight ADD INDEX idx_flight_route (route_id);
ALTER TABLE Flight ADD INDEX idx_flight_departure_time (departure_time);
ALTER TABLE Booking ADD INDEX idx_booking_passenger (passenger_id);
ALTER TABLE Booking ADD INDEX idx_booking_flight (flight_id);
ALTER TABLE Payment ADD INDEX idx_payment_booking (booking_id);
ALTER TABLE Ticket ADD INDEX idx_ticket_flight (flight_id);
ALTER TABLE Ticket ADD INDEX idx_ticket_passenger (passenger_id);
ALTER TABLE Passenger ADD INDEX idx_passenger_phone (phone_number);

-- 如果Lab3中已有对 Passenger.name 的索引，请在此处注释掉或在无索引测试前手动DROP
-- SHOW INDEX FROM Passenger; -- 可以用来检查索引情况
