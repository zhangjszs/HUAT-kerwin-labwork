package org.example.weblab4;

import java.sql.ResultSet;
import java.sql.SQLException;

public class test {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("usage: test <stuno> <password>");
            return;
        }
        BaseDao baseDao = new BaseDao();
        ResultSet rs = baseDao.queryStudent(args[0], args[1]);
        try {
            while (rs != null && rs.next()) {
                // 获取并打印学生信息（口令不回显）
                System.out.println("Student ID: " + rs.getString("stuno"));
                // 可以继续获取其他列的信息
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
