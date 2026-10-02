package org.example.weblab4;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;

@WebServlet(name = "LoginServlet" ,value = "/LoginServlet")
public class LoginServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException, ServletException {
        String stuno = request.getParameter("stuno");
        String password = request.getParameter("password");

        BaseDao baseDao = new BaseDao();
        ResultSet rs = baseDao.queryStudent(stuno, password);

        try {
            if (rs != null && rs.next()) {
                request.setAttribute("stuname", rs.getString("stuno"));
                request.getRequestDispatcher("welcome.jsp").forward(request, response);
            } else {
                request.setAttribute("error", "学号或密码错误,请重新登录");
                request.getRequestDispatcher("index.jsp").forward(request, response);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            request.setAttribute("error", "内部服务器错误，请稍后再试");
            request.getRequestDispatcher("error.jsp").forward(request, response);
        }
    }

}
