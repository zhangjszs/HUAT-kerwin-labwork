package org.example.weblab4;

import java.sql.ResultSet;
import java.sql.SQLException;

public class test {
    public static void main(String[] args) {
        BaseDao baseDao = new BaseDao();
        ResultSet rs = baseDao.queryStudent("S001", "password1");
        try {
            while (rs != null && rs.next()) {
                // 获取并打印学生信息
                System.out.println("Student ID: " + rs.getString("stuno"));
                System.out.println("Password: " + rs.getString("password"));
                // 可以继续获取其他列的信息
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        }
    }
