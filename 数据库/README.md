# 数据库课程实验

本项目包含数据库课程的实验代码，包括SQL实践和数据库应用。

## 环境要求

- **操作系统**: Windows/Linux/macOS
- **数据库**: MySQL 8.0 或更高版本
- **工具**: MySQL Workbench, phpMyAdmin

## 依赖安装

```bash
# 安装 MySQL
# Windows: 下载安装包
# Linux: sudo apt install mysql-server

# 启动 MySQL 服务
sudo systemctl start mysql
```

## 运行步骤

1. 克隆仓库：
   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/数据库
   ```

2. 导入数据库：
   ```sql
   mysql -u root -p < labwork1/schema.sql
   ```

3. 运行查询：
   ```sql
   mysql -u root -p database_name < labwork1/queries.sql
   ```

## 目录结构

- `labwork1/`：基础SQL实验
- `labwork4/`：高级查询
- `labwork5/`：存储过程和触发器

## 实验说明

实验1到实验3为线上实验，后续会整理到这里。

## 贡献

欢迎添加更多数据库实验。

## 许可

MIT License