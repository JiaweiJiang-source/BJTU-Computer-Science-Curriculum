-- ============================================================
-- DBLab4_Auth_22281188.sql
-- 用户创建与授权管理脚本
-- 针对 AirlineDB 数据库
-- 作者: [江家玮/北京交通大学] 
-- 日期: 2025-04-19
-- ============================================================

-- ----------------------------------------
-- 1) 创建用户
-- ----------------------------------------
-- 创建几个不同角色的用户，密码

-- 创建一个票务代理用户
CREATE USER 'ticket_agent'@'localhost' IDENTIFIED BY 'AgentPass123!';

-- 创建一个数据分析师用户
CREATE USER 'data_analyst'@'localhost' IDENTIFIED BY 'AnalystPass456!';

-- 创建一个航班管理员用户
CREATE USER 'flight_manager'@'localhost' IDENTIFIED BY 'ManagerPass789!';

-- ----------------------------------------
-- 2) 授予和收回表级权限
-- ----------------------------------------

-- -- 为 ticket_agent 授予权限 --
-- 允许查询航班和乘客信息
GRANT SELECT ON AirlineDB.Flight TO 'ticket_agent'@'localhost';
GRANT SELECT ON AirlineDB.Passenger TO 'ticket_agent'@'localhost';
-- 允许创建和查询预订信息
GRANT SELECT, INSERT ON AirlineDB.Booking TO 'ticket_agent'@'localhost';
-- 允许更新预订的支付状态
GRANT UPDATE ON AirlineDB.Booking TO 'ticket_agent'@'localhost';

-- -- 为 data_analyst 授予权限 --
-- 允许查询数据库中的所有表 (只读权限)
GRANT SELECT ON AirlineDB.* TO 'data_analyst'@'localhost'; -- * 代表所有表

-- -- 为 flight_manager 授予权限 --
-- 允许管理航班和航班状态
GRANT SELECT, INSERT, UPDATE ON AirlineDB.Flight TO 'flight_manager'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AirlineDB.FlightStatus TO 'flight_manager'@'localhost';
-- 允许查询相关信息
GRANT SELECT ON AirlineDB.Airline TO 'flight_manager'@'localhost';
GRANT SELECT ON AirlineDB.Route TO 'flight_manager'@'localhost';
-- 授予对 Airport 表的所有权限
GRANT ALL PRIVILEGES ON AirlineDB.Airport TO 'flight_manager'@'localhost';

-- -- 演示收回权限 --
-- 假设不再允许票务代理直接更新整个 Booking 表，收回 UPDATE 权限
REVOKE UPDATE ON AirlineDB.Booking FROM 'ticket_agent'@'localhost';

-- 收回 flight_manager 对 Airport 表的 DELETE 权限 (从 ALL PRIVILEGES 中移除)
REVOKE DELETE ON AirlineDB.Airport FROM 'flight_manager'@'localhost';


-- ----------------------------------------
-- 3) 授予和收回列级权限
-- ----------------------------------------

-- -- 为 ticket_agent 授予列级权限 --
-- 允许票务代理仅查询乘客的姓名和电话号码
GRANT SELECT (passenger_id, name, phone_number) ON AirlineDB.Passenger TO 'ticket_agent'@'localhost';

-- 因此，我们先收回之前对 Passenger 表的 SELECT 权限
REVOKE SELECT ON AirlineDB.Passenger FROM 'ticket_agent'@'localhost';
-- 再授予列级 SELECT
GRANT SELECT (passenger_id, name, phone_number) ON AirlineDB.Passenger TO 'ticket_agent'@'localhost';

-- 允许票务代理仅更新 Booking 表的支付状态
GRANT UPDATE (payment_status) ON AirlineDB.Booking TO 'ticket_agent'@'localhost';

-- -- 演示收回列级权限 --
-- 收回票务代理更新 Booking 表支付状态的权限
REVOKE UPDATE (payment_status) ON AirlineDB.Booking FROM 'ticket_agent'@'localhost';


-- ----------------------------------------
-- 4) 创建角色并管理角色权限
-- ----------------------------------------

-- -- 创建角色 --
CREATE ROLE 'ReadOnlyRole'@'localhost';
CREATE ROLE 'BookingAgentRole'@'localhost';
CREATE ROLE 'FlightAdminRole'@'localhost';

-- -- 为角色授权 --
-- 为只读角色授予所有表的 SELECT 权限
GRANT SELECT ON AirlineDB.* TO 'ReadOnlyRole'@'localhost';

-- 为预订代理角色授权
GRANT SELECT ON AirlineDB.Flight TO 'BookingAgentRole'@'localhost';
GRANT SELECT (passenger_id, name, phone_number, email) ON AirlineDB.Passenger TO 'BookingAgentRole'@'localhost'; -- 授予查询乘客部分信息的权限
GRANT SELECT, INSERT ON AirlineDB.Booking TO 'BookingAgentRole'@'localhost';
GRANT UPDATE (payment_status) ON AirlineDB.Booking TO 'BookingAgentRole'@'localhost'; -- 允许更新支付状态
GRANT SELECT ON AirlineDB.Route TO 'BookingAgentRole'@'localhost'; -- 可能需要看航线信息

-- 为航班管理角色授权
GRANT SELECT, INSERT, UPDATE, DELETE ON AirlineDB.Flight TO 'FlightAdminRole'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON AirlineDB.FlightStatus TO 'FlightAdminRole'@'localhost';
GRANT SELECT ON AirlineDB.Airline TO 'FlightAdminRole'@'localhost';
GRANT SELECT ON AirlineDB.Route TO 'FlightAdminRole'@'localhost';
GRANT SELECT, UPDATE, INSERT, DELETE ON AirlineDB.Airport TO 'FlightAdminRole'@'localhost'; -- 之前授权给用户的 ALL PRIVILEGES 包含更多，这里给角色具体权限

-- -- 将角色授予用户 --
GRANT 'ReadOnlyRole'@'localhost' TO 'data_analyst'@'localhost';
GRANT 'BookingAgentRole'@'localhost' TO 'ticket_agent'@'localhost';
GRANT 'FlightAdminRole'@'localhost' TO 'flight_manager'@'localhost';


SET DEFAULT ROLE ALL TO 'data_analyst'@'localhost', 'ticket_agent'@'localhost', 'flight_manager'@'localhost';


-- -- 演示从用户收回角色 --
REVOKE 'ReadOnlyRole'@'localhost' FROM 'data_analyst'@'localhost';



-- ============================================================
-- 测试说明 (需要在 MySQL 客户端中执行)
-- ============================================================
/*

1.  **登录测试:**
    * 尝试分别使用 'ticket_agent'@'localhost', 'data_analyst'@'localhost', 'flight_manager'@'localhost' 和对应的密码登录 MySQL。
    * `mysql -u ticket_agent -p` (输入 AgentPass123!)
    * `mysql -u data_analyst -p` (输入 AnalystPass456!)
    * `mysql -u flight_manager -p` (输入 ManagerPass789!)

2.  **权限测试 (登录后):**
    * **对于 ticket_agent:**
        * `USE AirlineDB;`
        * 尝试 `SELECT * FROM Flight;` (成功)
        * 尝试 `SELECT name, phone_number FROM Passenger WHERE passenger_id = 1001;` (成功)
        * 尝试 `SELECT id_card_number FROM Passenger WHERE passenger_id = 1001;` (失败 - 无此列权限)
        * 尝试 `SELECT * FROM Airline;` (失败 - 无此表权限)
        * 尝试 `UPDATE Booking SET seat_type = 'Business' WHERE booking_id = 4001;` (失败 - 无此列 UPDATE 权限，除非重新授权)
        * 尝试 `DELETE FROM Flight WHERE flight_id = 3001;` (失败 - 无 DELETE 权限)

    * **对于 data_analyst:**
        * `USE AirlineDB;`
        * 尝试 `SELECT * FROM Flight;` (成功 - 通过 ReadOnlyRole，如果该角色未被撤销)
        * 尝试 `SELECT * FROM Airline;` (成功 - 通过 ReadOnlyRole，如果该角色未被撤销)
        * 尝试 `UPDATE Flight SET booked_seats = 10 WHERE flight_id = 3001;` (失败)

    * **对于 flight_manager:**
        * `USE AirlineDB;`
        * 尝试 `SELECT * FROM Flight;` (成功)
        * 尝试 `UPDATE Flight SET booked_seats = 50 WHERE flight_id = 3001;` (成功)
        * 尝试 `SELECT * FROM Airport;` (成功)
        * 尝试 `DELETE FROM Airport WHERE airport_id = 103;` (成功)
        * 尝试 `SELECT * FROM Passenger;` (失败 - 未授权)

3.  **查看权限:**
    * 登录具有足够权限的用户 (如 root)。
    * `SHOW GRANTS FOR 'ticket_agent'@'localhost';`
    * `SHOW GRANTS FOR 'data_analyst'@'localhost';`
    * `SHOW GRANTS FOR 'flight_manager'@'localhost';`
    * `SHOW GRANTS FOR 'ReadOnlyRole'@'localhost';`
    * `SHOW GRANTS FOR 'ticket_agent'@'localhost' USING 'BookingAgentRole'@'localhost';` (查看通过角色获得的权限)

*/