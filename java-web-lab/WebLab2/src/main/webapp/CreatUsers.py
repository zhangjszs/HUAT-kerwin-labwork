import os
import secrets
import string
import mysql.connector

# 连接到MySQL数据库（口令经环境变量注入；默认值仅用于本地课程环境）
cnx = mysql.connector.connect(
    host="localhost",
    user=os.environ.get("DB_USER", "root"),
    password=os.environ.get("DB_PASSWORD", "123456"),
    database="javalab2"
)
cursor = cnx.cursor()

# 生成随机字符串（账号/口令生成使用加密安全随机源）
def generate_random_string(length):
    letters = string.ascii_letters + string.digits
    return ''.join(secrets.choice(letters) for _ in range(length))

# 生成10个随机帐户和密码，并插入数据库
for _ in range(10):
    account = generate_random_string(10) + "@qq.com"
    password = generate_random_string(10)
    query = "INSERT INTO accounts (account, password) VALUES (%s, %s)"
    print(f"Inserting account: {account}, password: {password}")
    values = (account, password)
    cursor.execute(query, values)

# 提交更改并关闭连接
cnx.commit()
cursor.close()
cnx.close()

