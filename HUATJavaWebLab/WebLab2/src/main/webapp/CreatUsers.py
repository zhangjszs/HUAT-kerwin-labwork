import random
import string
import mysql.connector

# 连接到MySQL数据库
cnx = mysql.connector.connect(
    host="localhost",
    user="root",
    password="123456",
    database="javalab2"
)
cursor = cnx.cursor()

# 生成随机字符串
def generate_random_string(length):
    letters = string.ascii_letters + string.digits
    return ''.join(random.choice(letters) for _ in range(length))

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

