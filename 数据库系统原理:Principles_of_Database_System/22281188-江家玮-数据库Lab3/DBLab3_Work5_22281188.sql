-- ============================================================
-- DBLab3_22281188.sql
-- 航空公司数据库模式创建脚本
-- 目标平台: 通用 SQL (主要基于 MySQL 语法测试)
-- 作者: [江家玮/北京交通大学]  -- 请确认作者信息是否需要保留或修改
-- 日期: 2025-04-01
-- ============================================================

-- 选: 如果数据库不存在则创建
-- CREATE DATABASE IF NOT EXISTS AirlineDB CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
-- USE AirlineDB;

-- 按依赖反序删除表 (如果存在)
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
-- 表: Airline (航空公司)
-- ============================================================
CREATE TABLE Airline (
    airline_id INT PRIMARY KEY NOT NULL COMMENT '航空公司ID',
    name VARCHAR(100) NOT NULL COMMENT '名称',
    hq_location VARCHAR(255) COMMENT '总部位置',
    contact_info VARCHAR(50) COMMENT '联系方式',
    route_count INT DEFAULT 0 COMMENT '航线数量'
) COMMENT='航空公司信息表';

-- ============================================================
-- 表: Airport (机场)
-- ============================================================
CREATE TABLE Airport (
    airport_id INT PRIMARY KEY NOT NULL COMMENT '机场ID',
    airport_name VARCHAR(100) NOT NULL COMMENT '机场名称',
    location VARCHAR(255) NOT NULL COMMENT '机场位置',
    flight_count INT DEFAULT 0 COMMENT '航班数量 (可能为动态数据)'
) COMMENT='机场信息表';

-- ============================================================
-- 表: Passenger (乘客)
-- ============================================================
CREATE TABLE Passenger (
    passenger_id INT PRIMARY KEY NOT NULL COMMENT '乘客ID',
    name VARCHAR(100) NOT NULL COMMENT '姓名',
    id_card_number VARCHAR(18) NOT NULL UNIQUE COMMENT '身份证号',
    phone_number VARCHAR(20) NOT NULL COMMENT '联系电话',
    email VARCHAR(100) UNIQUE COMMENT '电子邮件',
    frequent_flyer_number VARCHAR(50) UNIQUE COMMENT '常旅客编号'
) COMMENT='乘客信息表';

-- ============================================================
-- 表: Route (航线)
-- ============================================================
CREATE TABLE Route (
    route_id INT PRIMARY KEY NOT NULL COMMENT '航线ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    origin VARCHAR(100) NOT NULL COMMENT '起点',
    destination VARCHAR(100) NOT NULL COMMENT '终点',
    duration VARCHAR(20) COMMENT '飞行时长 (e.g., 2h 30m)',
    FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE -- 外键约束行为的example
) COMMENT='航线信息表';

-- ============================================================
-- 表: Flight (航班)
-- ============================================================
CREATE TABLE Flight (
    flight_id INT PRIMARY KEY NOT NULL COMMENT '航班ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    route_id INT NOT NULL COMMENT '航线ID (外键)',
    departure_time DATETIME NOT NULL COMMENT '起飞时间',
    arrival_time DATETIME NOT NULL COMMENT '抵达时间',
    departure_location VARCHAR(100) NOT NULL COMMENT '起飞地点 (可能冗余, 可从Route获取)',
    destination_location VARCHAR(100) NOT NULL COMMENT '目的地 (可能冗余, 可从Route获取)',
    total_seats INT NOT NULL COMMENT '座位总数',
    booked_seats INT DEFAULT 0 COMMENT '已预订座位数',
    aircraft_model VARCHAR(50) COMMENT '机型',
    FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (route_id) REFERENCES Route(route_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='航班信息表';

-- ============================================================
-- 表: Booking (预订)
-- ============================================================
CREATE TABLE Booking (
    booking_id INT PRIMARY KEY NOT NULL COMMENT '预订ID',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    seat_type VARCHAR(20) COMMENT '座位类型 (e.g., Economy, Business)',
    booking_date DATETIME NOT NULL COMMENT '预订日期',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Pending' COMMENT '支付状态 (e.g., Pending, Paid, Cancelled)',
    FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE CASCADE ON UPDATE CASCADE, -- 如果乘客被删除，也许删除其预订？按需调整。
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE -- 如果存在预订，则无法删除航班
) COMMENT='预订信息表';

-- ============================================================
-- 表: Payment (支付)
-- ============================================================
CREATE TABLE Payment (
    payment_id INT PRIMARY KEY NOT NULL COMMENT '支付ID',
    booking_id INT NOT NULL COMMENT '预订ID (外键)',
    payment_method VARCHAR(50) NOT NULL COMMENT '支付方式 (e.g., Credit Card, Alipay)',
    payment_amount DECIMAL(10, 2) NOT NULL COMMENT '支付金额',
    payment_time DATETIME NOT NULL COMMENT '支付时间',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Success' COMMENT '支付状态 (e.g., Success, Failed)',
    FOREIGN KEY (booking_id) REFERENCES Booking(booking_id)
        ON DELETE RESTRICT ON UPDATE CASCADE -- 如果存在支付记录，通常无法删除预订
) COMMENT='支付信息表';

-- ============================================================
-- 表: Ticket (机票)
-- ============================================================
CREATE TABLE Ticket (
    ticket_id INT PRIMARY KEY NOT NULL COMMENT '机票ID',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    ticket_number VARCHAR(50) NOT NULL UNIQUE COMMENT '票号',
    seat_number VARCHAR(10) NOT NULL COMMENT '座位号',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价 (可能冗余, 可从Booking获取)',
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE RESTRICT ON UPDATE CASCADE -- 如果乘客有机票，是否禁止删除？或设为NULL？按需调整。
) COMMENT='机票信息表';

-- ============================================================
-- 表: FlightStatus (航班状态)
-- ============================================================
CREATE TABLE FlightStatus (
    flight_id INT PRIMARY KEY NOT NULL COMMENT '航班ID (主键/外键)',
    status VARCHAR(50) NOT NULL COMMENT '状态 (e.g., On Time, Delayed, Cancelled)',
    update_time DATETIME NOT NULL COMMENT '更新时间',
    delay_reason TEXT COMMENT '延误原因',
    cancellation_reason TEXT COMMENT '取消原因',
    FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE CASCADE ON UPDATE CASCADE -- 如果航班被删除，其状态记录也被移除。
) COMMENT='航班状态表 (通常存储当前状态)';


-- ============================================================
-- 添加索引以提高性能并强制约束
-- ============================================================

-- 关于聚集索引的说明 (MySQL/InnoDB):
-- 在 InnoDB 中，表的主键自动作为聚集索引。
-- 表数据根据主键的顺序进行物理存储。
-- 因此，在定义了主键的情况下，不需要（也不可能）使用单独的 'CREATE CLUSTERED INDEX' 命令。
-- 所有上述表都有主键，这些主键充当聚集索引，组织了每个表的物理数据存储。

-- 示例: 添加唯一索引以确保航空公司名称唯一
-- 这强制约束了两个航空公司不能同名。
CREATE UNIQUE INDEX idx_unique_airline_name ON Airline (name);

-- 在外键和常用查询列上添加索引 (非唯一/辅助索引)
-- 这些索引可以提高查找和连接的查询性能。
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
-- AND: 主键通常会自动被索引。在 CREATE TABLE 中定义的唯一键
-- (如 id_card_number, email, ticket_number) 也会自动被索引。

-- ============================================================
-- END!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-- ============================================================