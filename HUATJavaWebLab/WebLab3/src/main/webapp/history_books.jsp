<%@ page import="org.example.weblab3.Book" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="java.util.List" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>历史图书</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f8f9fa;
            color: #333;
            margin: 0;
            padding: 20px;
        }
        h1 {
            color: #333;
            text-align: center;
        }
        ul {
            list-style-type: none;
            padding: 0;
        }
        li {
            background-color: #fff;
            border: 1px solid #ddd;
            margin-bottom: 10px;
            padding: 15px;
            border-radius: 5px;
        }
        a {
            text-decoration: none;
            color: #007bff;
        }
        a:hover {
            text-decoration: underline;
        }
        .btn {
            display: inline-block;
            margin: 10px 0;
            padding: 10px 20px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 5px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
            cursor: pointer;
            transition: background-color 0.3s ease;
        }
        .btn:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
<h1>历史图书</h1>
<ul>
    <%
        List<Book> books = new ArrayList<>();
        books.add(new Book("历史书籍1", 10.0));
        books.add(new Book("历史书籍2", 15.0));
        books.add(new Book("历史书籍3", 20.0));
        books.add(new Book("历史书籍4", 25.0));

        for (Book book : books) {
    %>
    <li>
        书名：《<%= book.getName() %>》 价格：$<%= book.getPrice() %>
        <a href="add-to-cart?book=<%= book.getName() %>&page=history" class="btn">购买</a>
    </li>
    <% } %>
    <!-- 在这里添加更多历史图书 -->
</ul>
<div style="text-align: center; margin-top: 20px;">
<a href="show_cart.jsp" class="btn">显示购物车</a>
<a href="computer_books.jsp" class="btn">购买计算机图书</a>
</div>
</body>
</html>