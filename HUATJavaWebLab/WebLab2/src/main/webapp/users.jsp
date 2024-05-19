<%@ page import="java.sql.*" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%--
  Created by IntelliJ IDEA.
  User: Kwrwin_zhang
  Date: 2024/5/19
  Time: 下午2:26
  To change this template use File | Settings | File Templates.
--%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>显示所有用户数据</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f4f4f4;
            color: #333;
            margin: 0;
            padding: 20px;
        }
        .container {
            background-color: #fff;
            padding: 20px;
            margin: 20px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        h1 {
            text-align: center;
            color: #333;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 10px;
            text-align: left;
        }
        th {
            background-color: #007bff;
            color: white;
        }
        tr:nth-child(even) {
            background-color: #f9f9f9;
        }
    </style>
</head>
<body>
<div class="container">
    <h1>显示所有用户数据</h1>
    <table>
        <tr>
            <th>用户名</th>
            <th>密码</th>
        </tr>
        <%
            String url = "jdbc:mysql://localhost:3306/javalab2";
            try {
                Class.forName("com.mysql.cj.jdbc.Driver");
                Connection con = DriverManager.getConnection(url, "root", "123456");
                Statement sta = con.createStatement();
                String sql = "SELECT account, password FROM accounts"; // 更新SQL查询以匹配accounts表
                ResultSet rs = sta.executeQuery(sql);
                while (rs.next()) {
        %>
        <tr>
            <td><%= rs.getString("account") %></td>
            <td><%= rs.getString("password") %></td>
        </tr>
        <%
                }
                rs.close();
                sta.close();
                con.close();
            } catch (ClassNotFoundException | SQLException e) {
                out.println("数据库连接或查询时出错：" + e.getMessage());
            }
        %>
    </table>
</div>
</body>
</html>
