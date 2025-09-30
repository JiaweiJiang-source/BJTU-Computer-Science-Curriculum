-- ============================================================
-- lab7_procedures_triggers.sql
-- 要在DBLab3_22281188.sql & DBLab3_Data_Insert.sql之后运行
-- trigger和存储过程创建脚本
-- 作者: [江家玮/北京交通大学] 
-- 日期: 2025-05-15
-- ============================================================

USE AirlineDB;

DELIMITER //

-- ============================================================
-- I. 存储过程 (Stored Procedures)
-- ============================================================

-- 1. 多表查询存储过程
--    获取乘客详细信息及其预订。
DROP PROCEDURE IF EXISTS GetPassengerAndBookings;
CREATE PROCEDURE GetPassengerAndBookings(
    IN p_passenger_id INT
)
BEGIN
    SELECT
        p.passenger_id,
        p.name AS passenger_name,
        p.id_card_number,
        p.phone_number,
        p.email,
        b.booking_id,
        b.flight_id,
        f.departure_time,
        f.arrival_time,
        r.origin,
        r.destination,
        b.seat_type,
        b.booking_date,
        b.price AS booking_price,
        b.payment_status
    FROM Passenger p
    LEFT JOIN Booking b ON p.passenger_id = b.passenger_id
    LEFT JOIN Flight f ON b.flight_id = f.flight_id
    LEFT JOIN Route r ON f.route_id = r.route_id
    WHERE p.passenger_id = p_passenger_id;
END //

-- 2. 数据插入存储过程 (乘客)
DROP PROCEDURE IF EXISTS AddPassengerViaSP;
CREATE PROCEDURE AddPassengerViaSP(
    IN p_name VARCHAR(100),
    IN p_id_card_number VARCHAR(18),
    IN p_phone_number VARCHAR(20),
    IN p_email VARCHAR(100),
    IN p_frequent_flyer_number VARCHAR(50),
    OUT p_new_passenger_id INT
)
BEGIN
    -- 生成一个新的 passenger_id（假设它不是自增或者需要为存储过程手动生成）
    -- 如果 passenger_id 是自增的，这部分对于插入本身来说并非严格必要，
    -- 但将 ID 作为 OUT 参数仍然很有用。
    -- 为简单起见，我们假设 passenger_id 是自增的，并且我们获取它。
    -- 如果 passenger_id 不是自增的，则需要在这里添加逻辑来确定下一个 ID。
    -- 在此示例中，我们将假设 Passenger 表中的 passenger_id 是自增的。

    INSERT INTO Passenger (name, id_card_number, phone_number, email, frequent_flyer_number)
    VALUES (p_name, p_id_card_number, p_phone_number, p_email, p_frequent_flyer_number);

    SET p_new_passenger_id = LAST_INSERT_ID();
END //

-- 3. 数据删除存储过程 (乘客)
DROP PROCEDURE IF EXISTS DeletePassengerViaSP;
CREATE PROCEDURE DeletePassengerViaSP(
    IN p_passenger_id INT
)
BEGIN
    -- 考虑相关数据。如果对预订设置了 ON DELETE CASCADE，它们将被删除。
    -- 如果设置了 ON DELETE RESTRICT，如果存在预订，这可能会失败。
    -- 对于此存储过程，我们将尝试删除。错误处理将在调用应用程序中进行。
    DELETE FROM Passenger WHERE passenger_id = p_passenger_id;
END //

-- 4. 数据修改存储过程 (更新乘客邮箱)
DROP PROCEDURE IF EXISTS UpdatePassengerEmailViaSP;
CREATE PROCEDURE UpdatePassengerEmailViaSP(
    IN p_passenger_id INT,
    IN p_new_email VARCHAR(100)
)
BEGIN
    UPDATE Passenger
    SET email = p_new_email
    WHERE passenger_id = p_passenger_id;
END //


-- ============================================================
-- II. 触发器 (Triggers)
-- ============================================================

-- 如果不存在，则为乘客邮箱更改创建一个审计表
CREATE TABLE IF NOT EXISTS PassengerEmailAudit (
    audit_id INT AUTO_INCREMENT PRIMARY KEY,
    passenger_id INT,
    old_email VARCHAR(100),
    new_email VARCHAR(100),
    change_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by VARCHAR(255) DEFAULT 'DB_TRIGGER' -- 指示更改是由触发器完成的
);

-- 1. 数据插入触发器 (新预订插入后)
--    此触发器将更新 Flight 表中的已预订座位数。
DROP TRIGGER IF EXISTS AfterBookingInsert;
CREATE TRIGGER AfterBookingInsert
AFTER INSERT ON Booking
FOR EACH ROW
BEGIN
    UPDATE Flight
    SET booked_seats = booked_seats + 1
    WHERE flight_id = NEW.flight_id;
END //

-- 2. 数据更新触发器 (乘客邮箱更新后)
--    此触发器会将更改记录到 PassengerEmailAudit 表中。
DROP TRIGGER IF EXISTS AfterPassengerEmailUpdate;
CREATE TRIGGER AfterPassengerEmailUpdate
AFTER UPDATE ON Passenger
FOR EACH ROW
BEGIN
    IF OLD.email <> NEW.email OR (OLD.email IS NULL AND NEW.email IS NOT NULL) OR (OLD.email IS NOT NULL AND NEW.email IS NULL) THEN
        INSERT INTO PassengerEmailAudit (passenger_id, old_email, new_email)
        VALUES (OLD.passenger_id, OLD.email, NEW.email);
    END IF;
END //

-- 3. 数据删除触发器 (预订删除后)
--    此触发器将更新 Flight 表中的已预订座位数。
DROP TRIGGER IF EXISTS AfterBookingDelete;
CREATE TRIGGER AfterBookingDelete
AFTER DELETE ON Booking
FOR EACH ROW
BEGIN
    UPDATE Flight
    SET booked_seats = booked_seats - 1
    WHERE flight_id = OLD.flight_id;
END //

DELIMITER ;

-- 示例调用 (用于在 MySQL 客户端中测试):
/*
-- 测试 GetPassengerAndBookings
CALL GetPassengerAndBookings(1001);

-- 测试 AddPassengerViaSP
SET @new_id = 0;
CALL AddPassengerViaSP('Test SP User', '123456789012345679', '13000000000', 'testspuser@example.com', 'SPFLYER001', @new_id);
SELECT @new_id;
SELECT * FROM Passenger WHERE passenger_id = @new_id;

-- 测试 UpdatePassengerEmailViaSP (假设上面或已存在一个 passenger_id)
-- 首先，检查乘客 1001 的当前邮箱
SELECT email FROM Passenger WHERE passenger_id = 1001;
CALL UpdatePassengerEmailViaSP(1001, 'new.email.via.sp@example.com');
SELECT email FROM Passenger WHERE passenger_id = 1001;
SELECT * FROM PassengerEmailAudit WHERE passenger_id = 1001; -- 检查审计日志

-- 测试 DeletePassengerViaSP (使用通过存储过程添加的乘客 ID)
-- 确保此乘客没有来自 Booking/Ticket 的限制性外键约束
-- 例如，如果乘客 @new_id (例如，1004) 没有预订，这应该可以工作。
-- CALL DeletePassengerViaSP(@new_id);
-- SELECT * FROM Passenger WHERE passenger_id = @new_id;

-- 测试触发器:
-- 1. AfterBookingInsert:
--    检查某个航班 (例如，flight_id = 3001) 的初始已预订座位数
SELECT flight_id, booked_seats FROM Flight WHERE flight_id = 3001;
--    为此航班插入一个新预订 (passenger_id 1003, flight_id 3001)
--    确保 booking_id 是唯一的，例如 4004
INSERT INTO Booking (booking_id, passenger_id, flight_id, seat_type, booking_date, price, payment_status)
VALUES (4004, 1003, 3001, 'Economy', NOW(), 800.00, 'Pending');
--    再次检查已预订座位数
SELECT flight_id, booked_seats FROM Flight WHERE flight_id = 3001; -- 应该增加

-- 2. AfterPassengerEmailUpdate 已在上面的 UpdatePassengerEmailViaSP 中测试过。

-- 3. AfterBookingDelete:
--    删除上面创建的预订 (booking_id = 4004)
DELETE FROM Booking WHERE booking_id = 4004;
--    再次检查已预订座位数
SELECT flight_id, booked_seats FROM Flight WHERE flight_id = 3001; -- 应该减少
*/