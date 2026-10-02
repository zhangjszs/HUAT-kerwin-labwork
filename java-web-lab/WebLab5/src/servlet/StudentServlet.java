package servlet;
import service_imp.service_imp;
import user.User;

import java.io.*;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.LinkedList;

@WebServlet("/Student/*")
public class StudentServlet extends BaseServlet {
    LinkedList<User> students=null;
    public void show(HttpServletRequest request,HttpServletResponse response) throws ServletException{
        students = new service_imp().s_get_show();
        request.getSession().setAttribute("students",students);
        try {
            response.sendRedirect("show.jsp");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }
    public void search(HttpServletRequest request,HttpServletResponse response) throws ServletException{
        String name = request.getParameter("name");  
        students=new service_imp().s_search(name);
        request.getSession().setAttribute("students",students);
        try {
            response.sendRedirect("../show.jsp");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void del (HttpServletRequest request,HttpServletResponse response) throws ServletException{
        String num = request.getParameter("num");
        new service_imp().del(num);
        try {
            response.sendRedirect("../index.jsp");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }

    public void update (HttpServletRequest request,HttpServletResponse response) throws ServletException{
        String num = request.getParameter("num");
        String name = request.getParameter("name");
        String sex = request.getParameter("sex");
        String passwords = request.getParameter("password");
        new service_imp().update(num,name,sex,passwords);
        try {
            response.sendRedirect("../index.jsp");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void adds (HttpServletRequest request,HttpServletResponse response) throws ServletException{
        String num = request.getParameter("num");
        String name = request.getParameter("name");
        String sex = request.getParameter("sex");
        new service_imp().adds(num,name,sex);
        try {
            response.sendRedirect("../index.jsp");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}