<%--
  Created by IntelliJ IDEA.
  User: 17408
  Date: 2024/4/13
  Time: 8:54
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>修改</title>
    <style>
        .box0 {
            position: relative;
            top: 200px;
            left: 600px;
        }
    </style>
</head>
<body>
<%
    String num = request.getParameter("num");

%>
<div class="box0">
    <h2>
        正在修改----

        <%= request.getParameter("name")%>

        ----的学生信息:
    </h2>

    <form action="Student/update">
        <label>学号:</label>
        <input type="text" name="num"  value="<%= num%>" required>
        <br>
        <label>姓名:</label>
        <input type="text" name="name"  value="<%= request.getParameter("name")%>" required>
        <br>
        <label>密码:</label>
        <input type="text" name="password" required>
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
        <input type="submit" value="修改">
    </form>
</div>
</body>
</html>
