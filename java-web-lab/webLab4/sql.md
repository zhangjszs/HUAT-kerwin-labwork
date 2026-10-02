# 数据库设置
# 创建数据库
1. ```sql
   CREATE DATABASE user_db;
    USE user_db;

    CREATE TABLE users (
    stuno VARCHAR(20) PRIMARY KEY,
    password VARCHAR(20),
    stuname VARCHAR(20)
    );
    ```
2. 插入一些数据
    ```sql
    INSERT INTO users (stuno, password, stuname) VALUES
    ('S001', 'password1', 'John Doe'),
    ('S002', 'password2', 'Jane Smith'),
    ('S003', 'password3', 'Bob Johnson');
    ```