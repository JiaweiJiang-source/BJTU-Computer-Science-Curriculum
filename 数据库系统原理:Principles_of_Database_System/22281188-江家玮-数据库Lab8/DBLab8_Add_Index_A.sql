-- DBLab8_Add_Index_A.sql
-- 为 Passenger 表的 name 属性添加非聚集索引

USE AirlineDB;

-- 删除可能存在的旧索引 (如果重复运行脚本)
-- DROP INDEX idx_passenger_name ON Passenger;

-- 为属性 A (Passenger.name) 创建非聚集索引
CREATE INDEX idx_passenger_name ON Passenger(name);

-- 验证索引是否创建成功
SHOW INDEX FROM Passenger WHERE Key_name = 'idx_passenger_name';
