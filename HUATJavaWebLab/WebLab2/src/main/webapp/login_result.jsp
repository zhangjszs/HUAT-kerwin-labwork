<%@ page import="java.sql.*" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String account = request.getParameter("account");
    String password = request.getParameter("password");

    Connection con = null;
    PreparedStatement pstmt = null;
    ResultSet rs = null;

    try {
        Class.forName("com.mysql.cj.jdbc.Driver");
        String url = "jdbc:mysql://localhost:3306/javalab2";
        con = DriverManager.getConnection(url, "root", "123456");

        String sql = "SELECT * FROM accounts WHERE account = ? AND password = ?";
        pstmt = con.prepareStatement(sql);
        pstmt.setString(1, account);
        pstmt.setString(2, password); // 实际生产环境中应避免明文存储密码，此处仅为示例

        rs = pstmt.executeQuery();

        boolean isLoggedIn = rs.next(); // 如果有结果，说明登录成功

%>
<html>
<head>
    <title>Login Result</title>
</head>
<body>
<%
    if (isLoggedIn) {
%>
<script type="text/javascript">
    alert("登录成功!");
    setTimeout(function () {
        window.location.href = "index.jsp";
    }, 200);
</script>
<%
} else {
%>
<script type="text/javascript">
    alert("登录失败!");
    setTimeout(function () {
        window.location.href = "login.jsp";
    }, 200);
</script>
<%
        }
    } finally {
        if (rs != null) rs.close();
        if (pstmt != null) pstmt.close();
        if (con != null) con.close();
    }
%>
</body>
</html>