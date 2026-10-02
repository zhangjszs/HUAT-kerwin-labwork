<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %><%--
  Created by IntelliJ IDEA.
  User: Kwrwin_zhang
  Date: 2024/5/19
  Time: 下午2:26
  To change this template use File | Settings | File Templates.
--%>
<html>
<head>
    <meta charset="UTF-8">
    <title>用户注册</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f4f4f4;
            color: #333;
            margin: 0;
            padding: 20px;
        }
        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 20px;
        }
        form {
            max-width: 300px;
            margin: auto;
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        label {
            display: block;
            margin-bottom: 5px;
        }
        input[type="text"],
        input[type="password"] {
            width: 100%;
            padding: 10px;
            margin-bottom: 15px;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box;
        }
        input[type="submit"] {
            width: 100%;
            padding: 10px;
            border: none;
            border-radius: 4px;
            background-color: #28a745;
            color: white;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #1c7430;
        }
        p {
            text-align: center;
            margin-top: 15px;
        }
        a {
            color: #007bff;
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
    </style>
</head>
<body>
<h1>用户注册</h1>
<form action="register_result.jsp" method="post" name="registerForm" onsubmit="return validateForm()">
    <label for="account">用户名:</label>
    <input type="text" id="account" name="account" required>
    <br>
    <label for="password">密码:</label>
    <input type="password" id="password" name="password" required>
    <br>
    <input type="submit" value="注册">
</form>
<p>已有账号？<a href="login.jsp">登录</a></p>
</body>
</html>