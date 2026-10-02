package org.example.weblab4;

import java.sql.*;

public class BaseDao implements StudentDao {
    //创建三大对象
    protected static Connection conn;
    protected static PreparedStatement ps;
    protected static ResultSet rs;
    public static final String DRIVER = "com.mysql.cj.jdbc.Driver";
    public static final String URL = "jdbc:mysql://localhost:3306/user_db";
    public static final String USERNAME = "root";
    public static final String PASSWORD = "123456";

    //创建连接数据库的方法
    public static void getConnection() {
        try {
            Class.forName(DRIVER);
            conn = DriverManager.getConnection(URL, USERNAME, PASSWORD);
            System.out.println("连接：" + conn);
        } catch (ClassNotFoundException | SQLException e) {
            throw new RuntimeException(e);
        }
    }

    //创建关闭数据库的方法
    public static void closeCount() {
        try {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
            if (conn != null) {
                conn.close();
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    //通用的增删改的方法
    public int executeUpdate(String sql, Object[] params) {
        int flag = 0;
        getConnection();
        try {
            ps = conn.prepareStatement(sql);
            if (params != null) {
                for (int i = 0; i < params.length; i++) {
                    ps.setObject(i + 1, params[i]);
                }
            }
            flag = ps.executeUpdate();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        } finally {
            closeCount();
        }
        return flag;
    }

    //通用的查询方法
    public ResultSet executeQuery(String sql, Object[] params) {
        getConnection();
        try {
            ps = conn.prepareStatement(sql);
            if (params != null) {
                for (int i = 0; i < params.length; i++) {
                    ps.setObject(i + 1, params[i]);
                }
            }
            rs = ps.executeQuery();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return rs;
    }

    @Override
    public ResultSet queryStudent(String stuno, String password) {
        String sql = "SELECT * FROM users WHERE stuno = ? AND password = ?";
        Object[] params = {stuno, password};
        return executeQuery(sql, params);
    }
}

