<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <!-- <meta name="viewport" content="width=device-width, initial-scale=1.0" /> -->
    <title>实验一 JSP运行环境安装及配置</title>
    <link rel="stylesheet" href="style.css" />
    <script>
        function loadPage(url) {
            var iframe = document.querySelector(".main iframe");
            iframe.src = url;
        }
    </script>
</head>
<body>
<div class="container">
    <div class="header">
        <a href="#" onclick="loadPage('login.html')">登录</a>
        <a href="#" onclick="loadPage('register.html')">注册</a>
    </div>
    <div class="sidebar">
        <a href="#" onclick="loadPage('author.html')">作者简介</a>
    </div>
    <div class="main">
        <iframe></iframe>
    </div>
</div>
</body>
</html>