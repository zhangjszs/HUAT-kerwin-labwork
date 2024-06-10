package utils;

import java.sql.*;


public class databasebean {
    public static Connection getconnection()  {
        String url = "jdbc:mysql://localhost:3306/user?characterEncoding=UTF-8&serverTimezone=Asia/Shanghai";
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
        Connection con= null;
        try {
            con = DriverManager.getConnection(url, "root", "1234");
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        ResultSet rs=null;
        PreparedStatement sta=null;
        return con;
    }
    public static void close(Connection con, PreparedStatement sta,ResultSet rs){
        if(con!=null){
            try {
                con.close();
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }
        if(rs!=null){
            try {
                rs.close();
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }
        if(sta!=null){
            try {
                sta.close();
            } catch (SQLException e) {
                throw new RuntimeException(e);
            }
        }

    }
}
