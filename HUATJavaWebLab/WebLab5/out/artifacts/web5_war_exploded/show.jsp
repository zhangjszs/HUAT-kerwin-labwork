<%@ page import="java.sql.*" %>
<%@ page import="utils.databasebean" %>
<%@ page import="dateimp.DateImp" %>
<%@ page import="user.User" %>
<%@ page import="java.util.LinkedList" %>
<%--
  Created by IntelliJ IDEA.
  User: Administrator
  Date: 2024/3/13
  Time: 18:45
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <meta charset="UTF-8">
    <title>Title</title>
    <style>
        .box0 {
            position: relative;
            top: 50px;
            left: 300px;
            width: 1000px;
        }
        .box1 {
            position: relative;
            top: 50px;
            left: 300px;
            width: 1000px;
        }
        .table {
            width: 800px;
            height: auto;

        }
        .table td {
            width: 200px;
            border: 1px solid #4cff34;

        }
        .index {
            color: red;
        }

    </style>
</head>
<body>

<div class="box1">
    <h2 >显示所有用户信息</h2>
    <a href="index.jsp" class="index">首页</a>
    <a href="add.jsp" class="index">添加学生信息</a>
    <form action="Student/search" >
        <label>查询学生姓名(支持模糊查询):</label>
        <input type="text" name="name" required>
        <input type="submit" value="查询">
    </form>

    <table border="1" width="800px">
        <tr>
            <td width="200px" bgcolor="#fff8dc">学号:</td>
            <td width="200px" bgcolor="#fff8dc">姓名:</td>
            <td width="200px" bgcolor="#fff8dc">姓别:</td>
            <td width="200px" bgcolor="#fff8dc">功能</td>
        </tr>
    </table>
</div>

<%
    LinkedList<User> students = (LinkedList<User>)request.getSession().getAttribute("students");
    for(User student :students){


%>
<div class="box0">
    <table border="1" class="table">
        <tr>
            <td><%= student.getNum() %></td>
            <td><%= student.getName() %></td>
            <td><%= student.getSex() %></td>
            <td>
                <a href="Student/del?num=<%= student.getNum()%>">删除</a>
                <a href="update.jsp?num=<%= student.getNum()%> &name=<%= student.getName()%>">修改</a>
            </td>
        </tr>
    </table>
</div>

<%
    }
%>


</body>
</html>


