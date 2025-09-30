# 数据库系统原理实验 Lab8 - 索引与压力测试、备份与日志初步

本项目旨在通过一系列实验，探究数据库索引对性能的影响、进行数据库压力测试，并初步学习数据库的备份、恢复与日志分析机制。实验基于MySQL数据库，并使用Python脚本进行数据生成、插入、查询和耗时记录。

## 目录结构

```
Lab8_22281188/
├── Lab8_Report_22281188.md         # Markdown 格式实验报告
├── README.md                       # 本 README 文件
├── DBLab8_Base_Schema.sql          # 基础表结构脚本 (基于Lab3修改，移除了部分UNIQUE约束以避免插入冲突)
├── DBLab8_Add_Index_A.sql        # 为 Passenger.name 属性添加索引的脚本
├── scripts/                        # Python 脚本目录
│   ├── config.py                   # 数据库连接配置和测试参数
│   ├── data_generator.py         # 数据生成辅助脚本
│   ├── inserter.py                 # 数据插入脚本 (记录耗时)
│   ├── querier.py                  # 数据查询脚本 (记录耗时)
│   └── main_test_runner.py         # (可选) 概念性的测试编排脚本示例
├── results/                        # 实验结果存放目录
│   ├── no_index_insert_log.csv     # 无索引时插入操作耗时记录
│   ├── no_index_query_log.csv      # 无索引时查询操作耗时记录
│   ├── with_index_insert_log.csv   # 有索引时插入操作耗时记录
│   ├── with_index_query_log.csv    # 有索引时查询操作耗时记录
│   └── performance_charts/         # 生成的性能图表存放目录 (需手动生成)
├── backup/                         # 数据库备份文件存放目录 (需手动生成)
│   ├── airline_db_full_backup.sql
│   └── passenger_table_backup.sql
└── logs_analysis/                  # 日志分析相关文件 (需手动生成)
└── mysql_binlog_example.txt
```

## 先决条件

1.  **MySQL数据库**: 已安装并正在运行。
2.  **Python 3.x**: 已安装。
3.  **Python库**:
    * `mysql-connector-python`: 用于Python连接MySQL。
    * `Faker`: 用于生成模拟数据。
    * `pandas`, `matplotlib`, `seaborn`: (可选，用于数据分析和绘图)
    可以通过pip安装：
    ```bash
    pip install mysql-connector-python Faker pandas matplotlib seaborn
    ```

## 安装与设置

1.  **克隆或下载项目**: 将本项目文件放置到您的本地计算机。
2.  **配置数据库连接**:
    * 打开 `scripts/config.py` 文件。
    * 修改 `DB_CONFIG` 字典中的数据库连接参数（`host`, `user`, `password`, `database`, `port`）以匹配您的MySQL配置。
        ```python
        DB_CONFIG = {
            'host': 'localhost',
            'user': '你的MySQL用户名',
            'password': '你的MySQL密码',
            'database': 'AirlineDB', # 确保这个数据库存在
            'port': 3306
        }
        ```
    * （可选）根据需要调整 `INSERTER_CONFIG` 和 `QUERIER_CONFIG` 中的测试参数，如数据插入总量、批次大小、并发数等。**建议初次运行时将 `total_rows_to_insert` 设置为较小值（如10000）进行测试。**

3.  **创建数据库和基础表结构**:
    * 确保MySQL服务已启动。
    * 使用MySQL客户端工具（如命令行、Navicat、DBeaver等）连接到您的MySQL服务器。
    * 如果 `AirlineDB` 数据库尚不存在，请创建它：
        ```sql
        CREATE DATABASE IF NOT EXISTS AirlineDB CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
        ```
    * 执行 `DBLab8_Base_Schema.sql` 脚本来创建实验所需的表结构。此脚本基于Lab3的结构，但**已移除了 `Passenger` 表中 `email`, `id_card_number`, `frequent_flyer_number` 列的 `UNIQUE` 约束**，以避免在大量数据插入时因 `Faker` 生成重复数据而导致实验中断。
        ```bash
        # 命令行示例 (请替换为您的用户名和实际路径)
        mysql -u your_mysql_user -p AirlineDB < /path/to/your/project/DBLab8_Base_Schema.sql
        ```

## 运行实验

实验主要分为两部分：压力测试与索引实验，以及备份与日志初步实验。

### 第一部分：压力测试与索引实验

此部分包含两个主要场景：无索引和有索引。

**重要**: 在切换场景或重新运行同一场景前，请务必清空 `Passenger` 表的数据 (`TRUNCATE TABLE Passenger;`) 并确保索引状态正确。

#### 场景1: 无索引压力测试

1.  **准备数据库 (无索引)**:
    * 连接到 `AirlineDB`。
    * 确保 `Passenger` 表的 `name` 列上**没有**名为 `idx_passenger_name` 的索引。如果存在，请删除它：
        ```sql
        USE AirlineDB;
        DROP INDEX IF EXISTS idx_passenger_name ON Passenger;
        ```
    * 清空 `Passenger` 表：
        ```sql
        TRUNCATE TABLE Passenger;
        ```

2.  **运行插入脚本**:
    * 打开一个终端，导航到 `scripts/` 目录。
    * 修改 `inserter.py` 文件，确保 `if __name__ == '__main__':` 部分调用的是无索引场景：
        ```python
        # inserter.py
        if __name__ == '__main__':
            print("--- RUNNING INSERTER FOR NO INDEX SCENARIO ---")
            run_inserters(use_index_scenario=False)
            # print("\n--- RUNNING INSERTER FOR WITH INDEX SCENARIO ---")
            # run_inserters(use_index_scenario=True)
        ```
    * 执行插入脚本：`python inserter.py`
    * 耗时数据将记录在 `results/no_index_insert_log.csv`。

3.  **并发运行查询脚本**:
    * 在插入脚本运行时，打开**另一个**终端，导航到 `scripts/` 目录。
    * 修改 `querier.py` 文件，确保 `if __name__ == '__main__':` 部分调用的是无索引场景：
        ```python
        # querier.py
        if __name__ == '__main__':
            print("--- RUNNING QUERIER FOR NO INDEX SCENARIO ---")
            run_querier(use_index_scenario=False)
            # print("\n--- RUNNING QUERIER FOR WITH INDEX SCENARIO ---")
            # run_querier(use_index_scenario=True)
        ```
    * 执行查询脚本：`python querier.py`
    * 耗时数据将记录在 `results/no_index_query_log.csv`。

4.  **监控**: 观察脚本运行，等待插入达到目标数量。查询脚本会根据配置运行一段时间或次数。

#### 场景2: 有索引压力测试

1.  **准备数据库 (有索引)**:
    * 连接到 `AirlineDB`。
    * 清空 `Passenger` 表：
        ```sql
        USE AirlineDB;
        TRUNCATE TABLE Passenger;
        ```
    * 为 `Passenger.name` 列创建索引，执行 `DBLab8_Add_Index_A.sql` 脚本：
        ```bash
        # 命令行示例 (请替换为您的用户名和实际路径)
        mysql -u your_mysql_user -p AirlineDB < /path/to/your/project/DBLab8_Add_Index_A.sql
        ```
    * 验证索引已创建：`SHOW INDEX FROM Passenger WHERE Key_name = 'idx_passenger_name';`

2.  **运行插入脚本**:
    * 打开一个终端，导航到 `scripts/` 目录。
    * 修改 `inserter.py` 文件，确保 `if __name__ == '__main__':` 部分调用的是有索引场景：
        ```python
        # inserter.py
        if __name__ == '__main__':
            # print("--- RUNNING INSERTER FOR NO INDEX SCENARIO ---")
            # run_inserters(use_index_scenario=False)
            print("\n--- RUNNING INSERTER FOR WITH INDEX SCENARIO ---")
            run_inserters(use_index_scenario=True)
        ```
    * 执行插入脚本：`python inserter.py`
    * 耗时数据将记录在 `results/with_index_insert_log.csv`。

3.  **并发运行查询脚本**:
    * 在插入脚本运行时，打开**另一个**终端，导航到 `scripts/` 目录。
    * 修改 `querier.py` 文件，确保 `if __name__ == '__main__':` 部分调用的是有索引场景：
        ```python
        # querier.py
        if __name__ == '__main__':
            # print("--- RUNNING QUERIER FOR NO INDEX SCENARIO ---")
            # run_querier(use_index_scenario=False)
            print("\n--- RUNNING QUERIER FOR WITH INDEX SCENARIO ---")
            run_querier(use_index_scenario=True)
        ```
    * 执行查询脚本：`python querier.py`
    * 耗时数据将记录在 `results/with_index_query_log.csv`。

4.  **监控**: 同无索引场景。

#### 数据分析

* 测试完成后，`results/` 目录下会生成4个CSV日志文件。
* 您可以使用实验报告 (`Lab8_Report_22281188.md`) 中提供的Python绘图代码片段（或您选择的其他工具如Excel）来分析这些数据并生成性能对比图表。将图表和分析结果填入实验报告。

### 第二部分：备份与日志初步实验

1.  **数据备份与恢复**:
    * 使用 `mysqldump` 工具进行单表和整库备份，将备份文件存放在 `backup/` 目录。
        ```bash
        # 备份 Passenger 表 (替换your_mysql_user和密码)
        mysqldump -u your_mysql_user -p AirlineDB Passenger > backup/passenger_table_backup.sql
        
        # 备份整个 AirlineDB 数据库 (替换your_mysql_user和密码)
        mysqldump -u your_mysql_user -p AirlineDB --routines --events --triggers > backup/airline_db_full_backup.sql
        ```
    * 尝试在另一个（测试）数据库或另一台机器上使用这些备份文件恢复数据，并验证恢复的完整性。
        ```bash
        # 恢复示例 (替换your_mysql_user和目标数据库名)
        mysql -u your_mysql_user -p TargetDB < backup/passenger_table_backup.sql
        ```
    * 在实验报告中描述此过程。

2.  **数据库日志分析 (MySQL Binary Log)**:
    * **启用Binary Log**: 确保您的MySQL服务器已启用二进制日志（binlog），且 `binlog_format` 建议设置为 `ROW`。这通常在MySQL配置文件 (`my.cnf` 或 `my.ini`) 中设置，修改后需重启MySQL服务。
        ```ini
        [mysqld]
        log_bin = /var/log/mysql/mysql-bin # 或您的自定义路径
        server_id = 1 # 必须设置
        binlog_format = ROW
        ```
    * **执行操作**: 对 `Passenger` 表执行一些 `INSERT`, `UPDATE`, 或 `DELETE` 操作。
    * **查看Binlog**:
        * 找到当前binlog文件：登录MySQL执行 `SHOW MASTER STATUS;`。
        * 使用 `mysqlbinlog` 工具查看日志内容，并将感兴趣的部分输出到 `logs_analysis/mysql_binlog_example.txt`。
            ```bash
            # 示例 (替换为实际的binlog文件路径)
            mysqlbinlog /var/log/mysql/mysql-bin.000001 --base64-output=DECODE-ROWS -v --database=AirlineDB > logs_analysis/mysql_binlog_example.txt
            ```
    * 在实验报告中解释一条从binlog中找到的数据修改操作记录。

## 预期输出

* `results/` 目录下生成四个CSV日志文件，记录插入和查询的耗时。
* （手动）`results/performance_charts/` 目录下生成性能对比图表。
* （手动）`backup/` 目录下生成数据库备份SQL文件。
* （手动）`logs_analysis/` 目录下生成binlog分析文本文件。
* 完成的 `Lab8_Report_22281188.md` 实验报告，包含所有分析和截图。

## 注意事项与故障排查

* **数据库凭据**: 确保 `scripts/config.py` 中的数据库连接信息正确无误。
* **Python依赖**: 确保所有必要的Python库已安装。
* **文件路径**: 脚本中的相对路径是基于 `scripts/` 目录的。如果从其他位置运行，可能需要调整路径。
* **MySQL权限**: 执行 `mysqldump` 和 `mysql` 命令的用户需要有相应的数据库权限。执行 `DROP INDEX`, `CREATE INDEX`, `TRUNCATE TABLE` 也需要相应权限。
* **`UNIQUE`约束已移除**: 为了避免因 `Faker` 生成重复数据导致大量插入失败，`DBLab8_Base_Schema.sql` 中的 `Passenger` 表已移除了 `email`, `id_card_number`, `frequent_flyer_number` 的 `UNIQUE` 约束。实验的核心是测试 `name` 列的索引。
* **并发与死锁**: 如果在高并发插入时遇到死锁问题，可以尝试减少 `scripts/config.py` 中 `INSERTER_CONFIG['num_concurrent_inserters']` 的值。
* **脚本执行**: `inserter.py` 和 `querier.py` 需要手动修改其 `if __name__ == '__main__':` 部分来选择运行“无索引”还是“有索引”场景。
