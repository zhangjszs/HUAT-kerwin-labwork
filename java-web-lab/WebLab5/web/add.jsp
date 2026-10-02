<%--
  Created by IntelliJ IDEA.
  User: 17408
  Date: 2024/4/13
  Time: 8:17
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>添加学生信息</title>
    <style>
        .box0 {
            position: relative;
            top: 200px;
            left: 600px;
        }
    </style>
</head>
<body>
<div class="box0">
    <h2>添加学生信息</h2>
    <form action="Student/adds" id="loginForm">
        <label>学号:</label>
        <input type="text" name="num" required>
        <br>
        <label>姓名:</label>
        <input type="text" name="name" required>
        <br>
        <label >性别:</label>
        <select  name="sex">
            <option value="男">男</option>
            <option value="女">女</option>
            <option value="男娘">男娘</option>
            <option value="男同">男同</option>
            <option value="女人的dog">女人的dog</option>
        </select>
        <br>
        <label>验证码:</label>
        <input type="text" name="checkcode" required>
        <img src="checkcode.jsp" name="codeImage" onclick="reflesh()">
        <br>
        <input type="submit" value="添加">
    </form>
    <script type="text/javascript">
        function reflesh() {
            loginForm.codeImage.src="checkcode.jsp?id"+Math.random();
        }
    </script>
</div>
</body>
</html>
