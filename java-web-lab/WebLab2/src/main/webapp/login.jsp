<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%--
  Created by IntelliJ IDEA.
  User: Kwrwin_zhang
  Date: 2024/5/19
  Time: 下午2:26
  To change this template use File | Settings | File Templates.
--%>
<html>
<head>
    <title>用户登录</title>
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
            margin-bottom: 20px;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box; /* 确保内边距和边框不会增加宽度 */
        }
        input[type="submit"] {
            width: 100%;
            padding: 10px;
            border: none;
            border-radius: 4px;
            background-color: #007bff;
            color: white;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #0056b3;
        }
        .hint {
            font-size: 0.8em;
            color: #666;
        }
    </style>
</head>
<body>
<h1>输入用户数据</h1>
<form action="login_result.jsp" method="post">
    <!-- 用户名输入 -->
    <label for="account">用户名：</label>
    <input type="text" id="account" name="account" required>
    <span class="hint">请输入您的用户名。</span><br>

    <!-- 密码输入 -->
    <label for="password">密码：</label>
    <input type="password" id="password" name="password">
    <span class="hint">请输入您的密码。</span><br>

    <!-- 提交按钮 -->
    <input type="submit" value="登录">
</form>
</body>
</html>
