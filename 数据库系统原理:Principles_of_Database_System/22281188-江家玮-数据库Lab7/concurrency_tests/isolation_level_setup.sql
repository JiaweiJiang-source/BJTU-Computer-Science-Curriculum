-- isolation_level_setup.sql
-- 在 MySQL 客户端会话中运行这些命令，以设置手动测试所需的隔离级别。
-- Python 脚本将设置其自己的会话隔离级别。

-- 检查当前级别
SELECT @@SESSION.transaction_isolation;
SELECT @@GLOBAL.transaction_isolation;

-- 设置会话隔离级别 (为您的测试会话选择一个)
SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; -- 读未提交
SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED; -- 读已提交
SET SESSION TRANSACTION ISOLATION LEVEL REPEATABLE READ; -- 可重复读
SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE; -- 串行化

-- 如果 Accounts 表尚未创建，则创建它用于测试
USE AirlineDB;
CREATE TABLE IF NOT EXISTS Accounts (
    account_id INT PRIMARY KEY,
    balance DECIMAL(10, 2) NOT NULL
);

-- 确保存在用于测试的数据 (运行一次或检查是否存在)
INSERT IGNORE INTO Accounts (account_id, balance) VALUES (1, 1000.00);
INSERT IGNORE INTO Accounts (account_id, balance) VALUES (2, 500.00);