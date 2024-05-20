<!-- index.jsp -->
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>学生登录</title>
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

        .login-container {
            background-color: white;
            padding: 30px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        h1 {
            text-align: center;
            margin-bottom: 20px;
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        td {
            padding: 10px;
        }

        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        input[type="submit"] {
            width: 100%;
            background-color: #4CAF50;
            color: white;
            padding: 10px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }

        input[type="submit"]:hover {
            background-color: #45a049;
        }

        .error-message {
            color: red;
            text-align: center;
            margin-top: 10px;
        }
    </style>
    <script>
        function validateForm() {
            var stuno = document.forms["loginForm"]["stuno"].value;
            var password = document.forms["loginForm"]["password"].value;
            if (stuno == "" || password == "") {
                alert("学号和密码不能为空！");
                return false;
            }
            return true;
        }
    </script>

</head>
<body>
<div class="login-container">
    <h1>学生登录</h1>
    <form name="loginForm" action="LoginServlet" method="post" onsubmit="return validateForm()">
        <table>
            <tr>
                <td>学号:</td>
                <td><input type="text" name="stuno"></td>
            </tr>
            <tr>
                <td>密码:</td>
                <td><input type="password" name="password"></td>
            </tr>
            <tr>
                <td colspan="2" align="right">
                    <input type="submit" value="登录">
                </td>
            </tr>
        </table>
    </form>

    <% if (request.getAttribute("error") != null) { %>
    <p class="error-message"><%= request.getAttribute("error") %></p>
    <% } %>
</div>
</body>
</html>