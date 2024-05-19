package org.example.weblab3;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

/**
 * @author luocu
 * @version 1.0
 * @ClassName ShowCartServlet
 * @date 2024/5/19 上午11:09
 */
@WebServlet(name = "ShowCartServlet", value = "/show-cart")
//@JstlImport(prefix = "c", uri = "http://java.sun.com/jsp/jstl/core")
public class ShowCartServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // 获取购物车
        HttpSession session = request.getSession();
        List<Book> cart = (List<Book>) session.getAttribute("cart");

        if (cart != null && !cart.isEmpty()) {
            // 计算总价
            double totalPrice = 0;
            for (Book book : cart) {
                totalPrice += book.getPrice() * book.getQuantity();
                System.out.println(book.getName() + " " + book.getPrice() + " " + book.getQuantity() + " " );
            }

            // 将总价和购物车数据传递给 JSP 页面
            request.setAttribute("cart", cart);
            request.setAttribute("totalPrice", totalPrice);
            request.getRequestDispatcher("show_cart.jsp").forward(request, response);
        } else {
            // 如果购物车为空，重定向到其他页面
            response.sendRedirect("index.html");
        }
    }
}