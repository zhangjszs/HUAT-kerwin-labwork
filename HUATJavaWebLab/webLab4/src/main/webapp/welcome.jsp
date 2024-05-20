<%--
  Created by IntelliJ IDEA.
  User: Kwrwin_zhang
  Date: 2024/5/19
  Time: 下午5:07
  To change this template use File | Settings | File Templates.
--%>
<!-- welcome.jsp -->
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>欢迎页面</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f2f2f2;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }

        .welcome-container {
            background-color: white;
            padding: 30px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            text-align: center;
        }

        h1 {
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
<div class="welcome-container">
    <h1>欢迎, <%= request.getAttribute("stuname") %>!</h1>
    <p>您已成功登录系统。</p>
    <a href="index.jsp">返回登录</a>
</div>
</body>
</html>