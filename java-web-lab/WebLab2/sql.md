## 数据库相关操作
1. 打开mysql的终端
2. 输入
```sql
CREATE DATABASE javalab2;

USE javalab2;

CREATE TABLE accounts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    account VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL
);
```
## 相关文件说明
1. 初始化文件为class.html 由于时间原因 只美化了初始界面，想要美化请自行修改
2. CreatUser.py会自动创建用户数据，可以进行初始化。