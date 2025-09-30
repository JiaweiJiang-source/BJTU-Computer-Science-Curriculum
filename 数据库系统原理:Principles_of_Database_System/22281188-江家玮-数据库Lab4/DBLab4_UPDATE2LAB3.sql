-- ============================================================
-- DBLab4_UPDATE2LAB3
-- 航空公司数据库模式创建脚本--------------->对于LAB3的更新
-- 目标平台: 通用 SQL (主要基于 MySQL 语法测试)
-- 作者: [江家玮/北京交通大学] 
-- 日期: 2025-04-19
-- ============================================================

-- 不存在则创建
-- CREATE DATABASE IF NOT EXISTS AirlineDB CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
-- USE AirlineDB;

-- 按依赖反序删除表
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
    airline_id INT NOT NULL COMMENT '航空公司ID',
    name VARCHAR(100) NOT NULL COMMENT '名称',
    hq_location VARCHAR(255) COMMENT '总部位置',
    contact_info VARCHAR(50) COMMENT '联系方式',
    route_count INT DEFAULT 0 COMMENT '航线数量',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_airline PRIMARY KEY (airline_id),
    -- 3) 用户定义完整性 + 4) 约束命名 (航空公司名称唯一)
    CONSTRAINT uq_airline_name UNIQUE (name)
) COMMENT='航空公司信息表';

-- ============================================================
-- 表: Airport (机场)
-- ============================================================
CREATE TABLE Airport (
    airport_id INT NOT NULL COMMENT '机场ID',
    airport_name VARCHAR(100) NOT NULL COMMENT '机场名称',
    location VARCHAR(255) NOT NULL COMMENT '机场位置',
    flight_count INT DEFAULT 0 COMMENT '航班数量 (可能为动态数据)',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_airport PRIMARY KEY (airport_id)
) COMMENT='机场信息表';

-- ============================================================
-- 表: Passenger (乘客)
-- ============================================================
CREATE TABLE Passenger (
    passenger_id INT NOT NULL COMMENT '乘客ID',
    name VARCHAR(100) NOT NULL COMMENT '姓名',
    id_card_number VARCHAR(18) NOT NULL COMMENT '身份证号',
    phone_number VARCHAR(20) NOT NULL COMMENT '联系电话',
    email VARCHAR(100) COMMENT '电子邮件',
    frequent_flyer_number VARCHAR(50) COMMENT '常旅客编号',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_passenger PRIMARY KEY (passenger_id),
    -- 3) 用户定义完整性 + 4) 约束命名 (确保身份证号唯一)
    CONSTRAINT uq_passenger_idcard UNIQUE (id_card_number),
    -- 3) 用户定义完整性 + 4) 约束命名 (确保电子邮件唯一, 如果非空)
    CONSTRAINT uq_passenger_email UNIQUE (email),
    -- 3) 用户定义完整性 + 4) 约束命名 (确保常旅客编号唯一, 如果非空)
    CONSTRAINT uq_passenger_ffn UNIQUE (frequent_flyer_number)
) COMMENT='乘客信息表';

-- ============================================================
-- 表: Route (航线)
-- ============================================================
CREATE TABLE Route (
    route_id INT NOT NULL COMMENT '航线ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    origin VARCHAR(100) NOT NULL COMMENT '起点',
    destination VARCHAR(100) NOT NULL COMMENT '终点',
    duration VARCHAR(20) COMMENT '飞行时长 (e.g., 2h 30m)',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_route PRIMARY KEY (route_id),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Airline 表)
    CONSTRAINT fk_route_airline FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE
) COMMENT='航线信息表';

-- ============================================================
-- 表: Flight (航班)
-- ============================================================
CREATE TABLE Flight (
    flight_id INT NOT NULL COMMENT '航班ID',
    airline_id INT NOT NULL COMMENT '航空公司ID (外键)',
    route_id INT NOT NULL COMMENT '航线ID (外键)',
    departure_time DATETIME NOT NULL COMMENT '起飞时间',
    arrival_time DATETIME NOT NULL COMMENT '抵达时间',
    departure_location VARCHAR(100) NOT NULL COMMENT '起飞地点',
    destination_location VARCHAR(100) NOT NULL COMMENT '目的地',
    total_seats INT NOT NULL COMMENT '座位总数',
    booked_seats INT DEFAULT 0 COMMENT '已预订座位数',
    aircraft_model VARCHAR(50) COMMENT '机型',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_flight PRIMARY KEY (flight_id),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Airline 表)
    CONSTRAINT fk_flight_airline FOREIGN KEY (airline_id) REFERENCES Airline(airline_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Route 表)
    CONSTRAINT fk_flight_route FOREIGN KEY (route_id) REFERENCES Route(route_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    -- 3) 用户定义完整性 + 4) 约束命名 (检查座位数)
    CONSTRAINT chk_flight_seats CHECK (booked_seats >= 0 AND booked_seats <= total_seats AND total_seats > 0),
    -- 3) 用户定义完整性 + 4) 约束命名 (检查起飞到达时间)
    CONSTRAINT chk_flight_times CHECK (arrival_time > departure_time)
) COMMENT='航班信息表';

-- ============================================================
-- 表: Booking (预订)
-- ============================================================
CREATE TABLE Booking (
    booking_id INT NOT NULL COMMENT '预订ID',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    seat_type VARCHAR(20) COMMENT '座位类型 (e.g., Economy, Business)',
    booking_date DATETIME NOT NULL COMMENT '预订日期',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Pending' COMMENT '支付状态',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_booking PRIMARY KEY (booking_id),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Passenger 表)
    CONSTRAINT fk_booking_passenger FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Flight 表)
    CONSTRAINT fk_booking_flight FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    -- 3) 用户定义完整性 + 4) 约束命名 (检查价格必须为正)
    CONSTRAINT chk_booking_price CHECK (price > 0),
    -- 3) 用户定义完整性 + 4) 约束命名 (检查支付状态必须在允许范围内)
    CONSTRAINT chk_booking_status CHECK (payment_status IN ('Pending', 'Paid', 'Cancelled'))
) COMMENT='预订信息表';

-- ============================================================
-- 表: Payment (支付)
-- ============================================================
CREATE TABLE Payment (
    payment_id INT NOT NULL COMMENT '支付ID',
    booking_id INT NOT NULL COMMENT '预订ID (外键)',
    payment_method VARCHAR(50) NOT NULL COMMENT '支付方式 (e.g., Credit Card, Alipay)',
    payment_amount DECIMAL(10, 2) NOT NULL COMMENT '支付金额',
    payment_time DATETIME NOT NULL COMMENT '支付时间',
    payment_status VARCHAR(20) NOT NULL DEFAULT 'Success' COMMENT '支付状态',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_payment PRIMARY KEY (payment_id),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Booking 表)
    CONSTRAINT fk_payment_booking FOREIGN KEY (booking_id) REFERENCES Booking(booking_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    -- 3) 用户定义完整性 + 4) 约束命名 (检查支付金额为正)
    CONSTRAINT chk_payment_amount CHECK (payment_amount > 0),
    -- 3) 用户定义完整性 + 4) 约束命名 (检查支付状态)
    CONSTRAINT chk_payment_status CHECK (payment_status IN ('Success', 'Failed', 'Processing')) -- 增加 'Processing' 状态
) COMMENT='支付信息表';

-- ============================================================
-- 表: Ticket (机票)
-- ============================================================
CREATE TABLE Ticket (
    ticket_id INT NOT NULL COMMENT '机票ID',
    flight_id INT NOT NULL COMMENT '航班ID (外键)',
    ticket_number VARCHAR(50) NOT NULL COMMENT '票号',
    seat_number VARCHAR(10) NOT NULL COMMENT '座位号',
    passenger_id INT NOT NULL COMMENT '乘客ID (外键)',
    price DECIMAL(10, 2) NOT NULL COMMENT '票价',
    -- 1) 实体完整性 (命名主键约束)
    CONSTRAINT pk_ticket PRIMARY KEY (ticket_id),
    -- 3) 用户定义完整性 + 4) 约束命名 (票号唯一)
    CONSTRAINT uq_ticket_number UNIQUE (ticket_number),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Flight 表)
    CONSTRAINT fk_ticket_flight FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Passenger 表)
    CONSTRAINT fk_ticket_passenger FOREIGN KEY (passenger_id) REFERENCES Passenger(passenger_id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
     -- 3) 用户定义完整性 + 4) 约束命名 (检查价格必须为正)
    CONSTRAINT chk_ticket_price CHECK (price >= 0) -- 允许价格为 0 (例如里程兑换?)
) COMMENT='机票信息表';

-- ============================================================
-- 表: FlightStatus (航班状态)
-- ============================================================
CREATE TABLE FlightStatus (
    flight_id INT NOT NULL COMMENT '航班ID (主键/外键)',
    status VARCHAR(50) NOT NULL COMMENT '状态',
    update_time DATETIME NOT NULL COMMENT '更新时间',
    delay_reason TEXT COMMENT '延误原因',
    cancellation_reason TEXT COMMENT '取消原因',
    -- 1) 实体完整性 (命名主键约束) - 注意：这里 flight_id 是主键，也同时是外键
    CONSTRAINT pk_flightstatus PRIMARY KEY (flight_id),
    -- 2) 参照完整性 + 4) 约束命名 (外键关联 Flight 表)
    CONSTRAINT fk_flightstatus_flight FOREIGN KEY (flight_id) REFERENCES Flight(flight_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    -- 3) 用户定义完整性 + 4) 约束命名 (检查状态值)
    CONSTRAINT chk_flightstatus_status CHECK (status IN ('Scheduled', 'On Time', 'Delayed', 'Cancelled', 'Boarding', 'Departed', 'Arrived'))
) COMMENT='航班状态表';


-- ============================================================
-- 添加索引以提高性能并强制约束
-- ============================================================

-- 关于聚集索引的说明 (MySQL/InnoDB):
-- 在 InnoDB 中，表的主键自动作为聚集索引。
-- 表数据根据主键的顺序进行物理存储。
-- 因此，在定义了主键的情况下，不需要（也不可能）使用单独的 'CREATE CLUSTERED INDEX' 命令。
-- 所有上述表都有主键，这些主键充当聚集索引，组织了每个表的物理数据存储。

-- 例如添加唯一索引以确保航空公司名称唯一
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