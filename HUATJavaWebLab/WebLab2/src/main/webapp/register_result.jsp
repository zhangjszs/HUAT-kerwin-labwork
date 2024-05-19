<%@ page import="java.sql.Connection" %>
<%@ page import="java.sql.DriverManager" %>
<%@ page import="java.sql.ResultSet" %>
<%@ page import="java.sql.PreparedStatement" %>

<%@ page contentType="text/html;charset=UTF-8" language="java" pageEncoding="UTF-8" %>
<html>
<head>
    <title>用户注册结果</title>
</head>
<body>
<%
    String account = request.getParameter("account"); // 更改变量名以匹配数据库字段
    String password = request.getParameter("password"); // 密码同样需要处理，这里假设直接使用，实际应用中应加密存储
    String url = "jdbc:mysql://localhost:3306/javalab2";

    Connection con = null;
    try {
        Class.forName("com.mysql.cj.jdbc.Driver");
        con = DriverManager.getConnection(url, "root", "123456");

        String sql = "INSERT INTO accounts(account, password) VALUES (?, ?)"; // 调整SQL语句以匹配accounts表和字段
        PreparedStatement psmt = con.prepareStatement(sql);
        psmt.setString(1, account);
        psmt.setString(2, password); // 实际应用中，密码应在此处进行加密处理

        int n = psmt.executeUpdate();
        out.println("注册结果: ");
        if (n > 0) {
            out.println("恭喜，注册成功！");
        } else {
            out.println("注册失败，请重试。");
        }

    } catch (Exception e) {
        out.println("注册过程中出现错误：" + e.getMessage());
    } finally {
        if (con != null) try {
            con.close();
        } catch (Exception e) {
        }
    }
%>
<br>
<a href="login.jsp">返回登录页面</a>
</body>
</html>