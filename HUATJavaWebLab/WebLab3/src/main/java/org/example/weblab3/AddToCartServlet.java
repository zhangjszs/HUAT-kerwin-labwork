package org.example.weblab3;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author luocu
 * @version 1.0
 * @ClassName AddToCartServlet
 * @date 2024/5/19 上午11:07
 */
@WebServlet(name = "AddToCartServlet", value = "/add-to-cart")
public class AddToCartServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // 获取要添加到购物车的书籍名称
        String bookName = request.getParameter("book");
        // 获取页面标记
        String page = request.getParameter("page");

        // 创建或获取购物车
        HttpSession session = request.getSession();
        List<Book> cart = (List<Book>) session.getAttribute("cart");
        if (cart == null) {
            cart = new ArrayList<>();
            session.setAttribute("cart", cart);
        }

        // 查找并添加书籍到购物车
        boolean found = false;
        for (Book book : cart) {
            if (book.getName().equals(bookName)) {
                book.incrementQuantity();
                found = true;
                break;
            }
        }
        if (!found) {
            for (Book book : getAllBooks()) {
                if (book.getName().equals(bookName)) {
                    cart.add(book);
                    break;
                }
            }
        }

        // 根据页面标记重定向到对应的 JSP 页面
        if (page != null && page.equals("history")) {
            response.sendRedirect("history_books.jsp");
        } else if (page != null && page.equals("computer")) {
            response.sendRedirect("computer_books.jsp");
        } else {
            response.sendRedirect("index.html");
        }
    }

    private List<Book> getAllBooks() {
        List<Book> books = new ArrayList<>();
        books.add(new Book("历史书籍1", 10.0));
        books.add(new Book("历史书籍2", 15.0));
        books.add(new Book("历史书籍3", 20.0));
        books.add(new Book("历史书籍4", 25.0));
        books.add(new Book("Java编程思想", 50.0));
        books.add(new Book("算法导论", 60.0));
        books.add(new Book("深入理解计算机系统", 70.0));
        books.add(new Book("编程珠玑", 40.0));
        return books;
    }
}