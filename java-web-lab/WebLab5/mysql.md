# 创建数据库javalab5

```mysql
CREATE DATABASE IF NOT EXISTS javalab5;
```

# 切换到javalab5数据库

```mysql
USE javalab5;
```

# 在javalab5数据库中创建表t_student

```mysql
-- 创建表
CREATE TABLE `users`
(
    `num`      INT NOT NULL,
    `name`     VARCHAR(20) DEFAULT NULL,
    `sex`      VARCHAR(10) DEFAULT NULL,
    `password` VARCHAR(50) DEFAULT NULL,
    PRIMARY KEY (`num`)
);
```

# 在t_student表中插入数据

```mysql
-- 插入数据
INSERT INTO `users` (`num`, `name`, `sex`, `password`)
VALUES (0999, 'abc', '女', '1234'),
       (2516, '大牛', '沃尔玛', '12345'),
       (4618, '熊二', '沃尔玛', '12345'),
       (4893, '大牛', '沃尔玛', '12345678'),
       (7132, '大牛', '沃尔玛', '12345'),
       (1223, '小三', '男', '23213');
```