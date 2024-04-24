打开mysql的终端
输入
```sql
CREATE DATABASE javawebwork2;

USE javawebwork2;

CREATE TABLE users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50) NOT NULL,
  password VARCHAR(50) NOT NULL
);
```