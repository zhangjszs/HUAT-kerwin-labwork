<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<html>
<head>
    <title>购物车</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f4f4f4;
            color: #333;
            margin: 0;
            padding: 20px;
        }
        h1 {
            color: #333;
            text-align: center;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 20px;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
        tr:nth-child(even) {
            background-color: #f9f9f9;
        }
        p {
            text-align: right;
            margin-top: 10px;
            font-weight: bold;
        }
    </style>
</head>
<body>
<h1>购物车</h1>
<table>
    <tr>
        <th>书籍</th>
        <th>价格</th>
        <th>数量</th>
        <th>操作</th>
    </tr>
    <c:forEach items="${cart}" var="book">
        <tr>
            <td>${book.name}</td>
            <td>${book.price}</td>
            <td>${book.quantity}</td>
            <td>
<%--                <form action="/delete-book" method="POST">--%>
<%--                    <input type="hidden" name="bookId" value="${book.id}">--%>
                    <button type="submit">删除</button>
<%--                </form>--%>
            </td>
        </tr>
    </c:forEach>
</table>
<p>总价: $${totalPrice}</p>
<a href="index.html" class="btn">返回商城</a>
</body>
</html>