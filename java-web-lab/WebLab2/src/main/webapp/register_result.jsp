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
    String account = request.getParameter("account");
    String password = request.getParameter("password"); // 实际应用中密码应该加密存储
    String url = "jdbc:mysql://localhost:3306/javalab2";

    Connection con = null;
    PreparedStatement checkStmt = null;
    PreparedStatement insertStmt = null;

    try {
        Class.forName("com.mysql.cj.jdbc.Driver");
        con = DriverManager.getConnection(url, "root", "123456");

        // 先检查账户是否已存在
        String checkSql = "SELECT * FROM accounts WHERE account = ?";
        checkStmt = con.prepareStatement(checkSql);
        checkStmt.setString(1, account);
        ResultSet rs = checkStmt.executeQuery();

        if (rs.next()) {
            out.println("该用户名已存在，请更换用户名重新注册。");
        } else {
            // 如果账户名不存在，执行插入操作
            String insertSql = "INSERT INTO accounts(account, password) VALUES (?, ?)";
            insertStmt = con.prepareStatement(insertSql);
            insertStmt.setString(1, account);
            insertStmt.setString(2, password); // 实际应用中密码应在存储前加密

            int n = insertStmt.executeUpdate();
            out.println("注册结果: ");
            if (n > 0) {
                out.println("恭喜，注册成功！");
            } else {
                out.println("注册失败，请重试。");
            }
        }
    } catch (Exception e) {
        out.println("注册过程中出现错误：" + e.getMessage());
    } finally {
        try {
            if (checkStmt != null) checkStmt.close();
            if (insertStmt != null) insertStmt.close();
            if (con != null) con.close();
        } catch (Exception e) {}
    }
%>
<br>
<a href="login.jsp">返回登录页面</a>
</body>
</html>